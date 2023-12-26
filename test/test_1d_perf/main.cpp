#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <interp_make.h>

auto generate_vals() {
    double x_min = -15.0, x_max = 15.0;
    double y_min = -15.0, y_max = 15.0;
    const int N = 500000;
    std::vector<double> xp(N), yp(N), x(N * 2);

    double step = std::abs(x_max - x_min) / N;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(x_min + step, x_max - step);
    auto func = [](double x) {
        return std::sin(x) + 0.5 * std::sin(2 * x) + 0.2 * std::sin(5 * x) + std::exp(-std::pow(x, 2));
    };

    // generation of source points
    double dx = x_min;
    for (int i = 0; i < xp.size(); ++i) {
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

TEST(Prev, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, x] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << '\n';

    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp);
    auto y = interp(x);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "prev: " << duration << " ms\n";
}

TEST(Next, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, x] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << '\n';

    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp);
    auto y = interp(x);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "next: " << duration << " ms\n";
}

TEST(NearestNeighbour, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, x] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << '\n';

    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp);
    auto y = interp(x);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "nearest_neighbour: " << duration << " ms\n";
}

TEST(Linear, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, x] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << '\n';

    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    auto y = interp(x);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "linear: " << duration << " ms\n";
}

TEST(Quadratic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, x] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << '\n';

    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp);
    auto y = interp(x);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "quadratic: " << duration << " ms\n";
}

TEST(Cubic, Perf) {
#ifndef NDEBUG
    GTEST_SKIP_("allowed only in release build");
#endif
    auto [xp, yp, x] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << '\n';

    auto start = std::chrono::steady_clock::now();
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp);
    auto y = interp(x);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "cubic: " << duration << " ms\n";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}