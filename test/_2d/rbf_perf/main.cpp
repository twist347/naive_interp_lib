#include <gtest/gtest.h>
#include <interp.h>
#include "../../test_utils.h"

double (*func)(double, double) = plotting::surfaces::elliptical_paraboloid;
constexpr int count = 100; // count * count points
constexpr double x_min = -10.0, x_max = 10.0, y_min = -10.0, y_max = 10.0;
const auto [xp, yp, zp, x, y] = plotting::_2d::generate_vals(func, count, x_min, x_max, y_min, y_max);

TEST(RBFLinear, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type2DRBF::Linear>(xp, yp, zp, x, y);
    std::cout << "rbf_linear: " << duration << " ms\n";
}

TEST(RBFCubic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type2DRBF::Cubic>(xp, yp, zp, x, y);
    std::cout << "rbf_cubic: " << duration << " ms\n";
}

TEST(RBFQuintic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type2DRBF::Quintic>(xp, yp, zp, x, y);
    std::cout << "rbf_quintic: " << duration << " ms\n";
}

TEST(Multiquadric, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type2DRBF::Multiquadric>(xp, yp, zp, x, y);
    std::cout << "rbf_multiquadric: " << duration << " ms\n";
}

TEST(InverseMultiquadric, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type2DRBF::InverseMultiquadric>(xp, yp, zp, x, y);
    std::cout << "rbf_inverse_multiquadric: " << duration << " ms\n";
}

TEST(RBFGauss, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type2DRBF::Gaussian>(xp, yp, zp, x, y);
    std::cout << "rbf_gauss: " << duration << " ms\n";
}

TEST(RBFThinPlate, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [_, duration] = interp_measure_time<ni::Type2DRBF::ThinPlate>(xp, yp, zp, x, y);
    std::cout << "rbf_thin_plate: " << duration << " ms\n";
}

int main(int argc, char** argv) {
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << " y: " << y.size() << '\n';
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}