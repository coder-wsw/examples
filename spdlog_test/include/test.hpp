#ifndef TEST
#define TEST
#include "spdlog/spdlog.h"

class Test {
public:
    Test()
    {
        SPDLOG_TRACE("Test init");
    };
    ~Test()
    {
        SPDLOG_TRACE("Test deinit");
    };
    void print(int i)
    {
        SPDLOG_INFO("test {}", i);
    };

    int calc(int a, int b)
    {
        SPDLOG_INFO("calc {} + {} = {}", a, b, a + b);
        return a + b;
    }

    std::mutex m_mutex;
};

#endif // !TEST