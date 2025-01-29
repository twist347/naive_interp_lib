#pragma once

#include <cassert>

#include "i_1d_prev_alg.h"
#include "i_1d_next_alg.h"
#include "i_1d_nearest_neighbour_alg.h"
#include "i_1d_linear_alg.h"
#include "i_1d_quadratic_alg.h"
#include "i_1d_cubic_alg.h"
#include "i_1d_types.h"
#include "../utility/params.h"

#ifndef NDEBUG
    #include "i_1d_msg.h"
#endif

namespace interp {

    namespace detail {

        template<Type1D type, typename ... Args>
        auto interp_dispatch(Args &&... args) noexcept -> void {
            if constexpr (type == Type1D::Prev) {
                prev(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Next) {
                next(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::NearestNeighbour) {
                nearest_neighbour(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Linear) {
                linear(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Quadratic) {
                quadratic(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Cubic) {
                cubic(std::forward<Args>(args)...);
            } else {
                // todo: add msg
                static_assert(false);
            }
        }

        template<Type1D type>
        consteval static auto min_num_points_() noexcept -> std::size_t {
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
                // todo: add string msg
                static_assert(false);
            }
        }

#ifndef NDEBUG
        template<typename XpIter, typename Param>
        auto check_input_data(
            XpIter xp_first, XpIter xp_last,
            std::size_t min_num,
            const Param &p
        ) -> void {
            if (!std::is_sorted(xp_first, xp_last)) {
                throw std::invalid_argument(msg::must_be_sorted);
            }

            if (p.bounds_check && p.extrapolate) {
                throw std::invalid_argument(msg::incorrect_flags);
            }

            if (std::distance(xp_first, xp_last) < min_num) {
                throw std::invalid_argument(msg::min_nums + std::to_string(min_num));
            }
        }
#endif

    }

    // iterator interface

    template<
        Type1D type,
        typename XIter,
        typename XpIter, typename YpIter,
        typename DestIter
    >
    auto do_i(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<typename std::iterator_traits<DestIter>::value_type> &p = {}
    ) INTERP_RELEASE_NOEXCEPT -> void {

#ifndef NDEBUG
        detail::check_input_data(
            xp_first, xp_last,
            detail::min_num_points_<type>(),
            p
        );
#endif

        detail::interp_dispatch<type>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
    }

    // container interface

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
        const params_1d<typename std::remove_cvref_t<XContainer>::value_type> &p = {}
    ) INTERP_RELEASE_NOEXCEPT -> void {
        do_i<type>(
            std::cbegin(x), std::cend(x),
            std::cbegin(xp), std::cend(xp),
            std::cbegin(yp),
            std::begin(dest),
            p
        );
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
        const params_1d<typename std::remove_cvref_t<XContainer>::value_type> &p = {}
    ) -> DestContainer {
        DestContainer dest(std::size(x));
        do_i<type>(x, xp, yp, dest, p);

        return dest;
    }

}