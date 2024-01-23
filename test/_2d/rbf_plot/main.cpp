#include <interp_make.h>
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

    const auto z_rbf_linear = ni::make_i<ni::Type2DRBF::Linear>(xp, yp, zp)(x, y);
    const auto z_rbf_cubic = ni::make_i<ni::Type2DRBF::Cubic>(xp, yp, zp)(x, y);
    const auto z_rbf_quintic = ni::make_i<ni::Type2DRBF::Quintic>(xp, yp, zp)(x, y);
    const auto z_rbf_m = ni::make_i<ni::Type2DRBF::Multiquadric>(xp, yp, zp)(x, y);
    const auto z_rbf_im = ni::make_i<ni::Type2DRBF::InverseMultiquadric>(xp, yp, zp)(x, y);
    const auto z_rbf_gauss = ni::make_i<ni::Type2DRBF::Gaussian>(xp, yp, zp)(x, y);
    const auto z_rbf_tp = ni::make_i<ni::Type2DRBF::ThinPlate>(xp, yp, zp)(x, y);

    plotting_gnuplot(xp, yp, zp, data_path, "orig");
    plotting_gnuplot(x, y, z_rbf_linear, data_path, "rbf_lin");
    plotting_gnuplot(x, y, z_rbf_cubic, data_path, "rbf_cub");
    plotting_gnuplot(x, y, z_rbf_quintic, data_path, "rbf_qui");
    plotting_gnuplot(x, y, z_rbf_m, data_path, "rbf_m");
    plotting_gnuplot(x, y, z_rbf_im, data_path, "rbf_im");
    plotting_gnuplot(x, y, z_rbf_gauss, data_path, "rbf_gauss");
    plotting_gnuplot(x, y, z_rbf_tp, data_path, "rbf_tp");

    // nans test
    std::vector<double> zp_nans(zp.begin(), zp.end());
    for (std::size_t i = 0; i < zp.size(); i += 2) {
        zp_nans[i] = ni::utils::nan<double>;
    }

    const auto z_rbf_lin_n = ni::make_i<ni::Type2DRBF::Linear>(xp, yp, zp_nans)(x, y);
    plotting_gnuplot(x, y, z_rbf_lin_n, data_path, "rbf_lin_n");
    // nans test end

    fs::path script_path = data_path / script_name;
    std::ofstream script(script_path);

    if (script.is_open()) {

        script << "set size ratio -1\n"
                  "set xzeroaxis\n"
                  "set yzeroaxis\n"
                  "set zzeroaxis\n"
                  "splot 'orig' with points"
                  ", 'rbf_lin' with points"
                  ", 'rbf_cub' with points"
                  ", 'rbf_qui' with points"
                  ", 'rbf_m' with points"
                  ", 'rbf_im' with points"
                  ", 'rbf_tp' with points"
                  ", 'rbf_lin_n' with points"
                  ", 'rbf_gauss' with points\n"
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
