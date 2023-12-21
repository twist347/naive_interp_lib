#pragma once

#include <limits>
#include <optional>
#include <algorithm>
#include <utils.h>

namespace naive_interp {
    template<class Container, class Scalar = Container::value_type>
    constexpr auto prev(const Container &x, const Container &xp, const Container &yp) -> Container {
        Container y(x.size());

        using idx_t = Container::size_type;

        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Scalar>::quiet_NaN();
                continue;
            }
            if (x[i] == xp[xp.size() - 1]) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            y[i] = yp[idx - 1];
        }
        return y;
    }

    template<class Container, class Scalar = Container::value_type>
    constexpr auto next(const Container &x, const Container &xp, const Container &yp) -> Container {
        Container y(x.size());

        using idx_t = Container::size_type;

        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Scalar>::quiet_NaN();
                continue;
            }
            if (x[i] == xp[xp.size() - 1]) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::lower_bound(xp.begin(), xp.end(), x[i]));
            y[i] = yp[idx];
        }
        return y;
    }

    template<class Container, class Scalar = Container::value_type>
    constexpr auto nearest_neighbour(const Container &x, const Container &xp, const Container &yp) -> Container {
        Container y(x.size());

        using idx_t = Container::size_type;

        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Scalar>::quiet_NaN();
                continue;
            }
            if (x[i] == xp[xp.size() - 1]) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            const Scalar prev_distance = std::abs(x[i] - xp[idx - 1]);
            const Scalar next_distance = std::abs(xp[idx] - x[i]);
            y[i] = prev_distance < next_distance ? yp[idx - 1] : yp[idx];
        }
        return y;
    }

    template<class Container, class Scalar = Container::value_type>
    constexpr auto linear(const Container &x, const Container &xp, const Container &yp) -> Container {
        Container y(x.size());

        using idx_t = Container::size_type;

        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Scalar>::quiet_NaN();
                continue;
            }
            if (x[i] == xp[xp.size() - 1]) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            const Scalar x0 = xp[idx - 1], x1 = xp[idx];
            const Scalar y0 = yp[idx - 1], y1 = yp[idx];
            y[i] = y0 + ((y1 - y0) / (x1 - x0)) * (x[i] - x0);
        }
        return y;
    }
}