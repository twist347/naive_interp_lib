#include <gtest/gtest.h>
#include <interp_make.h>
#include <test_utils.h>

double (*func)(double) = plotting::curves::custom;
constexpr int N = 1000000;
const auto [xp, yp, x] = plotting::_1d::generate_vals(func, N);

TEST(Prev, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "prev: " << duration << " ms\n";
}

TEST(Next, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "next: " << duration << " ms\n";
}

TEST(NearestNeighbour, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "nearest_neighbour: " << duration << " ms\n";
}

TEST(Linear, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "linear: " << duration << " ms\n";
}

TEST(Quadratic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "quadratic: " << duration << " ms\n";
}

TEST(Cubic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "cubic: " << duration << " ms\n";
}

TEST(CubicSpline, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp);
    const auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "cubic_spline: " << duration << " ms\n";
}

TEST(CubicSpline, Akima) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp);
    const auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "akima: " << duration << " ms\n";
}

TEST(CubicSpline, Steffen) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Steffen>(xp, yp);
    const  auto y = interp(x);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "steffen: " << duration << " ms\n";
}

int main(int argc, char **argv) {
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << '\n';
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}