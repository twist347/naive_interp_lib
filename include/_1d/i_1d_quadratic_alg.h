#pragma once

#include <algorithm>
#include <cmath>
#include "utils.h"
#include "exec.h"
#include "i_1d_alg.h"

namespace interp {

    namespace detail {

        template<typename Value>
        constexpr auto quadratic_calc_(
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
        constexpr auto direct_quad_calc_(XpIter xp_first, XpIter xp_last, YpIter yp_first, Value xi) -> Value {
            auto idx = std::distance(xp_first, std::upper_bound(xp_first, xp_last, xi)) - 1;
            if (utils::eq(xi, *(xp_first + idx))) {
                return *(yp_first + idx);
            }
            idx = idx < 3 ? 2 : idx; // shift idx for calculations below
            const auto xp_idx = xp_first + idx;
            const auto yp_idx = yp_first + idx;
            const auto x0 = *(xp_idx - 2), x1 = *(xp_idx - 1), x2 = *xp_idx;
            const auto y0 = *(yp_idx - 2), y1 = *(yp_idx - 1), y2 = *yp_idx;
            return quadratic_calc_(x0, y0, x1, y1, x2, y2, xi);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter>
        auto quadratic_pure_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first
        ) -> void {
            using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter>>::value_type;

            const auto calc = [&](value_type xi) -> value_type {
                if (std::isnan(xi)) [[unlikely]] {
                    return utils::nan<value_type>;
                } else [[likely]] {
                    return direct_quad_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            const auto x_sz = std::distance(x_first, x_last);
            utils::custom_transform(x_sz, THRESHOLD_VALUE, x_first, x_last, dest_first, calc);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
        auto quadratic_bounds_check_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first,
            const Param &p
        ) -> void {
            using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter>>::value_type;

            const value_type left_dflt_val = p.bounds.first;
            const value_type right_dflt_val = p.bounds.second;
            const value_type lx0 = *xp_first;
            const value_type rx2 = *(xp_last - 1);

            const auto calc = [&](value_type xi) -> value_type {
                if (std::isnan(xi)) [[unlikely]] {
                    return utils::nan<value_type>;
                } else if (utils::less(xi, lx0)) [[unlikely]] {
                    return left_dflt_val;
                } else if (utils::greater(xi, rx2)) [[unlikely]] {
                    return right_dflt_val;
                } else [[likely]] {
                    return direct_quad_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            const auto x_sz = std::distance(x_first, x_last);
            utils::custom_transform(x_sz, THRESHOLD_VALUE, x_first, x_last, dest_first, calc);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter>
        auto quadratic_extr_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first
        ) -> void {
            using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter>>::value_type;
            const auto yp_size = std::distance(xp_first, xp_last);

            const value_type lx0 = *xp_first, lx1 = *(xp_first + 1), lx2 = *(xp_first + 2);
            const value_type ly0 = *yp_first, ly1 = *(yp_first + 1), ly2 = *(yp_first + 2);
            const value_type rx0 = *(xp_last - 3), rx1 = *(xp_last - 2), rx2 = *(xp_last - 1);
            const value_type ry0 = *(yp_first + yp_size - 3),
                ry1 = *(yp_first + yp_size - 2), ry2 = *(yp_first + yp_size - 1);

            const auto calc = [&](value_type xi) -> value_type {
                if (std::isnan(xi)) [[unlikely]] {
                    return utils::nan<value_type>;
                } else if (utils::less(xi, lx0)) [[unlikely]] {
                    return quadratic_calc_(lx0, ly0, lx1, ly1, lx2, ly2, xi);
                } else if (utils::greater(xi, rx2)) [[unlikely]] {
                    return quadratic_calc_(rx0, ry0, rx1, ry1, rx2, ry2, xi);
                } else [[likely]] {
                    return direct_quad_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            const auto x_sz = std::distance(x_first, x_last);
            utils::custom_transform(x_sz, THRESHOLD_VALUE, x_first, x_last, dest_first, calc);
        }

    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto quadratic(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) -> void {
        if (!p.bounds_check && !p.extrapolate) {
            detail::quadratic_pure_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first);
        } else if (p.bounds_check && !p.extrapolate) {
            detail::quadratic_bounds_check_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            detail::quadratic_extr_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first);
        } else {
            throw std::invalid_argument(detail::exception_msg);
        }
    }

}
