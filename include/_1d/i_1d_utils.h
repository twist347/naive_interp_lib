#pragma once

#include <iterator>

#include "i_1d_types.h"

namespace interp::detail {

    template<Type1D type>
    constexpr static auto min_num_points_() -> std::size_t {
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

    template<typename XpIter, typename YpIter>
    auto check_input_data(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first, YpIter yp_last,
        bool extrapolate, bool bounds_check,
        std::size_t min_num
    ) -> void {
        if (std::distance(xp_first, xp_last) != std::distance(yp_first, yp_last)) {
            throw std::invalid_argument("size of xp must be equal to yp size");
        }

        if (bounds_check && extrapolate) {
            throw std::invalid_argument("incompatible arguments: bounds_check == true && extrapolate == true");
        }

        if (std::distance(xp_first, xp_last) < min_num) {
            throw std::invalid_argument("the number of points must be at least " + std::to_string(min_num));
        }
    }

}
