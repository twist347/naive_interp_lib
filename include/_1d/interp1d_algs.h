#pragma once

#include <limits>
#include <algorithm>
#include <stdexcept>
#include <utils.h>
#include <interp1d_gsl.h>
#include <container_type_traits.h>

namespace ni::_1d::detail {

    template<class Container>
    constexpr auto prev(const Container &x, const Container &xp, const Container &yp) -> Container {
        using idx_t = ni::detail::container_size_type_t<Container>;
        using value_t = ni::detail::container_value_type_t<Container>;

        if (xp.size() < 1 || yp.size() < 1 || xp.size() != yp.size()) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        Container y(x.size());

#pragma omp parallel for schedule(guided)
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<value_t>::quiet_NaN();
                continue;
            }
            if (utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            y[i] = yp[idx - 1];
        }
        return y;
    }

    template<class Container>
    constexpr auto next(const Container &x, const Container &xp, const Container &yp) -> Container {
        using idx_t = ni::detail::container_size_type_t<Container>;
        using value_t = ni::detail::container_value_type_t<Container>;

        if (xp.size() < 1 || yp.size() < 1 || xp.size() != yp.size()) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        Container y(x.size());

#pragma omp parallel for schedule(guided)
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<value_t>::quiet_NaN();
                continue;
            }
            if (utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::lower_bound(xp.begin(), xp.end(), x[i]));
            y[i] = yp[idx];
        }
        return y;
    }

    template<class Container>
    constexpr auto nearest_neighbour(const Container &x, const Container &xp, const Container &yp) -> Container {
        using idx_t = ni::detail::container_size_type_t<Container>;
        using value_t = ni::detail::container_value_type_t<Container>;

        if (xp.size() < 1 || yp.size() < 1 || xp.size() != yp.size()) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        Container y(x.size());

#pragma omp parallel for schedule(guided)
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<value_t>::quiet_NaN();
                continue;
            }
            if (utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            const value_t prev_distance = std::abs(x[i] - xp[idx - 1]);
            const value_t next_distance = std::abs(xp[idx] - x[i]);
            y[i] = utils::less_flt(prev_distance, next_distance) ? yp[idx - 1] : yp[idx];
        }
        return y;
    }

    template<class Container>
    constexpr auto linear(const Container &x, const Container &xp, const Container &yp) -> Container {
        using idx_t = ni::detail::container_size_type_t<Container>;
        using value_t = ni::detail::container_value_type_t<Container>;

        if (xp.size() < 2 || yp.size() < 2 || xp.size() != yp.size()) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        Container y(x.size());

#pragma omp parallel for schedule(guided)
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<value_t>::quiet_NaN();
                continue;
            }
            if (utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            const value_t x0 = xp[idx - 1], x1 = xp[idx];
            const value_t y0 = yp[idx - 1], y1 = yp[idx];
            y[i] = y0 + (y1 - y0) / (x1 - x0) * (x[i] - x0);
        }
        return y;
    }

    template<class Container>
    constexpr auto quadratic(const Container &x, const Container &xp, const Container &yp) -> Container {
        using idx_t = ni::detail::container_size_type_t<Container>;
        using value_t = ni::detail::container_value_type_t<Container>;

        if (xp.size() < 3 || yp.size() < 3 || xp.size() != yp.size()) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        auto calc_ = [](value_t x0, value_t y0, value_t x1, value_t y1, value_t x2, value_t y2, value_t xi) {
            const value_t a = (xi - x1) * (xi - x2) / ((x0 - x1) * (x0 - x2)) * y0;
            const value_t b = (xi - x0) * (xi - x2) / ((x1 - x0) * (x1 - x2)) * y1;
            const value_t c = (xi - x0) * (xi - x1) / ((x2 - x0) * (x2 - x1)) * y2;
            return a + b + c;
        };

        Container y(x.size());

#pragma omp parallel for schedule(guided)
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<value_t>::quiet_NaN();
                continue;
            }
            if (utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            idx = (idx == 1) ? idx + 1 : idx;
            const value_t x0 = xp[idx - 2], x1 = xp[idx - 1], x2 = xp[idx];
            const value_t y0 = yp[idx - 2], y1 = yp[idx - 1], y2 = yp[idx];
            y[i] = calc_(x0, y0, x1, y1, x2, y2, x[i]);
        }
        return y;
    }

    template<class Container>
    constexpr auto cubic(const Container &x, const Container &xp, const Container &yp) -> Container {
        using idx_t = ni::detail::container_size_type_t<Container>;
        using value_t = ni::detail::container_value_type_t<Container>;

        if (xp.size() < 4 || yp.size() < 4 || xp.size() != yp.size()) {
            throw std::invalid_argument("the input data does not meet the conditions");
        }

        auto calc_ = [](value_t x0, value_t y0, value_t x1, value_t y1, value_t x2, value_t y2, value_t x3, value_t y3,
                        value_t xi) {
            const value_t a = (xi - x1) * (xi - x2) * (xi - x3) / ((x0 - x1) * (x0 - x2) * (x0 - x3)) * y0;
            const value_t b = (xi - x0) * (xi - x2) * (xi - x3) / ((x1 - x0) * (x1 - x2) * (x1 - x3)) * y1;
            const value_t c = (xi - x0) * (xi - x1) * (xi - x3) / ((x2 - x0) * (x2 - x1) * (x2 - x3)) * y2;
            const value_t d = (xi - x0) * (xi - x1) * (xi - x2) / ((x3 - x0) * (x3 - x1) * (x3 - x2)) * y3;
            return a + b + c + d;
        };

        Container y(x.size());

#pragma omp parallel for schedule(guided)
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<value_t>::quiet_NaN();
                continue;
            }
            if (utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            idx = (idx == 1) ? idx + 2 : idx;
            idx = (idx == 2) ? idx + 1 : idx;
            const value_t x0 = xp[idx - 3], x1 = xp[idx - 2], x2 = xp[idx - 1], x3 = xp[idx];
            const value_t y0 = yp[idx - 3], y1 = yp[idx - 2], y2 = yp[idx - 1], y3 = yp[idx];
            y[i] = calc_(x0, y0, x1, y1, x2, y2, x3, y3, x[i]);
        }
        return y;
    }

    template<class Container>
    constexpr auto cubic_spline(const Container &x, const Container &xp, const Container &yp) -> Container {
        return ni::_1d::detail::i_gsl<ni::_1d::detail::TypeGSL::CubicSpline, Container>(xp, yp)(x);
    }

    template<class Container>
    constexpr auto akima(const Container &x, const Container &xp, const Container &yp) -> Container {
        return ni::_1d::detail::i_gsl<ni::_1d::detail::TypeGSL::Akima, Container>(xp, yp)(x);
    }

    template<class Container>
    constexpr auto steffen(const Container &x, const Container &xp, const Container &yp) -> Container {
        return ni::_1d::detail::i_gsl<ni::_1d::detail::TypeGSL::Steffen, Container>(xp, yp)(x);
    }
}
