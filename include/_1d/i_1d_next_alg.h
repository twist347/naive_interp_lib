#pragma once

#include <algorithm>
#include <cmath>

#include "utils.h"
#include "../utility/exec.h"

namespace interp::detail {

    template<typename XpIter, typename YpIter, typename Value>
    constexpr auto calc_next_value_(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        Value xi
    ) noexcept -> Value {
        const auto idx = std::distance(xp_first, std::lower_bound(xp_first, xp_last, xi));
        return *(yp_first + idx);
    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto next_pure_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) noexcept -> void {
        using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter> >::value_type;

        const auto calc = [&](value_type xi) -> value_type {
            if (std::isnan(xi)) {
                return utils::nan<value_type>;
            }
            return calc_next_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto next_bounds_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) noexcept -> void {
        using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter> >::value_type;

        const value_type left_dflt_val = p.bounds.first;
        const value_type right_dflt_val = p.bounds.second;
        const value_type lx0 = *xp_first, rx0 = *(xp_last - 1);

        const auto calc = [&](value_type xi) -> value_type {
            if (std::isnan(xi)) {
                return utils::nan<value_type>;
            }
            if (utils::less(xi, lx0)) {
                return left_dflt_val;
            }
            if (utils::greater(xi, rx0)) {
                return right_dflt_val;
            }
            return calc_next_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto next_extr_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) noexcept -> void {
        using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter> >::value_type;
        const auto yp_size = std::distance(xp_first, xp_last);

        const value_type lx0 = *xp_first, rx0 = *(xp_last - 1);
        const value_type ly0 = *yp_first, ry0 = *(yp_first + yp_size - 1);

        const auto calc = [&](value_type xi) -> value_type {
            if (std::isnan(xi)) {
                return utils::nan<value_type>;
            }
            if (utils::less(xi, lx0)) {
                return ly0;
            }
            if (utils::greater(xi, rx0)) {
                return ry0;
            }
            return calc_next_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto next_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) noexcept -> void {
        if (!p.bounds_check && !p.extrapolate) {
            next_pure_impl_(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (p.bounds_check && !p.extrapolate) {
            next_bounds_impl_(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            next_extr_impl_(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        }
        // else nothing to do
    }

}
