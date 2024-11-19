#pragma once

namespace interp {

    namespace detail {

        template<typename XpIter, typename YpIter, typename Value>
        constexpr auto direct_nn_calc_(
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first, Value xi
        ) -> Value {
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

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter>
        auto nn_pure_impl(
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
                    return direct_nn_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            detail::apply_transform(x_first, x_last, dest_first, calc);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
        auto nn_bounds_check_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first,
            const Param &p
        ) -> void {
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
                    return direct_nn_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            detail::apply_transform(x_first, x_last, dest_first, calc);
        }

        template<typename XIter, typename XpIter, typename YpIter, typename DestIter>
        auto nn_extr_impl(
            XIter x_first, XIter x_last,
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            DestIter dest_first
        ) -> void {
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
                    return direct_nn_calc_(xp_first, xp_last, yp_first, xi);
                }
            };

            detail::apply_transform(x_first, x_last, dest_first, calc);
        }

    }

    template<typename XIter, typename XpIter, typename YpIter, typename DestIter, typename Param>
    auto nearest_neighbour(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const Param &p
    ) -> void {
        if (!p.bounds_check && !p.extrapolate) {
            detail::nn_pure_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first);
        } else if (p.bounds_check && !p.extrapolate) {
            detail::nn_bounds_check_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first, p);
        } else if (!p.bounds_check && p.extrapolate) {
            detail::nn_extr_impl(x_first, x_last, xp_first, xp_last, yp_first, dest_first);
        } else {
            throw std::invalid_argument(detail::exception_msg);
        }
    }

}