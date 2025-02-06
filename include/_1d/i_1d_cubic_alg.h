#pragma once

#include <algorithm>
#include <cmath>

#include "utils.h"
#include "../utility/exec.h"

namespace interp::detail {

    template<typename Value>
    constexpr auto cubic_formula_(
        Value x0, Value y0,
        Value x1, Value y1,
        Value x2, Value y2,
        Value x3, Value y3,
        Value xi
    ) noexcept -> Value {
        return y0 * ((xi - x1) * (xi - x2) * (xi - x3)) / ((x0 - x1) * (x0 - x2) * (x0 - x3)) +
               y1 * ((xi - x0) * (xi - x2) * (xi - x3)) / ((x1 - x0) * (x1 - x2) * (x1 - x3)) +
               y2 * ((xi - x0) * (xi - x1) * (xi - x3)) / ((x2 - x0) * (x2 - x1) * (x2 - x3)) +
               y3 * ((xi - x0) * (xi - x1) * (xi - x2)) / ((x3 - x0) * (x3 - x1) * (x3 - x2));
    }

    template<typename XpIter, typename YpIter, typename Value>
    constexpr auto calc_cubic_value_(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        Value xi
    ) noexcept -> Value {
        auto idx = std::distance(xp_first, std::lower_bound(xp_first, xp_last, xi));
        if (utils::eq(xi, *(xp_first + idx))) {
            return *(yp_first + idx);
        }
        idx = std::max<decltype(idx)>(idx, 3); // shift idx for calculations below
        const auto xp_idx = xp_first + idx;
        const auto yp_idx = yp_first + idx;
        const auto x0 = *(xp_idx - 3), x1 = *(xp_idx - 2), x2 = *(xp_idx - 1), x3 = *xp_idx;
        const auto y0 = *(yp_idx - 3), y1 = *(yp_idx - 2), y2 = *(yp_idx - 1), y3 = *yp_idx;
        return cubic_formula_(x0, y0, x1, y1, x2, y2, x3, y3, xi);
    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto cubic_pure_impl_(
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
            return calc_cubic_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto cubic_bounds_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) noexcept -> void {
        using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter> >::value_type;

        const value_type left_dflt_val = p.bounds.first;
        const value_type right_dflt_val = p.bounds.second;
        const value_type lx0 = *xp_first;
        const value_type rx3 = *(xp_last - 1);

        const auto calc = [&](value_type xi) -> value_type {
            if (std::isnan(xi)) {
                return utils::nan<value_type>;
            }
            if (utils::less(xi, lx0)) {
                return left_dflt_val;
            }
            if (utils::greater(xi, rx3)) {
                return right_dflt_val;
            }
            return calc_cubic_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto cubic_extr_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) noexcept -> void {
        using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter> >::value_type;
        const auto yp_size = std::distance(xp_first, xp_last);

        const value_type lx0 = *xp_first, lx1 = *(xp_first + 1), lx2 = *(xp_first + 2), lx3 = *(xp_first + 3);
        const value_type ly0 = *yp_first, ly1 = *(yp_first + 1), ly2 = *(yp_first + 2), ly3 = *(yp_first + 3);
        const value_type rx0 = *(xp_last - 4), rx1 = *(xp_last - 3), rx2 = *(xp_last - 2), rx3 = *(xp_last - 1);
        const value_type ry0 = *(yp_first + yp_size - 4), ry1 = *(yp_first + yp_size - 3),
                         ry2 = *(yp_first + yp_size - 2), ry3 = *(yp_first + yp_size - 1);

        const auto calc = [&](value_type xi) -> value_type {
            if (std::isnan(xi)) {
                return utils::nan<value_type>;
            }
            if (utils::less(xi, lx0)) {
                return cubic_formula_(lx0, ly0, lx1, ly1, lx2, ly2, lx3, ly3, xi);
            }
            if (utils::greater(xi, rx3)) {
                return cubic_formula_(rx0, ry0, rx1, ry1, rx2, ry2, rx3, ry3, xi);
            }
            return calc_cubic_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }


    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto cubic_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) noexcept -> void {
        if (!p.bounds_check && !p.extrapolate) {
            cubic_pure_impl_(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (p.bounds_check && !p.extrapolate) {
            cubic_bounds_impl_(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            cubic_extr_impl_(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        }
        // else nothing to do
    }

}
