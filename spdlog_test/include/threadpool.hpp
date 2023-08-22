#pragma once
#include <thread>
#include <vector>
#include <queue>
#include "spdlog/spdlog.h"

class threadpool
{
public:
    threadpool(size_t n){ 
        SPDLOG_TRACE("thread pool init");
        for(size_t i = 0; i < n; i++){
            m_pool.push_back(std::thread([this](){
                SPDLOG_TRACE("thread start");
                while(true){
                    if(m_tasks.empty()){
                        // SPDLOG_TRACE("task empty");
                        continue;
                    }
                    auto task = m_tasks.front();
                    m_tasks.pop();
                    task();
                    SPDLOG_INFO("task done");
                }
                // std::this_thread::sleep_for(std::chrono::seconds(1));
                SPDLOG_TRACE("thread end");
            }));
        }
    };
    ~threadpool() { SPDLOG_TRACE("thread pool destroy");};

    void addTask(std::function<void()> task){
        m_tasks.push(task);
    }

    void start(){
        SPDLOG_TRACE("thread pool start");
        for(auto& t : m_pool){
            t.detach();
        }
    }
private:
    std::vector<std::thread> m_pool;
    std::queue<std::function<void()>> m_tasks;
};