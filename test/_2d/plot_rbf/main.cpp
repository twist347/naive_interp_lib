#include <interp_make.h>
#include <test_utils.h>

// LINUX ONLY PLOTTING VIA GNUPLOT

int main() {
#ifdef __linux
    using namespace plotting::_2d;

    const std::string folder_name = "data";
    const auto path = std::filesystem::canonical(__FILE__).parent_path();
    std::system((std::string("cd ") + path.string() + " && mkdir " + folder_name).c_str());

    double (*func)(double, double) = plotting::surfaces::sin_cos;
    constexpr int N = 70; // N * N
    const auto [xp, yp, zp, x, y] = generate_vals(func, N);

    std::cout << "xp: " << xp.size() << ", yp: " << yp.size() << ", zp: " << zp.size() << '\n';
    std::cout << "x: " << x.size() << ", y: " << x.size() << ", z: " << y.size() << '\n';

    const auto z_rbf_linear = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Linear>(xp, yp, zp)(x, y);
    const auto z_rbf_cubic = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Cubic>(xp, yp, zp)(x, y);
    const auto z_rbf_quintic = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Quintic>(xp, yp, zp)(x, y);
    const auto z_rbf_m = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Multiquadric>(xp, yp, zp)(x, y);
    const auto z_rbf_im = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::InverseMultiquadric>(xp, yp, zp)(x, y);
    const auto z_rbf_gauss = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Gaussian>(xp, yp, zp)(x, y);
    const auto z_rbf_tp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::ThinPlate>(xp, yp, zp)(x, y);

    plotting_gnuplot(xp, yp, zp, path / folder_name, "orig");
    plotting_gnuplot(x, y, z_rbf_linear, path / folder_name, "rbf_lin");
    plotting_gnuplot(x, y, z_rbf_cubic, path / folder_name, "rbf_cub");
    plotting_gnuplot(x, y, z_rbf_quintic, path / folder_name, "rbf_qui");
    plotting_gnuplot(x, y, z_rbf_m, path / folder_name, "rbf_m");
    plotting_gnuplot(x, y, z_rbf_im, path / folder_name, "rbf_im");
    plotting_gnuplot(x, y, z_rbf_gauss, path / folder_name, "rbf_gauss");
    plotting_gnuplot(x, y, z_rbf_tp, path / folder_name, "rbf_tp");

    // nans test
    std::vector<double> zp_nans(zp.begin(), zp.end());
    for (std::size_t i = 0; i < zp.size(); i += 2) {
        zp_nans[i] = std::numeric_limits<double>::quiet_NaN();
    }

    const auto z_rbf_lin_n = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Linear>(xp, yp, zp_nans)(x, y);
    plotting_gnuplot(x, y, z_rbf_lin_n, path / folder_name, "rbf_lin_n");
    // nans test end

    const std::filesystem::path parent_path = std::filesystem::canonical(__FILE__).parent_path();
    std::system((std::string("cd ") + parent_path.string() + " && mkdir data").c_str());

    const std::filesystem::path script_path = parent_path / "data";
    std::ofstream script(script_path / "script");
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
    std::string cmd = std::string("cd ") + script_path.string() + " && gnuplot ./script";
    std::system(cmd.c_str());
#endif
}
