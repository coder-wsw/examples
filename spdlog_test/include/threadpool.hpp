#pragma once

#include <atomic>
#include <future>
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
        m_run = false;
        m_cond.notify_all();
        for (auto& t : m_pool) {
            if (t.joinable())
                t.join();
        }
        SPDLOG_TRACE("thread pool destroy");
    };

    template <class F, class... Args>
    auto addTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        SPDLOG_INFO("template task with args");
        if (!m_run)
            throw std::runtime_error("commit on ThreadPool is stopped.");

        using RetType =
            decltype(f(args...)); // typename std::result_of<F(Args...)>::type,
                                  // 函数 f 的返回值类型

        auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(
            std::forward<F>(f),
            std::forward<Args>(args)...)); // 把函数入口及参数,打包(绑定)

        std::future<RetType> future = task->get_future();

        { // 添加任务到队列
            // 对当前块的语句加锁  lock_guard 是 mutex 的 stack
            // 封装类，构造的时候 lock()，析构的时候 unlock()
            std::lock_guard<std::mutex> lock{m_lock};
            m_tasks.emplace([task]() { // push(Task{...}) 放到队列后面
                (*task)();
            });
        }

        if (m_idlThrNum < 1 && m_pool.size() < MAX_SIZE)
            addThread(1);
        m_cond.notify_one(); // 唤醒一个线程执行
        return future;
    }

    template <class F> void addTask2(F&& task)
    {
        SPDLOG_INFO("template task without args");
        if (!m_run)
            throw std::runtime_error("commit on ThreadPool is stopped.");
        {
            std::lock_guard<std::mutex> lock{m_lock};
            m_tasks.emplace(std::forward<F>(task));
        }

        if (m_idlThrNum < 1 && m_pool.size() < MAX_SIZE)
            addThread(1);
        m_cond.notify_one(); // 唤醒一个线程执行
    }

    void addTask(std::function<void()> task)
    {
        SPDLOG_INFO("normal task");
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
                        std::function<void()> task;
                        {
                            if (!m_run && m_tasks.empty()) {
                                SPDLOG_TRACE("thread end");
                                return;
                            }
                            std::unique_lock<std::mutex> lock{m_lock};
                            m_cond.wait(lock, [this]() {
                                return !m_run || !m_tasks.empty();
                            });
                            task = std::move(m_tasks.front());
                            m_idlThrNum--;
                            m_tasks.pop();
                        }
                        task();
                        {
                            std::lock_guard<std::mutex> lock{m_lock};
                            m_idlThrNum++;
                        }
                    }
                    {
                        std::lock_guard<std::mutex> lock{m_lock};
                        m_idlThrNum++;
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
    std::atomic<bool> m_run{true};   // 线程池是否执行
    std::atomic<int> m_idlThrNum{0}; // 空闲线程数量
};