#include <interp.h>
#include "../../test_utils.h"

// LINUX ONLY PLOTTING VIA GNUPLOT

int main() {
#ifdef __linux
    using namespace plotting::_2d;
    namespace fs = std::filesystem;

    const std::string folder_name = "data";
    const std::string script_name = "script";

    const auto data_path = fs::canonical(__FILE__).parent_path() / folder_name;

    if (!fs::exists(data_path)) {
        fs::create_directory(data_path);
    }

    double (*func)(double, double) = plotting::surfaces::sin_cos;
    constexpr int count = 70; // count * count
    constexpr double x_min = -10.0, x_max = 10.0, y_min = -10.0, y_max = 10.0;
    const auto [xp, yp, zp, x, y] = generate_vals(func, count, x_min, x_max, y_min, y_max);

    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << ", y: " << x.size() << ", z: " << y.size() << '\n';

    const auto z_idw = ni::do_i<ni::Type2DScat::IDW>(x, y, xp, yp, zp);
    const auto z_nn = ni::do_i<ni::Type2DScat::NearestNeighbour>(x, y, xp, yp, zp);
    const auto z_tin = ni::do_i<ni::Type2DScat::TIN>(x, y, xp, yp, zp);

    plotting_gnuplot(xp, yp, zp, data_path, "orig");
    plotting_gnuplot(x, y, z_idw, data_path, "idw");
    plotting_gnuplot(x, y, z_nn, data_path, "nn");
    plotting_gnuplot(x, y, z_tin, data_path, "tin");

    // nans test
    std::vector<double> zp_nans(zp.begin(), zp.end());
    for (std::size_t i = 0; i < zp.size(); i += 2) {
        zp_nans[i] = ni::utils::nan<double>;
    }
    const auto z_idw_n = ni::do_i<ni::Type2DScat::IDW>(x, y, xp, yp,zp_nans);
    const auto z_nn_n = ni::do_i<ni::Type2DScat::NearestNeighbour>(x, y, xp, yp,zp_nans);
    const auto z_tin_n = ni::do_i<ni::Type2DScat::TIN>(x, y, xp, yp,zp_nans);

    plotting_gnuplot(x, y, z_idw_n, data_path, "idw_n");
    plotting_gnuplot(x, y, z_nn_n, data_path, "nn_n");
    plotting_gnuplot(x, y, z_tin_n, data_path, "tin_n");

    // nans test end

    fs::path script_path = data_path / script_name;
    std::ofstream script(script_path);

    if (script.is_open()) {
        script << "set size ratio -1\n"
                  "set xzeroaxis\n"
                  "set yzeroaxis\n"
                  "set zzeroaxis\n"
                  "splot 'orig' with points"
                  ", 'idw' with points"
                  ", 'nn' with points"
                  ", 'tin' with points"
                  ", 'idw_n' with points"
                  ", 'nn_n' with points"
                  ", 'tin_n' with points"
                  "\n pause -1";
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