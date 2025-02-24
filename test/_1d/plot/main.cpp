#include <iostream>
#include <vector>

#include "interp.h"
#include "test_utils.h"

// LINUX ONLY PLOTTING VIA GNUPLOT

int main() {
#ifdef __linux

    std::cout << "Plotting 1d interps starts in ";
#ifndef NDEBUG
    std::cout << "Debug build ";
#else
    std::cout << "Release build ";
#endif
    std::cout << "with Address, Undefined and Leak sanitizers\n";

    using container = std::vector<double>;
    using namespace plotting::_1d;

    const std::string folder_name = "data";
    const std::string script_name = "script";
    const auto data_path = std::filesystem::canonical(__FILE__).parent_path() / folder_name;

    if (!std::filesystem::exists(data_path)) {
        std::filesystem::create_directory(data_path);
    }

    double (*func)(double) = plotting::curves::custom;
    constexpr int count = 10'000;
    constexpr double x_min = -15.0, x_max = 15.0;
    const auto [xp, yp, x] = generate_arrays<container>(func, count, x_min, x_max);

    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << " y: " << x.size() << '\n';

    const container y_prev = interp::do_i<interp::Type1D::Prev>(x, xp, yp);
    const container y_next = interp::do_i<interp::Type1D::Next>(x, xp, yp);
    const container y_nn = interp::do_i<interp::Type1D::NearestNeighbour>(x, xp, yp);
    const container y_linear = interp::do_i<interp::Type1D::Linear>(x, xp, yp);
    const container y_quadratic = interp::do_i<interp::Type1D::Quadratic>(x, xp, yp);
    const container y_cubic = interp::do_i<interp::Type1D::Cubic>(x, xp, yp);

    plotting_gnuplot(xp, yp, data_path, "orig");
    plotting_gnuplot(x, y_prev, data_path, "prev");
    plotting_gnuplot(x, y_next, data_path, "next");
    plotting_gnuplot(x, y_nn, data_path, "nn");
    plotting_gnuplot(x, y_linear, data_path, "linear");
    plotting_gnuplot(x, y_quadratic, data_path, "quadratic");
    plotting_gnuplot(x, y_cubic, data_path, "cubic");

    std::filesystem::path script_path = data_path / script_name;
    std::ofstream script(script_path);

    if (script.is_open()) {
        script <<
               "set size ratio -1\n"
               "set xzeroaxis\n"
               "set yzeroaxis\n"
               "plot 'orig' with points"
               ", 'prev' with points"
               ", 'next' with points"
               ", 'nn' with points"
               ", 'linear' with points"
               ", 'quadratic' with points"
               ", 'cubic' with points\n"
               "pause -1";
        script.close();
    } else {
        std::cerr << "Unable to open script file for writing\n";
        return 1;
    }

// execute the gnuplot command
    std::string cmd = "cd " + script_path.parent_path().string() + " && gnuplot ./script";
    if (std::system(cmd.c_str()) != 0) {
        std::cerr << "Failed to execute command: " << cmd << '\n';
    }

#endif
}