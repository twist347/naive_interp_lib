#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <chrono>
#include <interp_make.h>

double elliptical_paraboloid(double x, double y, double a = 1, double b = 1, double c = 1) {
    return ((x * x) / (a * a) + (y * y) / (b * b));
}

auto generate_vals() {
    double x_min = -10.0, x_max = 10.0;
    double y_min = -10.0, y_max = 10.0;
    int N = 500;
    double step = std::abs(x_max - x_min) / N;
    auto sz = (N + 1) * (N + 1);

    std::vector<double> xp(sz), yp(sz), zp(sz), x(sz), y(sz);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(x_min + 0.1, x_max - 0.1);

    // generation of source points
    std::size_t idx = 0;
    for (double dx = x_min; dx < x_max; dx += step) {
        for (double dy = y_min; dy < y_max; dy += step) {
            double z = elliptical_paraboloid(dx, dy);
            xp[idx] = dx;
            yp[idx] = dy;
            zp[idx] = z;
            ++idx;
        }
    }

    // generation of query points
    for (int i = 0; i < x.size(); ++i) {
        x[i] = dis(gen);
        y[i] = dis(gen);
    }
    return std::tuple{xp, yp, zp, x, y};
}

TEST(IDW, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, zp, x, y] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << "y: " << y.size() << '\n';
    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::IDW>(xp, yp, zp);
    auto z = interp(x, y);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "idw: " << duration << " ms\n";
}

TEST(NearestNeighbour, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, zp, x, y] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << "y: " << y.size() << '\n';
    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::NearestNeighbour>(xp, yp, zp);
    auto z = interp(x, y);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "nearest_neighbour: " << duration << " ms\n";
}

TEST(TIN, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, zp, x, y] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << "y: " << y.size() << '\n';
    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::TIN>(xp, yp, zp);
    auto z = interp(x, y);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "tin: " << duration << " ms\n";
}

#if 0

TEST(RBFLinear, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, zp, x, y] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << "y: " << y.size() << '\n';
    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Linear>(xp, yp, zp);
    auto z = interp(x, y);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_linear: " << duration << " ms\n";
}

TEST(RBFGauss, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, zp, x, y] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << "y: " << y.size() << '\n';
    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Gauss>(xp, yp, zp);
    auto z = interp(x, y);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "rbf_gauss: " << duration << " ms\n";
}

#endif

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}