#include <interp_make.h>
#include <test_utils.h>

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
    constexpr int N = 70; // N * N
    const auto [xp, yp, zp, x, y] = generate_vals(func, N);

    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << ", y: " << x.size() << ", z: " << y.size() << '\n';

    const auto z_idw = ni::make_i<ni::_2d::Type2DScat::IDW>(xp, yp, zp)(x, y);
    const auto z_nn = ni::make_i<ni::_2d::Type2DScat::NearestNeighbour>(xp, yp, zp)(x, y);
    const auto z_tin = ni::make_i<ni::_2d::Type2DScat::TIN>(xp, yp, zp)(x, y);

    plotting_gnuplot(xp, yp, zp, data_path, "orig");
    plotting_gnuplot(x, y, z_idw, data_path, "idw");
    plotting_gnuplot(x, y, z_nn, data_path, "nn");
    plotting_gnuplot(x, y, z_tin, data_path, "tin");

    // nans test
    std::vector<double> zp_nans(zp.begin(), zp.end());
    for (std::size_t i = 0; i < zp.size(); i += 2) {
        zp_nans[i] = ni::utils::nan<double>;
    }
    const auto z_idw_n = ni::make_i<ni::_2d::Type2DScat::IDW>(xp, yp, zp_nans)(x, y);
    const auto z_nn_n = ni::make_i<ni::_2d::Type2DScat::NearestNeighbour>(xp, yp, zp_nans)(x, y);
    const auto z_tin_n = ni::make_i<ni::_2d::Type2DScat::TIN>(xp, yp, zp_nans)(x, y);

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