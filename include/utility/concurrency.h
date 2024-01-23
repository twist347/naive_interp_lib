#pragma once

#include <thread>

namespace ni {
    const unsigned int thread_num = std::thread::hardware_concurrency();

    struct parallel {
        bool omp_flag = false;
        [[nodiscard]] auto num_threads() const -> unsigned int {
            if (omp_flag) {
                return ni::thread_num;
            }
            return 1;
        }
    };
}