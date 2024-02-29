#include <interp.h>
#include "../../test_utils.h"

// LINUX ONLY PLOTTING VIA GNUPLOT

int main() {
#ifdef __linux

    using namespace plotting::_1d;
    namespace fs = std::filesystem;

    const std::string folder_name = "data";
    const std::string script_name = "script";
    const auto data_path = fs::canonical(__FILE__).parent_path() / folder_name;

    if (!fs::exists(data_path)) {
        fs::create_directory(data_path);
    }

    double (*func)(double) = plotting::curves::custom;
    constexpr int count = 10'000;
    constexpr double x_min = -15.0, x_max = 15.0;
    const auto [xp, yp, x] = generate_vals(func, count, x_min, x_max);

    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << "y: " << x.size() << '\n';

    const auto y_prev = ni::make_i<ni::Type1D::Prev>(xp, yp)(x);
    const auto y_next = ni::make_i<ni::Type1D::Next>(xp, yp)(x);
    const auto y_nn = ni::make_i<ni::Type1D::NearestNeighbour>(xp, yp)(x);
    const auto y_linear = ni::make_i<ni::Type1D::Linear>(xp, yp)(x);
    const auto y_quadratic = ni::make_i<ni::Type1D::Quadratic>(xp, yp)(x);
    const auto y_cubic = ni::make_i<ni::Type1D::Cubic>(xp, yp)(x);
    const auto y_cubic_s = ni::make_i<ni::Type1D::CubicSpline>(xp, yp)(x);
    const auto y_akima = ni::make_i<ni::Type1D::Akima>(xp, yp)(x);
    const auto y_steffen = ni::make_i<ni::Type1D::Steffen>(xp, yp)(x);

    plotting_gnuplot(xp, yp, data_path, "orig");
    plotting_gnuplot(x, y_prev, data_path, "prev");
    plotting_gnuplot(x, y_next, data_path, "next");
    plotting_gnuplot(x, y_nn, data_path, "nn");
    plotting_gnuplot(x, y_linear, data_path, "linear");
    plotting_gnuplot(x, y_quadratic, data_path, "quadratic");
    plotting_gnuplot(x, y_cubic, data_path, "cubic");
    plotting_gnuplot(x, y_cubic_s, data_path, "cubic_s");
    plotting_gnuplot(x, y_akima, data_path, "akima");
    plotting_gnuplot(x, y_steffen, data_path, "steffen");

    fs::path script_path = data_path / script_name;
    std::ofstream script(script_path);

    if (script.is_open()) {
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
    } else {
        std::cerr << "Unable to open script file for writing\n";
        return 1;
    }

    // Execute the gnuplot command
    std::string cmd = "cd " + script_path.parent_path().string() + " && gnuplot ./script";
    if (std::system(cmd.c_str()) != 0) {
        std::cerr << "Failed to execute command: " << cmd << '\n';
    }
#endif
}
