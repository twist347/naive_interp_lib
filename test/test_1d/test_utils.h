#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>

template<class T>
void print(const T &t) {
    for (auto &val: t) {
        std::cout << val << '\n';
    }
}

template<class T>
auto arrays_eq(T &&real, T &&expected) -> bool {
    using idx_t = std::remove_cvref_t<T>::size_type;
    if (real.size() != expected.size()) {
        return false;
    }
    for (idx_t i = 0; i < real.size(); ++i) {
        auto r = real[i], e = expected[i];
        if (std::isnan(r) || std::isnan(r) || r != e) {
            std::cerr << std::fixed << std::setprecision(8) << "real: " << r << ", expected: " << e << " at idx " << i
                      << '\n';
            return false;
        }
    }
    return true;
}
