#pragma once

#include <iostream>
#include <filesystem>
#include <random>
#include <chrono>
#include <vector>
#include <utils.h>

template<class Container>
constexpr void print(const Container &c) {
    for (const auto &val: c) {
        std::cout << val << '\n';
    }
}

template<class T>
constexpr auto arrays_eq(const T &real, const T &expected,
               typename std::remove_cvref_t<T>::value_type eps =
               ni::utils::eps<typename std::remove_cvref_t<T>::value_type>) -> bool {
    using idx_t = std::remove_cvref_t<T>::size_type;
    if (real.size() != expected.size()) {
        return false;
    }
    for (idx_t i = 0; i < real.size(); ++i) {
        const auto r = real[i], e = expected[i];
        if ((!std::isnan(r) || !std::isnan(e)) && !ni::utils::eq_flt(r, e, eps)) {
            std::cerr << std::fixed << std::setprecision(-std::log10(eps)) << "real: " << r << ", expected: " << e
                      << " at idx " << i << '\n';
            return false;
        }
    }
    return true;
}

namespace plotting::curves {

    template<class Value = double>
    constexpr Value custom(Value x) {
        return std::sin(x) + 0.5 * std::sin(2 * x) + 0.2 * std::sin(5 * x) + std::exp(-std::pow(x, 2));
    }
}

namespace plotting::surfaces {
    template<class Value = double>
    constexpr Value elliptical_paraboloid(Value x, Value y) {
        return (x * x) + (y * y);
    }

    template<class Value = double>
    constexpr Value hyperbolic_paraboloid(Value x, Value y) {
        return (x * x) - (y * y);
    }

    template<class Value = double>
    constexpr Value ellipsoid(Value x, Value y) {
        return std::sqrt(1.0 - (x * x) - (y * y));
    }

    template<class Value = double>
    constexpr Value cone(Value x, Value y) {
        return std::sqrt((x * x) + (y * y));
    }

    template<class Value = double>
    constexpr Value sin_cos(Value x, Value y) {
        return std::sin(x) * std::cos(y);
    }

    template<class Value = double>
    constexpr Value one_sheet_hyperboloid(Value x, Value y) {
        return std::sqrt(1.0 - (x * x) + (y * y));
    }
}

namespace plotting {
    namespace _1d {
        template<class Container>
        constexpr auto plotting_gnuplot(const Container &xp, const Container &yp,
                              const std::string &path, const std::string &file_name, int step = 1) -> void {
            const std::filesystem::path plot_path(path);
            std::ofstream source(plot_path / file_name);
            for (std::size_t i = 0; i < xp.size(); i += step) {
                source << xp[i] << ' ' << yp[i] << '\n';
            }
            source.close();
        }

        template<class Value = double>
        constexpr auto generate_vals(Value (*func)(Value), int N) {
            const Value x_min = -15.0, x_max = 15.0;
            std::vector<Value> xp(N), yp(N), x(N / 10);

            const Value step = std::abs(x_max - x_min) / N;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<Value> dis(x_min + step, x_max - step);

            // generation of source points
            Value dx = x_min;
            for (std::size_t i = 0; i < xp.size(); ++i) {
                xp[i] = dx;
                yp[i] = func(dx);
                dx += step;
            }

            // generation of query points
            for (int i = 0; i < x.size(); ++i) {
                x[i] = dis(gen);
            }
            return std::tuple(xp, yp, x);
        }
    }

    namespace _2d {
        template<class Container>
        constexpr auto plotting_gnuplot(const Container &xp, const Container &yp, const Container &zp,
                              const std::string &path, const std::string &file_name, int step = 1) -> void {
            const std::filesystem::path plot_path(path);
            std::ofstream source(plot_path / file_name);
            for (std::size_t i = 0; i < xp.size(); i += step) {
                source << xp[i] << ' ' << yp[i] << ' ' << zp[i] << '\n';
            }
            source.close();
        }

        template<class Value = double>
        constexpr auto generate_vals(Value (*func)(Value, Value), int N) {
            const Value x_min = -10.0, x_max = 10.0;
            const Value y_min = -10.0, y_max = 10.0;
            const Value step = std::abs(x_max - x_min) / N;
            const auto sz = (N + 1) * (N + 1);

            std::vector<Value> xp(sz), yp(sz), zp(sz), x(sz), y(sz);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<Value> dis(x_min + 0.1, x_max - 0.1);

            // generation of source points
            std::size_t idx = 0;
            for (Value dx = x_min; dx < x_max; dx += step) {
                for (Value dy = y_min; dy < y_max; dy += step) {
                    const Value z = func(dx, dy);
                    xp[idx] = dx;
                    yp[idx] = dy;
                    zp[idx] = z;
                    ++idx;
                }
            }

            // generation of query points
            for (std::size_t i = 0; i < x.size(); ++i) {
                x[i] = dis(gen);
                y[i] = dis(gen);
            }
            return std::tuple{xp, yp, zp, x, y};
        }
    }
}
