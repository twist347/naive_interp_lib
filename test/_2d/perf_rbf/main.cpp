#include <gtest/gtest.h>
#include <interp_make.h>
#include <test_utils.h>

double (*func)(double, double) = plotting::surfaces::elliptical_paraboloid;
constexpr int N = 100; // N * N points
const auto [xp, yp, zp, x, y] = plotting::_2d::generate_vals(func, N);

TEST(RBFLinear, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Linear>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_linear: " << duration << " ms\n";
}

TEST(RBFCubic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Cubic>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_cubic: " << duration << " ms\n";
}

TEST(RBFQuintic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Quintic>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_quintic: " << duration << " ms\n";
}

TEST(Multiquadric, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Multiquadric>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_multiquadric: " << duration << " ms\n";
}

TEST(InverseMultiquadric, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::InverseMultiquadric>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_inverse_multiquadric: " << duration << " ms\n";
}

TEST(RBFGauss, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Gaussian>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_gauss: " << duration << " ms\n";
}

TEST(RBFThinPlate, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    const auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::ThinPlate>(xp, yp, zp);
    const auto z = interp(x, y);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_thin_plate: " << duration << " ms\n";
}

int main(int argc, char** argv) {
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << " y: " << y.size() << '\n';
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}