#pragma once

#include <cassert>
#include <iterator>

#include "i_1d_prev_alg.h"
#include "i_1d_next_alg.h"
#include "i_1d_nearest_neighbour_alg.h"
#include "i_1d_linear_alg.h"
#include "i_1d_quadratic_alg.h"
#include "i_1d_cubic_alg.h"
#include "i_1d_types.h"
#include "utility/params.h"
#include "utility/utils.h"

#ifndef NDEBUG
    #include "i_1d_msg.h"
#endif

#ifdef NDEBUG
    #define VALIDATE_PARAMS(xp_first, xp_last, p)
#else
    #define VALIDATE_PARAMS(xp_first, xp_last, p)  \
        assert(std::is_sorted((xp_first), (xp_last)));  \
        assert(!((p).bounds_check && (p).extrapolate)); \
        assert(std::distance((xp_first), (xp_last)) >= detail::min_num_points_<type>());
#endif

namespace interp {

    namespace detail {

        template<Type1D type, Numeric Value, typename... Args>
        auto interp_dispatch(Args &&... args) noexcept -> void {
            if constexpr (type == Type1D::Prev) {
                detail::prev_impl_<Value>(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Next) {
                detail::next_impl_<Value>(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::NearestNeighbour) {
                detail::nearest_neighbour_impl_<Value>(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Linear) {
                detail::linear_impl_<Value>(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Quadratic) {
                detail::quadratic_impl_<Value>(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Cubic) {
                detail::cubic_impl_<Value>(std::forward<Args>(args)...);
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

    }

    // iterator interface

    template<
        Type1D type,
        std::random_access_iterator XIter,
        std::random_access_iterator XpIter,
        std::random_access_iterator YpIter,
        std::random_access_iterator DestIter,
        Numeric Value = utils::common_iter_val_t<XIter, XpIter, YpIter, DestIter>
    >
    auto do_i(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p = {}
    ) noexcept -> void {
        VALIDATE_PARAMS(xp_first, xp_last, p);

        detail::interp_dispatch<type, Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
    }

    // container interface

    template<
        Type1D type,
        RandomAccessContainer XContainer,
        RandomAccessContainer XpContainer,
        RandomAccessContainer YpContainer,
        RandomAccessContainer DestContainer,
        Numeric Value = utils::common_cont_val_t<XContainer, XpContainer, YpContainer, DestContainer>
    >
    auto do_i(
        const XContainer &x,
        const XpContainer &xp,
        const YpContainer &yp,
        DestContainer &dest,
        const params_1d<Value> &p = {}
    ) noexcept -> void {
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
        RandomAccessContainer XContainer,
        RandomAccessContainer XpContainer,
        RandomAccessContainer YpContainer,
        RandomAccessContainer DestContainer = std::remove_cvref_t<XContainer>,
        Numeric Value = utils::common_cont_val_t<XContainer, XpContainer, YpContainer, DestContainer>
    >
    auto do_i(
        const XContainer &x,
        const XpContainer &xp,
        const YpContainer &yp,
        const params_1d<Value> &p = {}
    ) -> DestContainer {
        DestContainer dest(std::size(x));
        do_i<type>(x, xp, yp, dest, p);

        return dest;
    }

}
