#pragma once

#include <iostream>

#define assert_msg(expr, msg) \
    ((static_cast<bool>(expr)) ? \
        static_cast<void>(0) : \
        (std::cerr << "assertion failed: " << msg << "\n", assert(false)))

namespace interp::detail::msg {

    inline constexpr auto incorrect_flags = "logically incorrect flags combination: bounds_check && extrapolate";

    inline constexpr auto unknown_interp = "unsupported interp type";

    inline constexpr auto must_be_sorted = "the range defined by xp_first and xp_last must be sorted in ascending order";

    inline constexpr auto min_nums = "the number of points must be at least ";

}