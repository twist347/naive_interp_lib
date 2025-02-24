#pragma once

#include "utility/utils.h"
#include "utility/exec.h"

namespace interp::detail {

    template<typename Value>
    constexpr auto quadratic_formula_(
        Value x0, Value y0,
        Value x1, Value y1,
        Value x2, Value y2,
        Value xi
    ) noexcept -> Value {
        return y0 * (xi - x1) * (xi - x2) / ((x0 - x1) * (x0 - x2)) +
               y1 * (xi - x0) * (xi - x2) / ((x1 - x0) * (x1 - x2)) +
               y2 * (xi - x0) * (xi - x1) / ((x2 - x0) * (x2 - x1));
    }

    template<typename XpIter, typename YpIter, typename Value>
    constexpr auto calc_quad_value_(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        Value xi
    ) noexcept -> Value {
        auto idx = std::distance(xp_first, std::lower_bound(xp_first, xp_last, xi));
        if (utils::eq(xi, *(xp_first + idx))) {
            return *(yp_first + idx);
        }
        idx = std::max<decltype(idx)>(idx, 2); // shift idx for calculations below
        const auto xp_idx = xp_first + idx;
        const auto yp_idx = yp_first + idx;
        const auto x0 = *(xp_idx - 2), x1 = *(xp_idx - 1), x2 = *xp_idx;
        const auto y0 = *(yp_idx - 2), y1 = *(yp_idx - 1), y2 = *yp_idx;
        return quadratic_formula_(x0, y0, x1, y1, x2, y2, xi);
    }

    template<typename Value, typename XIter, typename XpIter, typename YpIter, typename DestIter>
    auto quadratic_pure_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        const auto calc = [&](Value xi) -> Value {
            if constexpr (std::numeric_limits<Value>::has_quiet_NaN) {
                if (std::isnan(xi)) {
                    return utils::nan<Value>;
                }
            }
            return calc_quad_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename Value, typename XIter, typename XpIter, typename YpIter, typename DestIter>
    auto quadratic_bounds_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        const Value left_dflt_val = p.bounds.first;
        const Value right_dflt_val = p.bounds.second;
        const Value lx0 = *xp_first;
        const Value rx2 = *(xp_last - 1);

        const auto calc = [&](Value xi) -> Value {
            if constexpr (std::numeric_limits<Value>::has_quiet_NaN) {
                if (std::isnan(xi)) {
                    return utils::nan<Value>;
                }
            }
            if (utils::less(xi, lx0)) {
                return left_dflt_val;
            }
            if (utils::greater(xi, rx2)) {
                return right_dflt_val;
            }
            return calc_quad_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename Value, typename XIter, typename XpIter, typename YpIter, typename DestIter>
    auto quadratic_extr_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        const auto yp_size = std::distance(xp_first, xp_last);

        const Value lx0 = *xp_first, lx1 = *(xp_first + 1), lx2 = *(xp_first + 2);
        const Value ly0 = *yp_first, ly1 = *(yp_first + 1), ly2 = *(yp_first + 2);
        const Value rx0 = *(xp_last - 3), rx1 = *(xp_last - 2), rx2 = *(xp_last - 1);
        const Value ry0 = *(yp_first + yp_size - 3),
                ry1 = *(yp_first + yp_size - 2), ry2 = *(yp_first + yp_size - 1);

        const auto calc = [&](Value xi) -> Value {
            if constexpr (std::numeric_limits<Value>::has_quiet_NaN) {
                if (std::isnan(xi)) {
                    return utils::nan<Value>;
                }
            }
            if (utils::less(xi, lx0)) {
                return quadratic_formula_(lx0, ly0, lx1, ly1, lx2, ly2, xi);
            }
            if (utils::greater(xi, rx2)) {
                return quadratic_formula_(rx0, ry0, rx1, ry1, rx2, ry2, xi);
            }
            return calc_quad_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename Value, typename XIter, typename XpIter, typename YpIter, typename DestIter>
    auto quadratic_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        if (!p.bounds_check && !p.extrapolate) {
            quadratic_pure_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (p.bounds_check && !p.extrapolate) {
            quadratic_bounds_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            quadratic_extr_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        }
        // else nothing to do
    }

}
