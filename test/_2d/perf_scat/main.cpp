#include <gtest/gtest.h>
#include <interp_make.h>
#include <test_utils.h>

double (*func)(double, double) = plotting::surfaces::elliptical_paraboloid;
constexpr int N = 500; // N * N
const auto [xp, yp, zp, x, y] = plotting::_2d::generate_vals(func, N);

TEST(IDW, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::IDW>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "idw: " << duration << " ms\n";
}

TEST(NearestNeighbour, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::NearestNeighbour>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "nearest_neighbour: " << duration << " ms\n";
}

TEST(TIN, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::TIN>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "tin: " << duration << " ms\n";
}

int main(int argc, char** argv) {
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << " y: " << y.size() << '\n';
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}