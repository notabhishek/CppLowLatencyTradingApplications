#pragma once 
#include <iostream>
#include <atomic>
#include <thread>
#include <string>
#include <pthread.h>
#include <chrono>
#include <memory>

namespace Common{

    // Set affinity of the current thread to be pinned to core_id
    inline auto setThreadCore(int core_id) noexcept {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);
        return (pthread_set_affinity_np(pthread_self(), sizeof(cpuset), &cpuset) == 0);
    }

    // Create a thread, set affinity on it, assign it a name, forward the function to be run on the thread and the args
    template<typename Func, typename... Args>
    inline auto createAndStartThread(int core_id, const std::string& name, Func&& func, Args&&... args) noexcept {
        std::atomic<bool> running(false);
        std::atomic<bool> failed(false);

        auto thread_body = [&] {
            // set thread affinity 
            if(core_id >= 0 && !setThreadCore(core_id)) {
                std::cerr << "Failed to set core affinity for " << name << " " << pthread_self() << " to " << core_id << std::endl;
                failed = true;
                return;
            }

            std::cout << "Set core affinity for " << name << " " << pthread_self() << " to " << core_id << std::endl;
            running = true;
            std::forward<Func>(func)(std::forward<Args>(args)...); 
        };

        auto thread_ptr = std::make_unique<std::thread>(thread_body);
        while(!running && !failed) {
            using namespace std::chrono::literals;
            std::this_thread::sleep_for(1s);
        } 

        if(failed) {
            thread_ptr->join();
            thread_ptr.reset();
            thread_ptr = nullptr;
        }

        return thread_ptr;
    }
} // namespace Common
