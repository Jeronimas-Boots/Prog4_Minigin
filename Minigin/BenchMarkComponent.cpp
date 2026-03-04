#include "BenchmarkComponent.h"
#include <imgui.h>
#include <chrono>
#include <algorithm>
#include <numeric>

struct Transform
{
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

struct GameObjectData
{
    Transform local;
    int id = 0;
};

struct GameObjectDataAlt
{
    Transform* local;
    int id = 0;
};

dae::BenchmarkComponent::BenchmarkComponent(dae::GameObject* owner)
    : Component(owner)
{
}

std::vector<float> dae::BenchmarkComponent::ProcessResults(const std::vector<float>& rawData, int numSamples) const
{
    std::vector<float> processedData;
    const int numStepSizes = 11; // 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024

    // Process each step size
    for (int stepIdx = 0; stepIdx < numStepSizes; ++stepIdx)
    {
        // Collect all samples for this step size
        std::vector<float> samples;
        for (int sampleIdx = 0; sampleIdx < numSamples; ++sampleIdx)
        {
            int index = sampleIdx * numStepSizes + stepIdx;
            if (index < rawData.size())
            {
                samples.push_back(rawData[index]);
            }
        }

        if (samples.empty()) continue;

        // Sort samples to identify outliers
        std::sort(samples.begin(), samples.end());

        // Remove outliers (top and bottom 10%, minimum 1 from each end if we have enough samples)
        if (samples.size() > 2)
        {
            const size_t removeCount = std::max(static_cast<size_t>(1), samples.size() / 10);
            if (samples.size() > removeCount * 2)
            {
                samples.erase(samples.begin(), samples.begin() + removeCount);
                samples.erase(samples.end() - removeCount, samples.end());
            }
        }

        // Calculate average
        float average = std::accumulate(samples.begin(), samples.end(), 0.0f) / samples.size();
        processedData.push_back(average);
    }

    return processedData;
}

void dae::BenchmarkComponent::RunBenchmarkEx1()
{
    m_TimesEx1.clear();
    std::vector<float> rawData;

    std::vector<int> arr;
    arr.resize(100'000'000);

    for (int j = 0; j < m_SamplesEx1; j++)
    {
        for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < arr.size(); i += stepSize)
            {
                arr[i] += 10;
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            rawData.push_back(static_cast<float>(elapsedTime));
        }
    }

    // Process results: remove outliers and calculate averages
    m_TimesEx1 = ProcessResults(rawData, m_SamplesEx1);
}

void dae::BenchmarkComponent::RunBenchmarkEx2()
{
    m_TimesEx2.clear();
    std::vector<float> rawData;

    std::vector<GameObjectData> arr;
    arr.resize(100'000'000);

    for (int j = 0; j < m_SamplesEx2; j++)
    {
        for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < arr.size(); i += stepSize)
            {
                arr[i].id += 10;
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            rawData.push_back(static_cast<float>(elapsedTime));
        }
    }

    // Process results: remove outliers and calculate averages
    m_TimesEx2 = ProcessResults(rawData, m_SamplesEx2);
}

void dae::BenchmarkComponent::RunBenchmarkEx3()
{
    m_TimesEx2Alt.clear();
    std::vector<float> rawData;

    std::vector<GameObjectDataAlt> arr;
    arr.resize(100'000'000);

    for (int j = 0; j < m_SamplesEx2; j++)
    {
        for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < arr.size(); i += stepSize)
            {
                arr[i].id += 10;
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            rawData.push_back(static_cast<float>(elapsedTime));
        }
    }

    // Process results: remove outliers and calculate averages
    m_TimesEx2Alt = ProcessResults(rawData, m_SamplesEx2);
}

void dae::BenchmarkComponent::Render() const
{
    if (ImGui::Begin("Exercise 1"))
    {
        ImGui::Text("# Samples:");
        ImGui::SameLine();
        static int numSamples = m_SamplesEx1;
        ImGui::InputInt("##Samples", &numSamples);

        if (ImGui::Button("Run Benchmark"))
        {
            const_cast<BenchmarkComponent*>(this)->m_SamplesEx1 = numSamples;
            const_cast<BenchmarkComponent*>(this)->m_TimesEx1.clear();
            const_cast<BenchmarkComponent*>(this)->RunBenchmarkEx1();
        }

        if (!m_TimesEx1.empty())
        {
            ImGui::PlotLines("##Ex1Plot", m_TimesEx1.data(), static_cast<int>(m_TimesEx1.size()),
                0, nullptr, 0.0f, *std::max_element(m_TimesEx1.begin(), m_TimesEx1.end()), ImVec2(0, 100));
        }
    }
    ImGui::End();

    if (ImGui::Begin("Exercise 2"))
    {
        ImGui::Text("# Samples:");
        ImGui::SameLine();
        static int numSamples = m_SamplesEx2;
        ImGui::InputInt("##Samples", &numSamples);

        if (ImGui::Button("Thrash the cache with GameObjectData"))
        {
            const_cast<BenchmarkComponent*>(this)->m_SamplesEx2 = numSamples;
            const_cast<BenchmarkComponent*>(this)->m_TimesEx2.clear();
            const_cast<BenchmarkComponent*>(this)->RunBenchmarkEx2();
        }

        if (!m_TimesEx2.empty())
        {
            ImGui::PlotLines("##Ex2Plot", m_TimesEx2.data(), static_cast<int>(m_TimesEx2.size()),
                0, nullptr, 0.0f, *std::max_element(m_TimesEx2.begin(), m_TimesEx2.end()), ImVec2(0, 100));
        }

        if (ImGui::Button("Thrash the cache with GameObjectDataAlt"))
        {
            const_cast<BenchmarkComponent*>(this)->m_SamplesEx2 = numSamples;
            const_cast<BenchmarkComponent*>(this)->m_TimesEx2Alt.clear();
            const_cast<BenchmarkComponent*>(this)->RunBenchmarkEx3();
        }

        if (!m_TimesEx2Alt.empty())
        {
            ImGui::PlotLines("##Ex3Plot", m_TimesEx2Alt.data(), static_cast<int>(m_TimesEx2Alt.size()),
                0, nullptr, 0.0f, *std::max_element(m_TimesEx2Alt.begin(), m_TimesEx2Alt.end()), ImVec2(0, 100));
        }
    }
    ImGui::End();
}