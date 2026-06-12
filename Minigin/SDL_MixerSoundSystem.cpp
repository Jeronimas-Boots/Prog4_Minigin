#include "SDL_MixerSoundSystem.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <mutex>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <variant>

#ifndef __EMSCRIPTEN__
#include <thread>
#endif

namespace dae
{
    struct PlaySfxMessage
    {
        std::string filepath;
        float       volume;
    };

    struct PlayMusicMessage
    {
        std::string filepath;
        float       volume;
        int         loops; // -1 = infinite, 0 = once
    };

    using SoundMessage = std::variant<PlaySfxMessage, PlayMusicMessage>;

    class SDL_MixerSoundSystem::SoundSystemImpl
    {
    public:
        SoundSystemImpl()
        {
            if (!MIX_Init())
            {
                std::cerr << "MIX_Init failed: " << SDL_GetError() << "\n";
                return;
            }

            m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
            if (!m_Mixer)
            {
                std::cerr << "MIX_CreateMixerDevice failed: " << SDL_GetError() << "\n";
                return;
            }

#ifndef __EMSCRIPTEN__
            m_Thread = std::thread(&SoundSystemImpl::ProcessLoop, this);
#endif
        }

        ~SoundSystemImpl()
        {
#ifndef __EMSCRIPTEN__
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                m_Quit = true;
            }
            m_Condition.notify_one();
            if (m_Thread.joinable())
                m_Thread.join();
#endif

            if (m_Mixer)
            {
                // Stop all audio first
                MIX_StopAllTracks(m_Mixer, 0);

                // Clear callbacks BEFORE DestroyMixer destroys the tracks internally
                if (m_MusicTrack)
                    MIX_SetTrackStoppedCallback(m_MusicTrack, nullptr, nullptr);
                for (auto* track : m_SfxTracks)
                    MIX_SetTrackStoppedCallback(track, nullptr, nullptr);

                // DestroyMixer automatically destroys ALL tracks — don't call MIX_DestroyTrack manually
                MIX_DestroyMixer(m_Mixer);
                m_Mixer = nullptr;
            }

            // Audio is NOT destroyed by MIX_DestroyMixer, so free it manually
            for (auto& [path, audio] : m_Audio)
                MIX_DestroyAudio(audio);
            m_Audio.clear();

            MIX_Quit();
        }

        void EnqueueSfx(const std::string& filepath, float volume)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.push(PlaySfxMessage{ filepath, volume });
#ifndef __EMSCRIPTEN__
            m_Condition.notify_one();
#endif
        }

        void EnqueueMusic(const std::string& filepath, float volume, int loops)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.push(PlayMusicMessage{ filepath, volume, loops });
#ifndef __EMSCRIPTEN__
            m_Condition.notify_one();
#endif
        }

        void Update()
        {
#ifdef __EMSCRIPTEN__
            DrainQueue();
#endif
        }

    private:
#ifndef __EMSCRIPTEN__
        void ProcessLoop()
        {
            while (true)
            {
                std::unique_lock<std::mutex> lock(m_Mutex);
                m_Condition.wait(lock, [this] { return m_Quit || !m_Queue.empty(); });

                if (m_Quit && m_Queue.empty())
                    break;

                std::queue<SoundMessage> local;
                std::swap(local, m_Queue);
                lock.unlock();

                while (!local.empty())
                {
                    Dispatch(local.front());
                    local.pop();
                }
            }
        }
#endif

        void DrainQueue()
        {
            std::queue<SoundMessage> local;
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                std::swap(local, m_Queue);
            }
            while (!local.empty())
            {
                Dispatch(local.front());
                local.pop();
            }
        }

        void Dispatch(const SoundMessage& msg)
        {
            std::visit([this](auto&& m) { Handle(m); }, msg);
        }

        void Handle(const PlaySfxMessage& msg)
        {
            if (!m_Mixer) return;

            MIX_Audio* audio = GetAudio(msg.filepath);
            if (!audio) return;

            // Use fire-and-forget for SFX — SDL_mixer manages the track lifetime
            MIX_Track* track = MIX_CreateTrack(m_Mixer);
            if (!track)
            {
                std::cerr << "MIX_CreateTrack failed: " << SDL_GetError() << "\n";
                return;
            }

            MIX_SetTrackGain(track, msg.volume);
            MIX_SetTrackAudio(track, audio);

            // Set stopped callback to auto-destroy the track when done
            MIX_SetTrackStoppedCallback(track, OnSfxTrackStopped, this);

            MIX_PlayTrack(track, 0);

            std::lock_guard<std::mutex> lock(m_TrackMutex);
            m_SfxTracks.push_back(track);
        }

        void Handle(const PlayMusicMessage& msg)
        {
            if (!m_Mixer) return;

            MIX_Audio* audio = GetAudio(msg.filepath);
            if (!audio) return;

            // Reuse a single music track
            if (!m_MusicTrack)
                m_MusicTrack = MIX_CreateTrack(m_Mixer);

            if (!m_MusicTrack)
            {
                std::cerr << "MIX_CreateTrack (music) failed: " << SDL_GetError() << "\n";
                return;
            }

            MIX_SetTrackGain(m_MusicTrack, msg.volume);
            MIX_SetTrackAudio(m_MusicTrack, audio);

            // Set loop count via properties
            SDL_PropertiesID props = SDL_CreateProperties();
            SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, msg.loops);
            MIX_PlayTrack(m_MusicTrack, props);
            SDL_DestroyProperties(props);
        }

        static void SDLCALL OnSfxTrackStopped(void* userdata, MIX_Track* track)
        {
            // Mark track for cleanup — called from mixer thread, so just flag it
            auto* self = static_cast<SoundSystemImpl*>(userdata);
            std::lock_guard<std::mutex> lock(self->m_TrackMutex);
            self->m_FinishedTracks.push_back(track);
        }

        MIX_Audio* GetAudio(const std::string& path)
        {
            auto it = m_Audio.find(path);
            if (it != m_Audio.end()) return it->second;

            MIX_Audio* audio = MIX_LoadAudio(m_Mixer, path.c_str(), false);
            if (!audio)
                std::cerr << "MIX_LoadAudio failed (" << path << "): " << SDL_GetError() << "\n";
            else
                m_Audio[path] = audio;
            return audio;
        }

        MIX_Mixer* m_Mixer{ nullptr };
        MIX_Track* m_MusicTrack{ nullptr };

        std::unordered_map<std::string, MIX_Audio*> m_Audio;
        std::vector<MIX_Track*>                     m_SfxTracks;
        std::vector<MIX_Track*>                     m_FinishedTracks;

        std::queue<SoundMessage> m_Queue;
        std::mutex               m_Mutex;
        std::mutex               m_TrackMutex;
        bool                     m_Quit{ false };

#ifndef __EMSCRIPTEN__
        std::condition_variable m_Condition;
        std::thread             m_Thread;
#endif
    };

    SDL_MixerSoundSystem::SDL_MixerSoundSystem()
        : m_pImpl(std::make_unique<SoundSystemImpl>())
    {
    }

    SDL_MixerSoundSystem::~SDL_MixerSoundSystem() = default;

    void SDL_MixerSoundSystem::Play(const std::string& filepath, float volume, int /*loops*/)
    {
        m_pImpl->EnqueueSfx(filepath, volume);
    }

    void SDL_MixerSoundSystem::PlayMusic(const std::string& filepath, float volume, int loops)
    {
        m_pImpl->EnqueueMusic(filepath, volume, loops);
    }

    void SDL_MixerSoundSystem::Update()
    {
        m_pImpl->Update();
    }
}