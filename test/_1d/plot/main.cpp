#include <interp_make.h>
#include <test_utils.h>

// LINUX ONLY PLOTTING VIA GNUPLOT

int main() {
#ifdef __linux
    using namespace plotting::_1d;

    const std::string folder_name = "data";
    const auto path = std::filesystem::canonical(__FILE__).parent_path();

    double (*func)(double) = plotting::curves::custom;
    constexpr int N = 10000;
    const auto [xp, yp, x] = plotting::_1d::generate_vals(func, N);

    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << '\n';
    std::cout << "x: " << x.size() << "y: " << x.size() << '\n';

    const auto y_prev = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp)(x);
    const auto y_next = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp)(x);
    const auto y_nn = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp)(x);
    const auto y_linear = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp)(x);
    const auto y_quadratic = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp)(x);
    const auto y_cubic = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp)(x);
    const auto y_cubic_s = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp)(x);
    const auto y_akima = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp)(x);
    const auto y_steffen = ni::_1d::make_i<ni::_1d::Type1D::Steffen>(xp, yp)(x);

    plotting_gnuplot(xp, yp, path / folder_name, "orig");
    plotting_gnuplot(x, y_prev, path / folder_name, "prev");
    plotting_gnuplot(x, y_next, path / folder_name, "next");
    plotting_gnuplot(x, y_nn, path / folder_name, "nn");
    plotting_gnuplot(x, y_linear, path / folder_name, "linear");
    plotting_gnuplot(x, y_quadratic, path / folder_name, "quadratic");
    plotting_gnuplot(x, y_cubic, path / folder_name, "cubic");
    plotting_gnuplot(x, y_cubic_s, path / folder_name, "cubic_s");
    plotting_gnuplot(x, y_akima, path / folder_name, "akima");
    plotting_gnuplot(x, y_steffen, path / folder_name, "steffen");

    const std::filesystem::path parent_path = std::filesystem::canonical(__FILE__).parent_path();
    std::system((std::string("cd ") + parent_path.string() + " && mkdir data").c_str());

    const std::filesystem::path script_path = parent_path / "data";
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
