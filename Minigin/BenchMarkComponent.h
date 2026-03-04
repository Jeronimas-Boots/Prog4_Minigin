#pragma once
#include "Component.h"
#include <vector>

namespace dae
{
    class BenchmarkComponent final : public Component
    {
    public:
        explicit BenchmarkComponent(GameObject* owner);
        void Render() const override;

    private:
        void RunBenchmarkEx1();
        void RunBenchmarkEx2();
        void RunBenchmarkEx3();

        std::vector<float> ProcessResults(const std::vector<float>& rawData, int numSamples) const;

        std::vector<float> m_TimesEx1;
        std::vector<float> m_TimesEx2;
        std::vector<float> m_TimesEx2Alt;

        int m_SamplesEx1 = 10;
        int m_SamplesEx2 = 10;
    };
}