#pragma once

#include <algorithm>
#include <cmath>

#include "utils.h"
#include "i_1d_utils.h"
#include "../utility/exec.h"

namespace interp {

    namespace detail {

        template<typename XpIter, typename YpIter, typename Value>
        constexpr auto direct_linear_calc_(
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
            const auto x0 = *(xp_idx - 1), x1 = *xp_idx;
            const auto y0 = *(yp_idx - 1), y1 = *yp_idx;
            return std::lerp(y0, y1, (xi - x0) / (x1 - x0));
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
        auto linear_pure_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first,
            const Param &p
        ) noexcept -> void {
            using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter>>::value_type;

            const auto calc = [&](value_type xi) -> value_type {
                if (std::isnan(xi)) [[unlikely]] {
                    return utils::nan<value_type>;
                } else [[likely]] {
                    return direct_linear_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
        auto linear_bounds_check_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first,
            const Param &p
        ) noexcept -> void {
            using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter>>::value_type;

            const value_type left_dflt_val = p.bounds.first;
            const value_type right_dflt_val = p.bounds.second;
            const value_type lx0 = *xp_first, rx1 = *(xp_last - 1);

            const auto calc = [&](value_type xi) -> value_type {
                if (std::isnan(xi)) [[unlikely]] {
                    return utils::nan<value_type>;
                } else if (utils::less(xi, lx0)) [[unlikely]] {
                    return left_dflt_val;
                } else if (utils::greater(xi, rx1)) [[unlikely]] {
                    return right_dflt_val;
                } else [[likely]] {
                    return direct_linear_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
        auto linear_extr_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first,
            const Param &p
        ) noexcept -> void {
            using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter>>::value_type;
            const auto yp_size = std::distance(xp_first, xp_last);

            const value_type lx0 = *xp_first, lx1 = *(xp_first + 1);
            const value_type ly0 = *yp_first, ly1 = *(yp_first + 1);
            const value_type rx0 = *(xp_last - 2), rx1 = *(xp_last - 1);
            const value_type ry0 = *(yp_first + yp_size - 2), ry1 = *(yp_first + yp_size - 1);
            const value_type left_delta_x = lx1 - lx0;
            const value_type right_delta_x = rx1 - rx0;

            const auto calc = [&](value_type xi) -> value_type {
                if (std::isnan(xi)) [[unlikely]] {
                    return utils::nan<value_type>;
                } else if (utils::less(xi, lx0)) [[unlikely]] {
                    return std::lerp(ly0, ly1, (xi - lx0) / left_delta_x);
                } else if (utils::greater(xi, rx1)) [[unlikely]] {
                    return std::lerp(ry0, ry1, (xi - rx0) / right_delta_x);
                } else [[likely]] {
                    return direct_linear_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
        }

    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Params>
    auto linear(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Params &p
    ) -> void {
        if (!p.bounds_check && !p.extrapolate) {
            detail::linear_pure_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (p.bounds_check && !p.extrapolate) {
            detail::linear_bounds_check_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            detail::linear_extr_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else {
            throw std::invalid_argument(detail::exception_msg);
        }
    }

}