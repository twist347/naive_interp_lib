#pragma once

#include "utility/utils.h"
#include "utility/exec.h"

namespace interp::detail {

    template<typename XpIter, typename YpIter, typename Value>
    constexpr auto calc_nearest_value_(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        Value xi
    ) noexcept -> Value {
        const auto idx = std::distance(xp_first, std::lower_bound(xp_first, xp_last, xi));
        if (utils::eq(xi, *(xp_first + idx))) {
            return *(yp_first + idx);
        }
        const auto xp_idx = xp_first + idx;
        const auto yp_idx = yp_first + idx;
        const auto prev_distance = std::abs(xi - *(xp_idx - 1));
        const auto next_distance = std::abs(*xp_idx - xi);
        return utils::less(prev_distance, next_distance) ? *(yp_idx - 1) : *yp_idx;
    }

    template<typename Value, typename XIter, typename XpIter, typename YpIter, typename DestIter>
    auto nn_pure_impl_(
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
            return calc_nearest_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename Value, typename XIter, typename XpIter, typename YpIter, typename DestIter>
    auto nn_bounds_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        const Value left_dflt_val = p.bounds.first;
        const Value right_dflt_val = p.bounds.second;
        const Value lx0 = *xp_first, rx0 = *(xp_last - 1);

        const auto calc = [&](Value xi) -> Value {
            if constexpr (std::numeric_limits<Value>::has_quiet_NaN) {
                if (std::isnan(xi)) {
                    return utils::nan<Value>;
                }
            }
            if (utils::less(xi, lx0)) {
                return left_dflt_val;
            }
            if (utils::greater(xi, rx0)) {
                return right_dflt_val;
            }
            return calc_nearest_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename Value, typename XIter, typename XpIter, typename YpIter, typename DestIter>
    auto nn_extr_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        const auto yp_size = std::distance(xp_first, xp_last);

        const Value lx0 = *xp_first, rx0 = *(xp_last - 1);
        const Value ly0 = *yp_first, ry0 = *(yp_first + yp_size - 1);

        const auto calc = [&](Value xi) -> Value {
            if constexpr (std::numeric_limits<Value>::has_quiet_NaN) {
                if (std::isnan(xi)) {
                    return utils::nan<Value>;
                }
            }
            if (utils::less(xi, lx0)) {
                return ly0;
            }
            if (utils::greater(xi, rx0)) {
                return ry0;
            }
            return calc_nearest_value_(xp_first, xp_last, yp_first, xi);
        };

        utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
    }

    template<typename Value, typename XIter, typename XpIter, typename YpIter, typename DestIter>
    auto nearest_neighbour_impl_(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p
    ) noexcept -> void {
        if (!p.bounds_check && !p.extrapolate) {
            nn_pure_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (p.bounds_check && !p.extrapolate) {
            nn_bounds_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            nn_extr_impl_<Value>(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        }
        // else nothing to do
    }

}
