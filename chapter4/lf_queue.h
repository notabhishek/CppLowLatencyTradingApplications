#pragma once 
#include <iostream>
#include <vector>
#include <atomic>
#include "macros.h"

namespace Common {
    template<typename T>
    class LFQueue final {
    private: 
        std::vector<T> store_;
        std::atomic<size_t> next_write_index_ = {0};
        std::atomic<size_t> next_read_index_ = {0};
        std::atomic<size_t> num_elements_ = {0};
    
    public: 
        explicit LFQueue(size_t numElems) 
          : store_(numElems, T()) 
        { }

        LFQueue() = delete;
        LFQueue(const LFQueue&) = delete;
        LFQueue(const LFQueue&&) = delete;
        LFQueue& operator=(const LFQueue&) = delete;
        LFQueue& operator=(LFQueue&&) = delete;

        auto getNextToWriteTo() noexcept -> T* {
            return &store_[next_write_index_];
        }  

        void updateWriteIndex() noexcept {
            next_write_index_ = (next_write_index_ + 1) % store_.size();
            ++num_elements_;
        }

        auto getNextToRead() const noexcept -> const T* {
            return (next_read_index_ == next_write_index_) ? nullptr : &store_[next_read_index_];
        }

        void updateReadIndex() noexcept {
            next_read_index_ = (next_read_index_ + 1) % store_.size();
            ASSERT(num_elements_ != 0, "Read an invalid element in:" + std::to_string(pthread_self()));
            --num_elements_;
        }

        auto size() const noexcept {
            return num_elements_.load();
        }
    };
} // namespace Common