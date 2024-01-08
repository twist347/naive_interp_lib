#include <random>
#include <vector>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <interp_make.h>

// LINUX ONLY PLOTTING VIA GNUPLOT

using FunctionPtr = double (*)(double);

double custom(double x) {
    return std::sin(x) + 0.5 * std::sin(2 * x) + 0.2 * std::sin(5 * x) + std::exp(-std::pow(x, 2));
}

template<class Container>
auto plotting_gnuplot(const Container &xp, const Container &yp, const std::string &file_name, int step = 1) -> void {
    std::filesystem::path plot_path = std::filesystem::canonical(__FILE__).parent_path() / "plot";
    std::ofstream source(plot_path / file_name);
    for (typename Container::size_type i = 0; i < xp.size(); i += step) {
        source << xp[i] << ' ' << yp[i] << '\n';
    }
    source.close();
}


auto generate_vals(FunctionPtr f) {
    double x_min = -15.0, x_max = 15.0;
    double y_min = -15.0, y_max = 15.0;
    const int N = 10000;
    std::vector<double> xp(N), yp(N), x(N / 10);

    double step = std::abs(x_max - x_min) / N;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(x_min + step, x_max - step);

    // generation of source points
    double dx = x_min;
    for (int i = 0; i < xp.size(); ++i) {
        xp[i] = dx;
        yp[i] = f(dx);
        dx += step;
    }

    // generation of query points
    for (int i = 0; i < x.size(); ++i) {
        x[i] = dis(gen);
    }
    return std::tuple(xp, yp, x);
}

int main() {
#ifdef __linux
    auto [xp, yp, x] = generate_vals(std::cos);
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << "y: " << x.size() << '\n';

    auto y_prev = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp)(x);
    auto y_next = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp)(x);
    auto y_nn = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp)(x);
    auto y_linear = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp)(x);
    auto y_quadratic = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp)(x);
    auto y_cubic = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp)(x);
    auto y_cubic_s = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp)(x);
    auto y_akima = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp)(x);
    auto y_steffen = ni::_1d::make_i<ni::_1d::Type1D::Steffen>(xp, yp)(x);

    plotting_gnuplot(xp, yp, "orig");
    plotting_gnuplot(x, y_prev, "prev");
    plotting_gnuplot(x, y_next, "next");
    plotting_gnuplot(x, y_nn, "nn");
    plotting_gnuplot(x, y_linear, "linear");
    plotting_gnuplot(x, y_quadratic, "quadratic");
    plotting_gnuplot(x, y_cubic, "cubic");
    plotting_gnuplot(x, y_cubic_s, "cubic_s");
    plotting_gnuplot(x, y_akima, "akima");
    plotting_gnuplot(x, y_steffen, "steffen");

    std::filesystem::path parent_path = std::filesystem::canonical(__FILE__).parent_path();
    std::system((std::string("cd ") + parent_path.string() + " && mkdir plot").c_str());

    std::filesystem::path script_path = parent_path / "plot";
    std::ofstream script(script_path / "script");
    script << "set size ratio -1\n"
              "set xzeroaxis\n"
              "set yzeroaxis\n"
              "plot 'orig' with points"
              ", 'prev' with points"
              ", 'next' with points"
              ", 'nn' with points"
              ", 'linear' with points"
              ", 'quadratic' with points"
              ", 'cubic' with points"
              ", 'cubic_s' with points"
              ", 'akima' with points"
              ", 'steffen' with points\n"
              "pause -1";
    script.close();
    std::string cmd = std::string("cd ") + script_path.string() + " && gnuplot ./script";
    std::system(cmd.c_str());
#endif
}
