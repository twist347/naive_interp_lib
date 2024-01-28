#pragma once

#include <thread>

namespace ni {

    struct parallel {
        bool omp_flag = false;

        const static inline unsigned int thread_num = std::thread::hardware_concurrency();

        [[nodiscard]] auto num_threads() const -> unsigned int {
            if (omp_flag) {
                return thread_num;
            }
            return 1;
        }
    };
}