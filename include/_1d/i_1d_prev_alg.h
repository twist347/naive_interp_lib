#pragma once

#include <algorithm>
#include <cmath>

#include "utils.h"
#include "i_1d_utils.h"
#include "../utility/exec.h"

namespace interp {

    namespace detail {

        template<typename XpIter, typename YpIter, typename Value>
        constexpr auto direct_prev_calc_(
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            Value xi
        ) noexcept -> Value {
            const auto idx = std::distance(xp_first, std::upper_bound(xp_first, xp_last, xi));
            return *(yp_first + idx - 1);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
        auto prev_pure_impl(
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
                    return direct_prev_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
        auto prev_bounds_check_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first,
            const Param &p
        ) noexcept -> void {
            using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter>>::value_type;

            const value_type left_dflt_val = p.bounds.first;
            const value_type right_dflt_val = p.bounds.second;
            const value_type lx0 = *xp_first, rx0 = *(xp_last - 1);

            const auto calc = [&](value_type xi) -> value_type {
                if (std::isnan(xi)) [[unlikely]] {
                    return utils::nan<value_type>;
                } else if (utils::less(xi, lx0)) [[unlikely]] {
                    return left_dflt_val;
                } else if (utils::greater(xi, rx0)) [[unlikely]] {
                    return right_dflt_val;
                } else [[likely]] {
                    return direct_prev_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
        auto prev_extr_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first,
            const Param &p
        ) noexcept -> void {
            using value_type = typename std::iterator_traits<std::remove_cvref_t<XIter>>::value_type;
            const auto yp_size = std::distance(xp_first, xp_last);

            const value_type lx0 = *xp_first, rx0 = *(xp_last - 1);
            const value_type ly0 = *yp_first, ry0 = *(yp_first + yp_size - 1);

            const auto calc = [&](value_type xi) -> value_type {
                if (std::isnan(xi)) [[unlikely]] {
                    return utils::nan<value_type>;
                } else if (utils::less(xi, lx0)) [[unlikely]] {
                    return ly0;
                } else if (utils::greater(xi, rx0)) [[unlikely]] {
                    return ry0;
                } else [[likely]] {
                    return direct_prev_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            utils::custom_transform(p.exec, x_first, x_last, dest_first, calc);
        }

    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto prev(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) -> void {
        if (!p.bounds_check && !p.extrapolate) {
            detail::prev_pure_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (p.bounds_check && !p.extrapolate) {
            detail::prev_bounds_check_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            detail::prev_extr_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else {
            throw std::invalid_argument(detail::exception_msg);
        }
    }

}