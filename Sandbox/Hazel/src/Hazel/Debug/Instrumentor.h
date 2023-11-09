//
// Created by ByteDance on 2023/11/7.
//

#pragma once

#include <string>
#include <fstream>
#include <thread>

namespace Hazel {
    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    public:
        Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0)
        {

        }

        void WriteHeader()
        {
            m_OutputStream << R"({"otherData":{}, "traceEvents":[ )";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            HZ_CORE_INFO("BeginSession----file = {0}", filepath);

            m_OutputStream.open(filepath);
            WriteHeader();
            m_CurrentSession = new InstrumentationSession{name};
        }


        void EndSession()
        {
            HZ_CORE_INFO("EndSession----");

            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            if (m_ProfileCount++ > 0) {
                m_OutputStream << ",";
            }
            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '\"', '\'');

            m_OutputStream << "{";
            m_OutputStream << R"("cat":"function",)";
            m_OutputStream << R"("dur":)" << (result.End - result.Start) << ',';
            m_OutputStream << R"("name":")" << name << "\",";
            m_OutputStream << R"("ph":"X",)";
            m_OutputStream << R"("pid":0,)";
            m_OutputStream << R"("tid":)" << result.ThreadID << ",";
            m_OutputStream << R"("ts":)" << result.Start;
            m_OutputStream << "}";
            m_OutputStream.flush();
        }

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
        {
            m_StartTimePoint = std::chrono::high_resolution_clock::now();
        }
        ~InstrumentationTimer()
        {
            if(!m_Stopped){
                Stop();
            }
        }

        void Stop()
        {
            auto endTimePoint = std::chrono::high_resolution_clock::now();
            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>()(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({m_Name, start, end, threadID});

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
        bool m_Stopped;
    };
}
#define HZ_CONCATENATE_IMPL(x, y) x##y
#define HZ_CONCATENATE(x, y) HZ_CONCATENATE_IMPL(x, y)
#define HZ_PROFILE 1
#if HZ_PROFILE
    #define HZ_PROFILE_BEGIN_SESSION(name, filepath) ::Hazel::Instrumentor::Get().BeginSession(name, filepath)
    #define HZ_PROFILE_END_SESSION() ::Hazel::Instrumentor::Get().EndSession()
    #define HZ_PROFILE_SCOPE(name) ::Hazel::InstrumentationTimer HZ_CONCATENATE(timer, __LINE__) (name)
    #define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
#define HZ_PROFILE_BEGIN_SESSION(name, filepath)
    #define HZ_PROFILE_END_SESSION()
    #define HZ_PROFILE_SCOPE(name)
    #define HZ_PROFILE_FUNCTION()
#endif
