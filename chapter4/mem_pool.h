#pragma once 
#include <vector>
#include <string>
#include <utility>
#include "macros.h"

namespace Common {

    template<typename T>
    class MemPool final {
    private: 
        struct ObjectBlock {
            T object_;
            bool is_free_ = true;
        };

        std::vector<ObjectBlock> store_;
        size_t next_free_index_ = 0;
    
    public: 
        explicit MemPool(size_t numElems)
        : store_(numElems, {T(), true}) {
            ASSERT(reinterpret_cast<const ObjectBlock*>(&store_[0].object_) == &(store_[0]), "T object should be first member of ObjectBlock");
        }

        MemPool() = delete;
        MemPool(const MemPool&) = delete;
        MemPool(MemPool&&) = delete;
        MemPool& operator=(const MemPool&) = delete;
        MemPool& operator=(MemPool&&) = delete;

        template<typename... Args>
        T* allocate(Args&&... args) noexcept {
            auto obj_block = &store_[next_free_index_];
            ASSERT(obj_block->is_free_, "Expected free ObjectBlock at index:" + std::to_string(next_free_index_));

            T* ret = &(obj_block->object_);
            ret = new(ret) T(std::forward<Args>(args)...);
            obj_block->is_free_ = false;
            updateNextFreeIndex();
            return ret;
        }

        void deallocate(const T* elem) noexcept {
            const auto elem_index = (reinterpret_cast<const ObjectBlock*>(elem) - &store_[0]);
            ASSERT(elem_index >= 0 && static_cast<size_t>(elem_index) < store_.size(), "Element being deallocated does not belong to this memory pool.");

            ASSERT(!store_[elem_index].is_free_, "Expected in-use ObjectBlock at index:" + std::to_string(elem_index));
            store_[elem_index].is_free_ = true;
        }
    private: 
        void updateNextFreeIndex() noexcept {
            const auto initial_free_index = next_free_index_;
            while(!store_[next_free_index_].is_free_) {
                ++next_free_index_;

                if(next_free_index_ == store_.size()) [[unlikely]] {
                    next_free_index_ = 0;
                }

                if(next_free_index_ == initial_free_index) [[unlikely]] {
                    ASSERT(initial_free_index != next_free_index_, "Memory pool out of space.");
                }
            }
        }
    };

} // namespace Common