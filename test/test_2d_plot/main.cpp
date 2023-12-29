#include <filesystem>
#include <fstream>
#include <vector>
#include <random>
#include <iostream>
#include <interp_make.h>

// LINUX ONLY PLOTTING VIA GNUPLOT

template<class Value>
double elliptical_paraboloid(Value x, Value y, Value a = 1, Value b = 1, Value c = 1) {
    return ((x * x) / (a * a) + (y * y) / (b * b));
}

template<class Value>
double hyperbolic_paraboloid(Value x, Value y, Value a = 1, Value b = 1, Value c = 1) {
    return ((x * x) / (a * a) - (y * y) / (b * b));
}

template<class Value>
double ellipsoid(Value x, Value y, Value a = 4, Value b = 4, Value c = 1) {
    return std::sqrt(1 - (x * x) / (a * a) - (y * y) / (b * b)) / (c * c);
}

template<class Value>
double cone(Value x, Value y, Value a = 1, Value b = 1, Value c = 1) {
    return std::sqrt((x * x) / (a * a) + (y * y) / (b * b)) / (c * c);
}

template<class Value>
double sin_cos(Value x, Value y) {
    return std::sin(x) * std::cos(y);
}

template<class Value>
double one_sheet_hyperboloid(Value x, Value y, Value a = 1, Value b = 1, Value c = 1) {
    return std::sqrt((x * x) / (a * a) + (y * y) / (b * b) - 1) / (c * c);
}

template<class Container>
auto plotting_gnuplot(const Container &xp, const Container &yp, const Container &zp,
                      const std::string &file_name, int step = 1) -> void {
    std::filesystem::path plot_path = std::filesystem::canonical(__FILE__).parent_path() / "plot";
    std::ofstream source(plot_path / file_name);
    for (typename Container::size_type i = 0; i < xp.size(); i += step) {
        source << xp[i] << ' ' << yp[i] << ' ' << zp[i] << '\n';
    }
    source.close();
}

auto generate_vals() {
    double x_min = -10.0, x_max = 10.0;
    double y_min = -10.0, y_max = 10.0;
    int N = 50;
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
    for (std::size_t i = 0; i < x.size(); ++i) {
        x[i] = dis(gen);
        y[i] = dis(gen);
    }
    return std::tuple{xp, yp, zp, x, y};
}

int main() {
#ifdef __linux
    auto [xp, yp, zp, x, y] = generate_vals();
    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << ", y: " << x.size() << ", z: " << y.size() << '\n';

    auto z_idw = ni::_2d::make_scat_i<ni::_2d::Type2DScat::IDW>(xp, yp, zp)(x, y);
    auto z_nn = ni::_2d::make_scat_i<ni::_2d::Type2DScat::NearestNeighbour>(xp, yp, zp)(x, y);
    auto z_tin = ni::_2d::make_scat_i<ni::_2d::Type2DScat::TIN>(xp, yp, zp)(x, y);
    auto z_rbf_linear = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Linear>(xp, yp, zp)(x, y);
    auto z_rbf_gauss = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Gauss>(xp, yp, zp)(x, y);

    plotting_gnuplot(xp, yp, zp, "orig");
    plotting_gnuplot(x, y, z_idw, "idw");
    plotting_gnuplot(x, y, z_nn, "nn");
    plotting_gnuplot(x, y, z_tin, "tin");
    plotting_gnuplot(x, y, z_rbf_linear, "rbf_lin");
    plotting_gnuplot(x, y, z_rbf_gauss, "rbf_gauss");

    std::filesystem::path parent_path = std::filesystem::canonical(__FILE__).parent_path();
    std::system((std::string("cd ") + parent_path.string() + " && mkdir plot").c_str());

    std::filesystem::path script_path = parent_path / "plot";
    std::ofstream script(script_path / "script");
    script << "set size ratio -1\n"
              "set xzeroaxis\n"
              "set yzeroaxis\n"
              "splot 'orig' with points, 'idw' with points, 'nn' with points, 'tin' with points, 'rbf_lin' with points, "
              "'rbf_gauss' with points\n"
              "pause -1";
    script.close();
    std::string cmd = std::string("cd ") + script_path.string() + " && gnuplot ./script";
    std::system(cmd.c_str());
#endif
}