#pragma once

#include <limits>
#include <algorithm>
#include <stdexcept>

#include "interp1d_gsl.h"
#include "utility/container_type_traits.h"
#include "utility/utils.h"

namespace ni::_1d::impl {

    template<class Container, class Iter>
    constexpr auto prev(Iter x_first, Iter x_last,
                        Iter xp_first, Iter xp_last,
                        Iter yp_first, Iter yp_last) -> Container {
        using size_type = ni::detail::container_size_type_t<Container>;
        using value_type = ni::detail::container_value_type_t<Container>;

        const size_type xp_sz = std::distance(xp_first, xp_last);
        const size_type yp_sz = std::distance(yp_first, yp_last);
        const size_type x_sz = std::distance(x_first, x_last);

        if (xp_sz < 1 || yp_sz < 1 || xp_sz != yp_sz) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        const auto xp_last_val = *(xp_first + xp_sz - 1);
        const auto yp_last_val = *(yp_first + yp_sz - 1);

        Container y(x_sz);

#pragma omp parallel for schedule(guided)
        for (size_type i = 0; i < x_sz; ++i) {
            const auto xi = *(x_first + i);
            if (std::isnan(xi)) {
                y[i] = utils::nan<value_type>;
                continue;
            }
            if (utils::eq_flt(xi, xp_last_val)) {
                y[i] = yp_last_val;
            }
            const auto idx = std::distance(xp_first, std::upper_bound(xp_first, xp_last, xi));
            y[i] = *(yp_first + idx - 1);
        }
        return y;
    }

    template<class Container, class Iter>
    constexpr auto next(Iter x_first, Iter x_last,
                        Iter xp_first, Iter xp_last,
                        Iter yp_first, Iter yp_last) -> Container {
        using size_type = ni::detail::container_size_type_t<Container>;
        using value_type = ni::detail::container_value_type_t<Container>;

        const size_type xp_sz = std::distance(xp_first, xp_last);
        const size_type yp_sz = std::distance(yp_first, yp_last);
        const size_type x_sz = std::distance(x_first, x_last);

        if (xp_sz < 1 || yp_sz < 1 || xp_sz != yp_sz) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        const auto xp_last_val = *(xp_first + xp_sz - 1);
        const auto yp_last_val = *(yp_first + yp_sz - 1);

        Container y(x_sz);

#pragma omp parallel for schedule(static)
        for (size_type i = 0; i < x_sz; ++i) {
            const auto xi = *(x_first + i);
            if (std::isnan(xi)) {
                y[i] = utils::nan<value_type>;
                continue;
            }
            if (utils::eq_flt(xi, xp_last_val)) {
                y[i] = yp_last_val;
                continue;
            }
            const auto idx = std::distance(xp_first, std::lower_bound(xp_first, xp_last, xi));
            y[i] = *(yp_first + idx);
        }
        return y;
    }

    template<class Container, class Iter>
    constexpr auto nearest_neighbour(Iter x_first, Iter x_last,
                                     Iter xp_first, Iter xp_last,
                                     Iter yp_first, Iter yp_last) -> Container {
        using size_type = ni::detail::container_size_type_t<Container>;
        using value_type = ni::detail::container_value_type_t<Container>;

        const size_type xp_sz = std::distance(xp_first, xp_last);
        const size_type yp_sz = std::distance(yp_first, yp_last);
        const size_type x_sz = std::distance(x_first, x_last);

        if (xp_sz < 1 || yp_sz < 1 || xp_sz != yp_sz) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        const auto xp_last_val = *(xp_first + xp_sz - 1);
        const auto yp_last_val = *(yp_first + yp_sz - 1);

        Container y(x_sz);

#pragma omp parallel for schedule(guided)
        for (size_type i = 0; i < x_sz; ++i) {
            const auto xi = *(x_first + i);
            if (std::isnan(xi)) {
                y[i] = utils::nan<value_type>;
                continue;
            }
            if (utils::eq_flt(xi, xp_last_val)) {
                y[i] = yp_last_val;
                continue;
            }
            const auto idx = std::distance(xp_first, std::upper_bound(xp_first, xp_last, xi));
            const value_type prev_distance = std::abs(xi - *(xp_first + idx - 1));
            const value_type next_distance = std::abs(*(xp_first + idx) - xi);
            y[i] = utils::less_flt(prev_distance, next_distance) ? *(yp_first + idx - 1) : *(yp_first + idx);
        }
        return y;
    }

    template<class Container, class Iter>
    constexpr auto linear(Iter x_first, Iter x_last,
                          Iter xp_first, Iter xp_last,
                          Iter yp_first, Iter yp_last) -> Container {
        using size_type = ni::detail::container_size_type_t<Container>;
        using value_type = ni::detail::container_value_type_t<Container>;

        const size_type xp_sz = std::distance(xp_first, xp_last);
        const size_type yp_sz = std::distance(yp_first, yp_last);
        const size_type x_sz = std::distance(x_first, x_last);

        if (xp_sz < 1 || yp_sz < 1 || xp_sz != yp_sz) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        const auto xp_last_val = *(xp_first + xp_sz - 1);
        const auto yp_last_val = *(yp_first + yp_sz - 1);

        Container y(x_sz);

#pragma omp parallel for schedule(guided)
        for (size_type i = 0; i < x_sz; ++i) {
            const auto xi = *(x_first + i);
            if (std::isnan(xi)) {
                y[i] = utils::nan<value_type>;
                continue;
            }
            if (utils::eq_flt(xi, xp_last_val)) {
                y[i] = yp_last_val;
                continue;
            }
            const auto idx = std::distance(xp_first, std::upper_bound(xp_first, xp_last, xi));
            const value_type x0 = *(xp_first + idx - 1), x1 = *(xp_first + idx);
            const value_type y0 = *(yp_first + idx - 1), y1 = *(yp_first + idx);
            y[i] = y0 + (y1 - y0) / (x1 - x0) * (xi - x0);
        }
        return y;
    }

    template<class Container, class Iter>
    constexpr auto quadratic(Iter x_first, Iter x_last,
                             Iter xp_first, Iter xp_last,
                             Iter yp_first, Iter yp_last) -> Container {
        using size_type = ni::detail::container_size_type_t<Container>;
        using value_type = ni::detail::container_value_type_t<Container>;

        const size_type xp_sz = std::distance(xp_first, xp_last);
        const size_type yp_sz = std::distance(yp_first, yp_last);
        const size_type x_sz = std::distance(x_first, x_last);

        if (xp_sz < 1 || yp_sz < 1 || xp_sz != yp_sz) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        constexpr auto calc_ = [](value_type x0, value_type y0, value_type x1, value_type y1, value_type x2,
                                  value_type y2, value_type xi) {
            const value_type a = (xi - x1) * (xi - x2) / ((x0 - x1) * (x0 - x2)) * y0;
            const value_type b = (xi - x0) * (xi - x2) / ((x1 - x0) * (x1 - x2)) * y1;
            const value_type c = (xi - x0) * (xi - x1) / ((x2 - x0) * (x2 - x1)) * y2;
            return a + b + c;
        };

        const auto xp_last_val = *(xp_first + xp_sz - 1);
        const auto yp_last_val = *(yp_first + yp_sz - 1);

        Container y(x_sz);

#pragma omp parallel for schedule(guided)
        for (size_type i = 0; i < x_sz; ++i) {
            const auto xi = *(x_first + i);
            if (std::isnan(xi)) {
                y[i] = utils::nan<value_type>;
                continue;
            }
            if (utils::eq_flt(xi, xp_last_val)) {
                y[i] = yp_last_val;
                continue;
            }
            auto idx = std::distance(xp_first, std::upper_bound(xp_first, xp_last, xi));
            idx = (idx == 1) ? idx + 1 : idx;
            const value_type x0 = *(xp_first + idx - 2), x1 = *(xp_first + idx - 1), x2 = *(xp_first + idx);
            const value_type y0 = *(yp_first + idx - 2), y1 = *(yp_first + idx - 1), y2 = *(yp_first + idx);
            y[i] = calc_(x0, y0, x1, y1, x2, y2, xi);
        }
        return y;
    }

    template<class Container, class Iter>
    constexpr auto cubic(Iter x_first, Iter x_last,
                         Iter xp_first, Iter xp_last,
                         Iter yp_first, Iter yp_last) -> Container {
        using size_type = ni::detail::container_size_type_t<Container>;
        using value_type = ni::detail::container_value_type_t<Container>;

        const size_type xp_sz = std::distance(xp_first, xp_last);
        const size_type yp_sz = std::distance(yp_first, yp_last);
        const size_type x_sz = std::distance(x_first, x_last);

        if (xp_sz < 1 || yp_sz < 1 || xp_sz != yp_sz) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        constexpr auto calc_ = [](value_type x0, value_type y0, value_type x1, value_type y1, value_type x2,
                                  value_type y2, value_type x3, value_type y3, value_type xi) {
            const value_type a = (xi - x1) * (xi - x2) * (xi - x3) / ((x0 - x1) * (x0 - x2) * (x0 - x3)) * y0;
            const value_type b = (xi - x0) * (xi - x2) * (xi - x3) / ((x1 - x0) * (x1 - x2) * (x1 - x3)) * y1;
            const value_type c = (xi - x0) * (xi - x1) * (xi - x3) / ((x2 - x0) * (x2 - x1) * (x2 - x3)) * y2;
            const value_type d = (xi - x0) * (xi - x1) * (xi - x2) / ((x3 - x0) * (x3 - x1) * (x3 - x2)) * y3;
            return a + b + c + d;
        };

        const auto xp_last_val = *(xp_first + xp_sz - 1);
        const auto yp_last_val = *(yp_first + yp_sz - 1);

        Container y(x_sz);

#pragma omp parallel for schedule(guided)
        for (size_type i = 0; i < x_sz; ++i) {
            const auto xi = *(x_first + i);
            if (std::isnan(xi)) {
                y[i] = utils::nan<value_type>;
                continue;
            }
            if (utils::eq_flt(xi, xp_last_val)) {
                y[i] = yp_last_val;
                continue;
            }
            auto idx = std::distance(xp_first, std::upper_bound(xp_first, xp_last, xi));
            idx = (idx == 1) ? idx + 2 : idx;
            idx = (idx == 2) ? idx + 1 : idx;
            const value_type x0 = *(xp_first + idx - 3), x1 = *(xp_first + idx - 2), x2 = *(xp_first + idx - 1),
                    x3 = *(xp_first + idx);
            const value_type y0 = *(yp_first + idx - 3), y1 = *(yp_first + idx - 2), y2 = *(yp_first + idx - 1),
                    y3 = *(yp_first + idx);
            y[i] = calc_(x0, y0, x1, y1, x2, y2, x3, y3, xi);
        }
        return y;
    }

    template<class Container>
    constexpr auto cubic_spline(const Container &x, const Container &xp, const Container &yp) -> Container {
        constexpr bool is_cached = false;
        return ni::_1d::impl::i_gsl<ni::_1d::impl::TypeGSL::CubicSpline, Container, is_cached>(xp, yp)(x);
    }

    template<class Container>
    constexpr auto akima(const Container &x, const Container &xp, const Container &yp) -> Container {
        constexpr bool is_cached = false;
        return ni::_1d::impl::i_gsl<ni::_1d::impl::TypeGSL::Akima, Container, is_cached>(xp, yp)(x);
    }

    template<class Container>
    constexpr auto steffen(const Container &x, const Container &xp, const Container &yp) -> Container {
        constexpr bool is_cached = false;
        return ni::_1d::impl::i_gsl<ni::_1d::impl::TypeGSL::Steffen, Container, is_cached>(xp, yp)(x);
    }

}
