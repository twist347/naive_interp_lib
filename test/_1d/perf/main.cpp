#include <gtest/gtest.h>
#include <interp.h>
#include "../../test_utils.h"

double (*func)(double) = plotting::curves::custom;
constexpr int count = 2'000'000;
constexpr double x_min = -15.0, x_max = 15.0;
const auto [xp, yp, x] = plotting::_1d::generate_vals(func, count, x_min, x_max);

TEST(Prev, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::Prev>(xp, yp, x);
    std::cout << "prev: " << duration << " ms\n";
}

TEST(Next, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::Next>(xp, yp, x);
    std::cout << "next: " << duration << " ms\n";
}

TEST(NearestNeighbour, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::NearestNeighbour>(xp, yp, x);
    std::cout << "nearest_neighbour: " << duration << " ms\n";
}

TEST(Linear, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::Linear>(xp, yp, x);
    std::cout << "linear: " << duration << " ms\n";
}

TEST(Quadratic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::Quadratic>(xp, yp, x);
    std::cout << "quadratic: " << duration << " ms\n";
}

TEST(Cubic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::Cubic>(xp, yp, x);
    std::cout << "cubic: " << duration << " ms\n";
}

TEST(CubicSpline, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::CubicSpline>(xp, yp, x);
    std::cout << "cubic_spline: " << duration << " ms\n";
}

TEST(Akima, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::Akima>(xp, yp, x);
    std::cout << "akima: " << duration << " ms\n";
}

TEST(Steffen, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type1D::Steffen>(xp, yp, x);
    std::cout << "steffen: " << duration << " ms\n";
}

TEST(Linear, Avg) {
    long long avg_dur = 0L;
    for (std::size_t i = 0; i < 300; ++i) {
        const auto [_, duration] = interp_measure_time<ni::Type1D::Linear>(xp, yp, x);
        avg_dur += duration;
    }
    std::cout << "avg linear: " << avg_dur / 300 << '\n';
}

int main(int argc, char **argv) {
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << '\n';
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}