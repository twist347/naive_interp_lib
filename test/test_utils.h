#pragma once

#include <filesystem>
#include <chrono>
#include <fstream>
#include <random>
#include <iostream>
#include <interp.h>

namespace test_utils {

    template<typename Value>
    concept Numeric = std::integral<Value> || std::floating_point<Value>;

    namespace detail {

        template<Numeric Value>
        struct RndDis {
            using type = std::conditional_t<
                    std::is_integral_v<Value>,
                    std::uniform_int_distribution<Value>,
                    std::uniform_real_distribution<Value>
            >;
        };

    }

    template<typename Value>
    requires Numeric<Value>
    auto gen_rnd_num(Value min, Value max) -> Value {
        static std::mt19937 gen(std::random_device{}());
        return typename detail::RndDis<Value>::type{ min, max }(gen);
    }

    template<typename Iter>
    requires Numeric<typename std::iterator_traits<Iter>::value_type>
    auto fill_rnd_range(
        Iter first, Iter last,
        typename std::iterator_traits<Iter>::value_type min_val,
        typename std::iterator_traits<Iter>::value_type max_val
    ) -> void {
        std::generate(first, last, [min_val, max_val]() { return gen_rnd_num(min_val, max_val); });
    }

}

template<typename Container>
void print(const Container &c) {
    for (const auto &val: c) {
        std::cout << val << ' ';
    }
    std::cout << '\n';
}

template<class T, typename Scalar = typename T::value_type>
auto arrays_eq(const T &real, const T &expected, Scalar eps = std::numeric_limits<Scalar>::epsilon()) -> bool {
    if (real.size() != expected.size()) {
        std::cerr << "real.size() != expected.size()\n";
        return false;
    }
    for (typename T::size_type i = 0; i < real.size(); ++i) {
        const auto r = real[i], e = expected[i];
        if ((!std::isnan(r) || !std::isnan(e)) && r != e) {
            std::cerr << std::fixed << std::setprecision(-std::log10(eps)) << "real: " << r << ", expected: " << e
                      << " at idx " << i << '\n';
            return false;
        }
    }
    return true;
}

template<typename Container>
auto read_from_csv(const std::string &filename) -> Container {
    using size_type = typename Container::size_type;
    std::ifstream file(filename);

    if (!file) {
        throw std::ifstream::failure("File not found: " + filename);
    }

    std::string line;
    Container array;
    size_type num_lines = 0;

    while (std::getline(file, line)) {
        ++num_lines;
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    array.resize(num_lines);
    for (size_type i = 0; i < num_lines; ++i) {
        std::getline(file, line);
        array[i] = std::stod(line);
    }
    return array;
}

template<auto type, bool ReturnData = false>
auto interp_measure_time(const auto &xp, const auto &yp, const auto &x) {
    const auto start = std::chrono::high_resolution_clock::now();

    const auto res = interp::do_i<type>(x, xp, yp);

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if constexpr (ReturnData) {
        return std::make_pair(res, duration);
    } else {
        return duration;
    }
}

template<auto type, bool ReturnData = false>
auto interp_measure_time(
        const auto &xp, const auto &yp, const auto &zp,
        const auto &x, const auto &y
) {
    const auto start = std::chrono::steady_clock::now();

    const auto res = interp::do_i<type>(x, y, xp, yp, zp);

    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if constexpr (ReturnData) {
        return std::make_pair(res, duration);
    } else {
        return duration;
    }
}

namespace plotting::curves {

    template<typename Value = double>
    Value custom(Value x) {
        return std::sin(x) + 0.5 * std::sin(2 * x) + 0.2 * std::sin(5 * x) + std::exp(-std::pow(x, 2));
    }

    template<typename Value = double>
    Value sincos(Value x) {
        return std::sin(x) * std::cos(x);
    }

    template<typename Value = double>
    Value xcos(Value x) {
        return x * std::cos(x);
    }

    template<typename Value = double>
    Value xsin(Value x) {
        return x * std::sin(x);
    }

}

namespace plotting::surfaces {

    template<typename Value = double>
    Value elliptical_paraboloid(Value x, Value y) {
        return (x * x) + (y * y);
    }

    template<typename Value = double>
    Value hyperbolic_paraboloid(Value x, Value y) {
        return (x * x) - (y * y);
    }

    template<typename Value = double>
    Value ellipsoid(Value x, Value y) {
        return std::sqrt(1.0 - (x * x) - (y * y));
    }

    template<typename Value = double>
    Value cone(Value x, Value y) {
        return std::sqrt((x * x) + (y * y));
    }

    template<typename Value = double>
    Value sin_cos(Value x, Value y) {
        return std::sin(x) * std::cos(y);
    }

    template<typename Value = double>
    Value one_sheet_hyperboloid(Value x, Value y) {
        return std::sqrt(1.0 - (x * x) + (y * y));
    }

}

namespace plotting::_1d {

    template<class Container>
    auto plotting_gnuplot(
            const Container &xp,
            const Container &yp,
            const std::string &path,
            const std::string &file_name,
            int step = 1
    ) -> void {
        using size_type = typename Container::size_type;
        const std::filesystem::path plot_path(path);
        std::ofstream source(plot_path / file_name);
        for (size_type i = 0; i < xp.size(); i += step) {
            source << xp[i] << ' ' << yp[i] << '\n';
        }
        source.close();
    }

    template<class Container, class Value = typename Container::value_type>
    auto generate_arrays(Value (*func)(Value), int count, Value x_min, Value x_max) {
        Container xp(count), yp(count), x(count / 10);
        using size_type = typename Container::size_type;

        const Value step = std::abs(x_max - x_min) / count;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<Value> dis(x_min + step, x_max - step);

        // generation of source points
        Value dx = x_min;
        for (size_type i = 0; i < xp.size(); ++i) {
            xp[i] = dx;
            yp[i] = func(dx);
            dx += step;
        }

        // generation of query points
        for (size_type i = 0; i < x.size(); ++i) {
            x[i] = dis(gen);
        }
        return std::tuple(xp, yp, x);
    }

    template<class Container, class Value = typename Container::value_type>
    auto generate_array(int count, Value x_min, Value x_max) {
        using size_type = typename Container::size_type;
        Container x(count);

        const Value step = std::abs(x_max - x_min) / count;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<Value> dis(x_min + step, x_max - step);

        // generation of query points
        for (size_type i = 0; i < x.size(); ++i) {
            x[i] = dis(gen);
        }
        return x;
    }

}

namespace plotting::_2d {

    template<class Container>
    auto plotting_gnuplot(
            const Container &xp,
            const Container &yp,
            const Container &zp,
            const std::string &path,
            const std::string &file_name,
            int step = 1
    ) -> void {
        using size_type = typename Container::size_type;
        const std::filesystem::path plot_path(path);
        std::ofstream source(plot_path / file_name);
        for (size_type i = 0; i < xp.size(); i += step) {
            source << xp[i] << ' ' << yp[i] << ' ' << zp[i] << '\n';
        }
        source.close();
    }

    template<class Container, class Value = typename Container::value_type>
    auto generate_arrays(
            Value (*func)(Value, Value),
            int count,
            Value x_min, Value x_max, Value y_min, Value y_max
    ) {
        using size_type = typename Container::size_type;
        const Value step = std::abs(x_max - x_min) / count;
        const auto sz = (count + 1) * (count + 1);

        Container xp(sz), yp(sz), zp(sz), x(sz), y(sz);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<Value> dis(x_min + 0.1, x_max - 0.1);

        // generation of source points
        size_type idx = 0;
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
        for (size_type i = 0; i < x.size(); ++i) {
            x[i] = dis(gen);
            y[i] = dis(gen);
        }
        return std::tuple{xp, yp, zp, x, y};
    }

    template<typename TimeUnit, typename Func, typename ... Args>
    auto time_measure(Func func_to_measure, Args &&... args) {
        const auto start = std::chrono::steady_clock::now();
        func_to_measure(std::forward<Args>(args)...);
        const auto end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<TimeUnit>(end - start);
    }

}