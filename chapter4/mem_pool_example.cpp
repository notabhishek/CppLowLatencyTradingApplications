#include "mem_pool.h"

struct MyStruct {
    int d_[3];
};

std::ostream& operator<<(std::ostream& os, const MyStruct& s) noexcept {
    os << "MyStruct{" << s.d_[0] << "," << s.d_[1] << "," << s.d_[2] << "}";
    return os;
}

int main() {
    using namespace Common;
    const int size = 10;
    MemPool<double> prim_pool(size);
    MemPool<MyStruct> struct_pool(size);

    const int objects = 12;
    for(int i = 0; i < objects; ++i) {
        auto p_ret = prim_pool.allocate(i);
        auto s_ret = struct_pool.allocate(MyStruct{i, i+1, i+2});

        std::cout << "prim_element:  " << *p_ret << " allocated at:" << p_ret << std::endl;
        std::cout << "struct_element: " << *s_ret << " allocated at:" << s_ret << std::endl;
    
        if(i % 3 == 0) {
            std::cout << "deallocating prim elem: " << *p_ret << " from: " << p_ret << std::endl;
            std::cout << "deallocating struct elem:  " << *s_ret << " from: " << s_ret << std::endl;
            prim_pool.deallocate(p_ret);
            struct_pool.deallocate(s_ret); 
        }
    }

    return 0;
}