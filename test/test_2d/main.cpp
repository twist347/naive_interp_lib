#include <gtest/gtest.h>
#include <interp2d_scat.h>
#include <interp_make.h>
#include <vector>
#include <test_utils.h>

TEST(IDWInterp, Base) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::IDW>(xp, yp, zp);
    auto z = interp(x, y);
    ASSERT_TRUE(arrays_eq(z, expected, 1e-5));
}

TEST(NNInterp, Base) {
    std::vector<double> xp{1, 0, 1, 0.5, 0.25}, yp{1, 1, 0, 0.5, 0.25}, zp{10, 10, 10, 5, 2.5}, x{0.75, 0.5},
            y{0.25, 1}, expected{10, 10};
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::NearestNeighbour>(xp, yp, zp);
    auto z = interp(x, y);
    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(TINInterp, Base) {
    std::vector<double> xp{1, 0, 1, 0.5, 0.25}, yp{1, 1, 0, 0.5, 0.25}, zp{10, 10, 10, 5, 2.5}, x{0.75, 0.5},
            y{0.25, 1}, expected{7.5, 10};
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::TIN>(xp, yp, zp);
    auto z = interp(x, y);
    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Linear) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Linear>(xp, yp, zp);
    auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Cubic) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Cubic>(xp, yp, zp);
    auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Quintic) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Quintic>(xp, yp, zp);
    auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Multiquadric) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Multiquadric>(xp, yp, zp);
    auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, InverseMultiquadric) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::InverseMultiquadric>(xp, yp, zp);
    auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Gaussian) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Gaussian>(xp, yp, zp);
    auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, ThinPlate) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::ThinPlate>(xp, yp, zp);
    auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(Interp2D, Traits) {
    using type = std::invoke_result_t<decltype(ni::_2d::make_scat_i<ni::_2d::Type2DScat::IDW,
            std::vector<double>, std::vector<double>, std::vector<double>>),
            std::vector<double>, std::vector<double>, std::vector<double>>;
    static_assert(std::is_same_v<type::container_type, std::vector<double>>);
    static_assert(std::is_same_v<type::value_type, double>);
    static_assert(std::is_same_v<type::size_type, std::size_t>);
    ASSERT_TRUE(1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}