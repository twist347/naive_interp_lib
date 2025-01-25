#pragma once

#include "../_1d/i_1d_prev_alg.h"
#include "../_1d/i_1d_next_alg.h"
#include "../_1d/i_1d_nearest_neighbour_alg.h"
#include "../_1d/i_1d_linear_alg.h"
#include "../_1d/i_1d_cubic_alg.h"
#include "../_1d/i_1d_types.h"
#include "../_1d/i_1d_utils.h"
#include "../utility/params.h"

namespace interp {

    template<
        Type1D type,
        typename XIter, typename XpIter,
        typename YpIter,
        typename DestIter
    >
    auto do_i(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first, YpIter yp_last,
        DestIter dest_first,
        params_1d<typename std::iterator_traits<DestIter>::value_type> p = {}
    ) -> void {
        detail::check_input_data(
            xp_first, xp_last,
            yp_first, yp_last,
            p.extrapolate, p.bounds_check, detail::min_num_points_<type>()
        );
        if constexpr (type == Type1D::Prev) {
            prev(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if constexpr (type == Type1D::Next) {
            next(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if constexpr (type == Type1D::NearestNeighbour) {
            nearest_neighbour(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if constexpr (type == Type1D::Linear) {
            linear(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if constexpr (type == Type1D::Quadratic) {
            quadratic(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if constexpr (type == Type1D::Cubic) {
            cubic(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else {
            throw std::invalid_argument("unsupported interp type");
        }
    }

    template<
        Type1D type,
        typename XContainer,
        typename XpContainer, typename YpContainer,
        typename DestContainer = std::remove_cvref_t<XContainer>
    >
    auto do_i(
        const XContainer &x,
        const XpContainer &xp,
        const YpContainer &yp,
        params_1d<typename std::remove_cvref_t<XContainer>::value_type> p = {}
    ) -> DestContainer {
        DestContainer dest(std::size(x));
        do_i<type>(
            std::cbegin(x), std::cend(x),
            std::cbegin(xp), std::cend(xp),
            std::cbegin(yp), std::cend(yp),
            std::begin(dest),
            p
        );

        return dest;
    }

    template<
        Type1D type,
        typename XContainer, typename XpContainer,
        typename YpContainer,
        typename DestContainer
    >
    auto do_i(
        const XContainer &x,
        const XpContainer &xp,
        const YpContainer &yp,
        DestContainer &dest,
        params_1d<typename std::remove_cvref_t<XContainer>::value_type> p = {}
    ) -> void {
        do_i<type>(
            std::cbegin(x), std::cend(x),
            std::cbegin(xp), std::cend(xp),
            std::cbegin(yp), std::cend(yp),
            std::begin(dest),
            p
        );
    }

}