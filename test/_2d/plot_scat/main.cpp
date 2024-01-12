#include <interp_make.h>
#include <test_utils.h>

// LINUX ONLY PLOTTING VIA GNUPLOT

int main() {
#ifdef __linux
    using namespace plotting::_2d;

    const std::string folder_name = "data";
    const auto path = std::filesystem::canonical(__FILE__).parent_path();
    std::system((std::string("cd ") + path.string() + " && mkdir " + folder_name).c_str());

    double (*func)(double, double) = plotting::surfaces::elliptical_paraboloid;
    constexpr int N = 70; // N * N
    const auto [xp, yp, zp, x, y] = generate_vals(func, N);

    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << ", y: " << x.size() << ", z: " << y.size() << '\n';

    const auto z_idw = ni::_2d::make_scat_i<ni::_2d::Type2DScat::IDW>(xp, yp, zp)(x, y);
    const auto z_nn = ni::_2d::make_scat_i<ni::_2d::Type2DScat::NearestNeighbour>(xp, yp, zp)(x, y);
    const auto z_tin = ni::_2d::make_scat_i<ni::_2d::Type2DScat::TIN>(xp, yp, zp)(x, y);

    plotting_gnuplot(xp, yp, zp, path / folder_name, "orig");
    plotting_gnuplot(x, y, z_idw, path / folder_name, "idw");
    plotting_gnuplot(x, y, z_nn, path / folder_name, "nn");
    plotting_gnuplot(x, y, z_tin, path / folder_name, "tin");

    // nans test
    std::vector<double> zp_nans(zp.begin(), zp.end());
    for (std::size_t i = 0; i < zp.size(); i += 2) {
        zp_nans[i] = std::numeric_limits<double>::quiet_NaN();
    }
    const auto z_idw_n = ni::_2d::make_scat_i<ni::_2d::Type2DScat::IDW>(xp, yp, zp_nans)(x, y);
    const auto z_nn_n = ni::_2d::make_scat_i<ni::_2d::Type2DScat::NearestNeighbour>(xp, yp, zp_nans)(x, y);
    const auto z_tin_n = ni::_2d::make_scat_i<ni::_2d::Type2DScat::TIN>(xp, yp, zp_nans)(x, y);

    plotting_gnuplot(x, y, z_idw_n, path / folder_name, "idw_n");
    plotting_gnuplot(x, y, z_nn_n, path / folder_name, "nn_n");
    plotting_gnuplot(x, y, z_tin_n, path / folder_name, "tin_n");

    // nans test end

    const std::filesystem::path script_path = path / folder_name;
    std::ofstream script(script_path / "script");
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
    std::string cmd = std::string("cd ") + script_path.string() + " && gnuplot ./script";
    std::system(cmd.c_str());
#endif
}