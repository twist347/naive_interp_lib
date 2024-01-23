#include <gtest/gtest.h>
#include <interp_make.h>
#include "../../test_utils.h"

double (*func)(double, double) = plotting::surfaces::elliptical_paraboloid;
constexpr int count = 500; // count * count
constexpr double x_min = -10.0, x_max = 10.0, y_min = -10.0, y_max = 10.0;
const auto [xp, yp, zp, x, y] = plotting::_2d::generate_vals(func, count, x_min, x_max, y_min, y_max);

TEST(IDW, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [res, duration] = interp_measure_time<ni::Type2DScat::IDW>(xp, yp, zp, x, y);
    std::cout << "idw: " << duration << " ms\n";
}

TEST(NearestNeighbour, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [res, duration] = interp_measure_time<ni::Type2DScat::NearestNeighbour>(xp, yp, zp, x, y);
    std::cout << "nearest_neighbour: " << duration << " ms\n";
}

TEST(TIN, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto [res, duration] = interp_measure_time<ni::Type2DScat::TIN>(xp, yp, zp, x, y);
    std::cout << "tin: " << duration << " ms\n";
}

int main(int argc, char** argv) {
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << " y: " << y.size() << '\n';
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}