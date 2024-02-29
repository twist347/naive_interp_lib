#include <gtest/gtest.h>
#include <interp.h>
#include "../../test_utils.h"

TEST(IDWInterp, Base) {
    const std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::make_i<ni::Type2DScat::IDW>(xp, yp, zp);
    const auto z = interp(x, y);
    ASSERT_TRUE(arrays_eq(z, expected, 1e-5));
}

TEST(NNInterp, Base) {
    const std::vector<double> xp{1, 0, 1, 0.5, 0.25}, yp{1, 1, 0, 0.5, 0.25}, zp{10, 10, 10, 5, 2.5}, x{0.75, 0.5},
            y{0.25, 1}, expected{10, 10};
    auto interp = ni::make_i<ni::Type2DScat::NearestNeighbour>(xp, yp, zp);
    const auto z = interp(x, y);
    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(TINInterp, Base) {
    const std::vector<double> xp{1, 0, 1, 0.5, 0.25}, yp{1, 1, 0, 0.5, 0.25}, zp{10, 10, 10, 5, 2.5}, x{0.75, 0.5},
            y{0.25, 1}, expected{7.5, 10};
    auto interp = ni::make_i<ni::Type2DScat::TIN>(xp, yp, zp);
    const auto z = interp(x, y);
    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Linear) {
    const std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::make_i<ni::Type2DRBF::Linear>(xp, yp, zp);
    const auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Cubic) {
    const std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::make_i<ni::Type2DRBF::Cubic>(xp, yp, zp);
    const auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Quintic) {
    const std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::make_i<ni::Type2DRBF::Quintic>(xp, yp, zp);
    const auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Multiquadric) {
    const std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::make_i<ni::Type2DRBF::Multiquadric>(xp, yp, zp);
    const auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, InverseMultiquadric) {
    const std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::make_i<ni::Type2DRBF::InverseMultiquadric>(xp, yp, zp);
    const auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, Gaussian) {
    const std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::make_i<ni::Type2DRBF::Gaussian>(xp, yp, zp);
    const auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(RBFInterp, ThinPlate) {
    const std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.63636};
    auto interp = ni::make_i<ni::Type2DRBF::ThinPlate>(xp, yp, zp);
    const auto z = interp(x, y);
    print(z);
//    ASSERT_TRUE(arrays_eq(z, expected));
}

TEST(Interp2D, Traits) {
    using type = std::invoke_result_t<decltype(ni::make_i<ni::Type2DScat::IDW,
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