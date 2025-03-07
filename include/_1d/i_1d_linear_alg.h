#pragma once

#include "utility/utils.h"
#include "utility/exec.h"

namespace interp::detail {

    template<rai XpIter, rai YpIter, Numeric Value>
    constexpr auto calc_linear_value_(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        Value xi
    ) noexcept -> Value {
        const auto idx = std::distance(xp_first, std::lower_bound(xp_first, xp_last, xi));
        if (utils::eq(xi, xp_first[idx])) {
            return yp_first[idx];
        }
        const auto x0 = xp_first[idx - 1], x1 = xp_first[idx];
        const auto y0 = yp_first[idx - 1], y1 = yp_first[idx];

        return std::lerp(y0, y1, (xi - x0) / (x1 - x0));
    }

    template<Numeric Value, rai XIter, rai XpIter, rai YpIter, rai DestIter>
    auto linear_pure_impl_(
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
            return calc_linear_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<Numeric Value, rai XIter, rai XpIter, rai YpIter, rai DestIter>
    auto linear_bounds_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        const auto len = std::distance(xp_first, xp_last);
        const Value left_dflt_val = p.bounds.first;
        const Value right_dflt_val = p.bounds.second;
        const Value lx0 = xp_first[0], rx1 = xp_first[len - 1];

        const auto calc = [&](Value xi) -> Value {
            if constexpr (std::numeric_limits<Value>::has_quiet_NaN) {
                if (std::isnan(xi)) {
                    return utils::nan<Value>;
                }
            }
            if (utils::less(xi, lx0)) {
                return left_dflt_val;
            }
            if (utils::greater(xi, rx1)) {
                return right_dflt_val;
            }
            return calc_linear_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<Numeric Value, rai XIter, rai XpIter, rai YpIter, rai DestIter>
    auto linear_extr_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        const auto len = std::distance(xp_first, xp_last);
        const Value lx0 = xp_first[0], lx1 = xp_first[1];
        const Value ly0 = yp_first[0], ly1 = yp_first[1];
        const Value rx0 = xp_first[len - 2], rx1 = xp_first[len - 1];
        const Value ry0 = yp_first[len - 2], ry1 = yp_first[len - 1];
        const Value left_delta_x = lx1 - lx0;
        const Value right_delta_x = rx1 - rx0;

        const auto calc = [&](Value xi) -> Value {
            if constexpr (std::numeric_limits<Value>::has_quiet_NaN) {
                if (std::isnan(xi)) {
                    return utils::nan<Value>;
                }
            }
            if (utils::less(xi, lx0)) {
                return std::lerp(ly0, ly1, (xi - lx0) / left_delta_x);
            }
            if (utils::greater(xi, rx1)) {
                return std::lerp(ry0, ry1, (xi - rx0) / right_delta_x);
            }
            return calc_linear_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<Numeric Value, rai XIter, rai XpIter, rai YpIter, rai DestIter>
    auto linear_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        if (!p.bounds_check && !p.extrapolate) {
            linear_pure_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (p.bounds_check && !p.extrapolate) {
            linear_bounds_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            linear_extr_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        }
        // else nothing to do
    }

}
