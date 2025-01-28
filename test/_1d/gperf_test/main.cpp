#include <benchmark/benchmark.h>
#include "interp.h"
#include "../../test_utils.h"

#include <vector>

double (*func)(double) = plotting::curves::custom;

using value_type = double;
using container_type = std::vector<value_type>;
constexpr std::size_t start = 250'000, finish = 2'000'000, step = 250'000;
constexpr std::size_t small_start = 1'000, small_finish = 4'000, small_step = 1'000;
constexpr value_type x_min = -15.0, x_max = 15.0;

static auto gb_prev_interp_small_data(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::Prev>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y)
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_next_interp_small_data(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::Next>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y)
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_nn_interp_small_data(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::NearestNeighbour>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y)
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_linear_interp_small_data(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::NearestNeighbour>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y)
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_quadratic_interp_small_data(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::NearestNeighbour>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y)
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_cubic_interp_small_data(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::NearestNeighbour>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y)
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_prev_interp(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::Prev>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y),
                {.exec = interp::Exec::PAR }
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_next_interp(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::Next>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y),
                {.exec = interp::Exec::PAR_UNSEQ}
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_nn_interp(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::NearestNeighbour>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y),
                {.exec = interp::Exec::PAR_UNSEQ}
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_linear_interp(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::NearestNeighbour>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y),
                {.exec = interp::Exec::PAR_UNSEQ}
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_quadratic_interp(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::NearestNeighbour>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y),
                {.exec = interp::Exec::PAR_UNSEQ}
        );
        benchmark::ClobberMemory();
    }
}

static auto gb_cubic_interp(benchmark::State &state) -> void {
    const auto size = state.range(0);
    container_type xp(size), yp(size), x(size), y(size);
    test_utils::fill_rnd_range(xp.begin(), xp.end(), x_min, x_max);
    test_utils::fill_rnd_range(yp.begin(), yp.end(), x_min, x_max);
    test_utils::fill_rnd_range(x.begin(), x.end(), x_min, x_max);
    std::sort(xp.begin(), xp.end());

    for ([[maybe_unused]] auto _ : state) {
        interp::do_i<interp::Type1D::NearestNeighbour>(
                std::cbegin(x), std::cend(x),
                std::cbegin(xp), std::cend(xp),
                std::cbegin(yp),
                std::begin(y),
                {.exec = interp::Exec::PAR_UNSEQ}
        );
        benchmark::ClobberMemory();
    }
}

constexpr auto time_unit = benchmark::kMicrosecond;
constexpr double min_wu_t = 1.0;

BENCHMARK(gb_prev_interp_small_data)->DenseRange(small_start, small_finish, small_step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_next_interp_small_data)->DenseRange(small_start, small_finish, small_step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_nn_interp_small_data)->DenseRange(small_start, small_finish, small_step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_linear_interp_small_data)->DenseRange(small_start, small_finish, small_step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_quadratic_interp_small_data)->DenseRange(small_start, small_finish, small_step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_cubic_interp_small_data)->DenseRange(small_start, small_finish, small_step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);

BENCHMARK(gb_prev_interp)->DenseRange(start, finish, step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_next_interp)->DenseRange(start, finish, step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_nn_interp)->DenseRange(start, finish, step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_linear_interp)->DenseRange(start, finish, step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_quadratic_interp)->DenseRange(start, finish, step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);
BENCHMARK(gb_cubic_interp)->DenseRange(start, finish, step)->Unit(time_unit)->MinWarmUpTime(min_wu_t);

BENCHMARK_MAIN();