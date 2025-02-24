#include "lf_queue.h"
#include "thread_utils.h"
#include <memory>

struct MyStruct {
    int d_[3];
};

inline std::ostream& operator<<(std::ostream& os, const MyStruct& s) {
    os << "MyStruct{" << s.d_[0] << "," << s.d_[1] << "," << s.d_[2] << "}";
    return os;
}

using namespace Common;

void consumeFunc(LFQueue<MyStruct>* lfq) {
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(5s);

    while(lfq->size()) {
        const auto data = *lfq->getNextToRead();
        lfq->updateReadIndex();

        std::cout << "consumeFunc read elem:" << data << " lfq-size:" << lfq->size() << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

int main() {
    LFQueue<MyStruct> lfq(20);
    std::unique_ptr consThread = createAndStartThread(-1, "consumerThread", consumeFunc, &lfq);
    
    for(auto i = 0; i < 50; ++i) {
        const MyStruct d{i, i*10, i*100};

        *lfq.getNextToWriteTo() = d;
        lfq.updateWriteIndex();

        std::cout << "main constructed element: " << d << " lfq-size:" << lfq.size() << std::endl;
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    consThread->join();
    std::cout << "main exiting!" << std::endl;

    return 0;
}