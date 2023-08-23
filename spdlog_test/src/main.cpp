#include <functional>
#include <iostream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "test.hpp"
#include "threadpool.hpp"

int main(int argc, char* argv[])
{
    try {
        // Create basic file logger (not rotated)
        auto my_logger =
            spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");

        // create a file rotating logger with 5mb size max and 3 rotated files
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/rotate", 1024 * 5, 10, true));

        // 调整各个输出端的日志等级
        sinks[0]->set_level(spdlog::level::trace);
        sinks[1]->set_level(spdlog::level::trace);

        auto logger = std::make_shared<spdlog::logger>("", std::begin(sinks),
                                                       std::end(sinks));
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t][%l] %v");
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);

        threadpool pool(4);

        Test test;
        int i = 0;
        std::thread t1([&]() {
            while (true) {
                SPDLOG_INFO("t1 {}", i);
                // std::this_thread::sleep_for(std::chrono::seconds(1));
                pool.addTask(std::bind(&Test::print, &test, i));
                // pool.addTask([&]() {
                //     std::lock_guard<std::mutex> lock(test.m_mutex);
                //     test.print(5);
                // });
                i++;
            }
        });
        t1.detach();
        pool.start();
        SPDLOG_INFO("main end");
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
    return EXIT_SUCCESS;
}