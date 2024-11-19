#pragma once

#include <cstddef>

namespace interp::detail {

    inline constexpr auto exception_msg = "logically incorrect flags combination: bounds_check && extrapolate";

    inline constexpr std::size_t THRESHOLD_VALUE = 5000;

    template<typename XIter, typename DestIter, typename UnaryOp>
    auto apply_transform(XIter x_first, XIter x_last, DestIter dest_first, UnaryOp op) -> void {
        const auto x_sz = std::distance(x_first, x_last);
        utils::custom_transform(x_sz, THRESHOLD_VALUE, x_first, x_last, dest_first, op);
    }

}

