#ifndef TEST
#define TEST
#include "spdlog/spdlog.h"

class Test
{
public:
    Test(){ SPDLOG_TRACE("Test init");};
    ~Test(){ SPDLOG_TRACE("Test deinit");};
    void print(int i){
        SPDLOG_INFO("test {}",i);
    };

};

#endif // !TEST