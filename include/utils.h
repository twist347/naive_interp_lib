#pragma once

#include <cmath>

namespace ni::utils {
    template<class T, class U>
    constexpr auto to(U u) -> T {
        return static_cast<T>(u);
    }

    template<class T>
    auto eps = to<T>(1e-10);

    template<class T>
    constexpr auto eq_flt(const T &lhs, const T &rhs) -> bool {
        return std::abs(lhs - rhs) < eps<T>;
    }

    template<class T>
    constexpr auto less_flt(const T &lhs, const T &rhs) -> bool {
        return rhs - lhs > eps<T>;
    }

    template<class T>
    constexpr auto greater_flt(const T &lhs, const T &rhs) -> bool {
        return lhs - rhs > eps<T>;
    }

    template<class T>
    constexpr auto less_eq_flt(const T &lhs, const T &rhs) -> bool {
        return std::abs(lhs - rhs) < eps<T> || rhs - lhs > eps<T>;
    }

    template<class T>
    constexpr auto greater_eq_flt(const T &lhs, const T &rhs) -> bool {
        return std::abs(lhs - rhs) < eps<T> || lhs - rhs > eps<T>;
    }
}