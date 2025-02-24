#include "thread_utils.h"

auto testFunction(int a, int b, bool sleep) {
    std::cout << "testFunction(" << a << "," << b << ")" << std::endl;
    std::cout << "testFunction(" << a + b << std::endl;
    if(sleep) {
        std::cout << "testFunction sleeping..." << std::endl;
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(5s);
    }
    std::cout << "testFunction done." << std::endl;
}

int main() {
    using namespace Common;
    std::unique_ptr t1 = createAndStartThread(-1, "testFunction1", testFunction, 10, 15, false);
    std::unique_ptr t2 = createAndStartThread(1, "testFunction2", testFunction, 20, 25, true);

    std::cout << "main waiting for threads to be done." << std::endl;
    t1 -> join();
    t2 -> join();
    std::cout << "main exiting." << std::endl;
    return 0;
}