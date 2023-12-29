#pragma once

#include <cmath>

namespace ni::utils {

    template<class T>
    concept is_flt = std::is_floating_point_v<T>;

    template<is_flt T, is_flt U>
    constexpr auto to(U u) -> T {
        return static_cast<T>(u);
    }

    template<is_flt T>
    auto eps = to<T>(1e-8);

    template<is_flt T>
    constexpr auto eq_flt(T lhs, T rhs, T e = eps<T>) -> bool {
        return std::abs(lhs - rhs) < e;
    }

    template<is_flt T>
    constexpr auto less_flt(T lhs, T rhs, T e = eps<T>) -> bool {
        return rhs - lhs > e;
    }

    template<is_flt T>
    constexpr auto greater_flt(T lhs, T rhs, T e = eps<T>) -> bool {
        return lhs - rhs > e;
    }

    template<is_flt T>
    constexpr auto less_eq_flt(T lhs, T rhs, T e = eps<T>) -> bool {
        return std::abs(lhs - rhs) < e || rhs - lhs > e;
    }

    template<is_flt T>
    constexpr auto greater_eq_flt(T lhs, T rhs, T e = eps<T>) -> bool {
        return std::abs(lhs - rhs) < e || lhs - rhs > e;
    }
}