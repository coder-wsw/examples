#pragma once

#include <iostream>
#include <queue>
#include <thread>
#include <vector>

#include "spdlog/spdlog.h"

#define MAX_SIZE 10
class threadpool {
public:
    threadpool(size_t n = 10)
    {
        SPDLOG_TRACE("thread pool init");
        addThread(n);
    };
    ~threadpool()
    {
        for (auto& t : m_pool) {
            if (t.joinable())
                t.join();
        }
        SPDLOG_TRACE("thread pool destroy");
    };

    void addTask(std::function<void()> task)
    {
        std::lock_guard<std::mutex> lock{m_lock};
        m_tasks.emplace(task);
        m_cond.notify_one();
    }

    void addThread(size_t size)
    {
        for (; m_pool.size() < MAX_SIZE && size > 0; --size) {
            m_pool.emplace_back([this]() {
                SPDLOG_TRACE("thread start");
                try {
                    while (true) {
                        {
                            std::unique_lock<std::mutex> lock{m_lock};
                            m_cond.wait(lock,
                                        [this]() { return !m_tasks.empty(); });
                            auto task = m_tasks.front();
                            task();
                            m_tasks.pop();
                            SPDLOG_INFO("task done");
                        }
                    }
                }
                catch (const std::exception& e) {
                    SPDLOG_ERROR("{}", e.what());
                }

                SPDLOG_TRACE("thread end");
            });
        }
    }

    void start()
    {
        SPDLOG_TRACE("thread pool start");
        for (auto& t : m_pool) {
            t.join();
        }
    }

private:
    std::vector<std::thread> m_pool;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_lock;
    std::condition_variable m_cond;
};