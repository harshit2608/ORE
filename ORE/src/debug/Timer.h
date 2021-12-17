#pragma once

namespace ORE
{
    class Timer
    {
    public:
        Timer(const std::string &name = "Timer")
            : m_Name(name)
        {
            Reset();
        }

        ~Timer()
        {
            // ORE_CORE_INFO("{0} took {1} seconds {2} ms", m_Name, Elapsed(), ElapsedMillis());
        }

        void PrintTime()
        {
            ORE_CORE_INFO("{0} took {1} seconds and {2} ms", m_Name, Elapsed(), ElapsedMillis());
        }

        void Reset()
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

    private:
        float Elapsed()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
        }

        float ElapsedMillis()
        {
            return Elapsed() * 1000.0f;
        }

    private:
        std::string m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };
} // namespace ORE
