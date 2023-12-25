#pragma once

#include <limits>
#include <optional>
#include <algorithm>
#include <utils.h>

namespace ni {
    template<class Container, class Value = Container::value_type>
    constexpr auto prev(const Container &x, const Container &xp, const Container &yp) -> Container {
        Container y(x.size());

        using idx_t = Container::size_type;

#pragma omp parallel for
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Value>::quiet_NaN();
                continue;
            }
            if (ni::utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            y[i] = yp[idx - 1];
        }
        return y;
    }

    template<class Container, class Value = Container::value_type>
    constexpr auto next(const Container &x, const Container &xp, const Container &yp) -> Container {
        Container y(x.size());

        using idx_t = Container::size_type;

#pragma omp parallel for
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Value>::quiet_NaN();
                continue;
            }
            if (ni::utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::lower_bound(xp.begin(), xp.end(), x[i]));
            y[i] = yp[idx];
        }
        return y;
    }

    template<class Container, class Value = Container::value_type>
    constexpr auto nearest_neighbour(const Container &x, const Container &xp, const Container &yp) -> Container {
        Container y(x.size());

        using idx_t = Container::size_type;

#pragma omp parallel for
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Value>::quiet_NaN();
                continue;
            }
            if (ni::utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            const Value prev_distance = std::abs(x[i] - xp[idx - 1]);
            const Value next_distance = std::abs(xp[idx] - x[i]);
            y[i] = ni::utils::less_flt(prev_distance, next_distance) ? yp[idx - 1] : yp[idx];
        }
        return y;
    }

    template<class Container, class Value = Container::value_type>
    constexpr auto linear(const Container &x, const Container &xp, const Container &yp) -> Container {
        Container y(x.size());

        using idx_t = Container::size_type;

#pragma omp parallel for
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Value>::quiet_NaN();
                continue;
            }
            if (ni::utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            const auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            const Value x0 = xp[idx - 1], x1 = xp[idx];
            const Value y0 = yp[idx - 1], y1 = yp[idx];
            y[i] = y0 + ((y1 - y0) / (x1 - x0)) * (x[i] - x0);
        }
        return y;
    }

    template<class Container, class Value = Container::value_type>
    constexpr auto quadratic(const Container &x, const Container &xp, const Container &yp) -> Container {
        auto calc_ = [](Value x0, Value y0, Value x1, Value y1, Value x2, Value y2, Value xi) {
            const Value a = ((xi - x1) * (xi - x2)) / ((x0 - x1) * (x0 - x2)) * y0;
            const Value b = ((xi - x0) * (xi - x2)) / ((x1 - x0) * (x1 - x2)) * y1;
            const Value c = ((xi - x0) * (xi - x1)) / ((x2 - x0) * (x2 - x1)) * y2;
            return a + b + c;
        };

        Container y(x.size());

        using idx_t = Container::size_type;

#pragma omp parallel for
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Value>::quiet_NaN();
                continue;
            }
            if (ni::utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            idx = (idx == 1) ? idx + 1 : idx;
            const Value x0 = xp[idx - 2], x1 = xp[idx - 1], x2 = xp[idx];
            const Value y0 = yp[idx - 2], y1 = yp[idx - 1], y2 = yp[idx];
            y[i] = calc_(x0, y0, x1, y1, x2, y2, x[i]);
        }
        return y;
    }

    template<class Container, class Value = Container::value_type>
    constexpr auto cubic(const Container &x, const Container &xp, const Container &yp) -> Container {
        auto calc_ = [](Value x0, Value y0, Value x1, Value y1, Value x2, Value y2, Value x3, Value y3, Value xi) {
            const Value a = ((xi - x1) * (xi - x2) * (xi - x3) / ((x0 - x1) * (x0 - x2) * (x0 - x3))) * y0;
            const Value b = ((xi - x0) * (xi - x2) * (xi - x3) / ((x1 - x0) * (x1 - x2) * (x1 - x3))) * y1;
            const Value c = ((xi - x0) * (xi - x1) * (xi - x3) / ((x2 - x0) * (x2 - x1) * (x2 - x3))) * y2;
            const Value d = ((xi - x0) * (xi - x1) * (xi - x2) / ((x3 - x0) * (x3 - x1) * (x3 - x2))) * y3;
            return a + b + c + d;
        };

        Container y(x.size());

        using idx_t = Container::size_type;

#pragma omp parallel for
        for (idx_t i = 0; i < x.size(); ++i) {
            if (std::isnan(x[i])) {
                y[i] = std::numeric_limits<Value>::quiet_NaN();
                continue;
            }
            if (ni::utils::eq_flt(x[i], xp[xp.size() - 1])) {
                y[i] = yp[yp.size() - 1];
                continue;
            }
            auto idx = std::distance(xp.begin(), std::upper_bound(xp.begin(), xp.end(), x[i]));
            idx = (idx == 1) ? idx + 2 : idx;
            idx = (idx == 2) ? idx + 1 : idx;
            const Value x0 = xp[idx - 3], x1 = xp[idx - 2], x2 = xp[idx - 1], x3 = xp[idx];
            const Value y0 = yp[idx - 3], y1 = yp[idx - 2], y2 = yp[idx - 1], y3 = yp[idx];
            y[i] = calc_(x0, y0, x1, y1, x2, y2, x3, y3, x[i]);
        }
        return y;
    }
}