#pragma once

#include <iterator>

#include "i_1d_types.h"

namespace interp::detail {

    inline constexpr auto exception_msg = "logically incorrect flags combination: bounds_check && extrapolate";

    template<Type1D type>
    consteval static auto min_num_points_() -> std::size_t {
        if constexpr (
            type == Type1D::Prev ||
            type == Type1D::Next ||
            type == Type1D::NearestNeighbour
        ) {
            return 1;
        } else if constexpr (type == Type1D::Linear) {
            return 2;
        } else if constexpr (type == Type1D::Quadratic) {
            return 3;
        } else if constexpr (type == Type1D::Cubic) {
            return 4;
        } else {
            throw std::invalid_argument("unknown 1d interp type");
        }
    }

    template<typename XpIter, typename Param>
    auto check_input_data(
        XpIter xp_first, XpIter xp_last,
        std::size_t min_num,
        const Param &p = {}
    ) -> void {
        if (p.bounds_check && p.extrapolate) {
            throw std::invalid_argument("incompatible arguments: bounds_check == true && extrapolate == true");
        }

        if (std::distance(xp_first, xp_last) < min_num) {
            throw std::invalid_argument(
                "incompatible arguments: the number of points must be at least " + std::to_string(min_num)
            );
        }
    }

}
