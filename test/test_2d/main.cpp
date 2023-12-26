#include <gtest/gtest.h>
#include <interp2d.h>
#include <interp2d_algs.h>
#include <interp_make.h>
#include <vector>
#include "../test_utils.h"

TEST(IDWAlg, Base) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.6363};
    auto z = ni::_2d::idw(x, y, xp, yp, zp);
    arrays_eq(z, expected);
}

TEST(A, Base) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.6363};
    auto z = ni::_2d::i_func_scat(ni::_2d::Type2DScatter::IDW, x, y, xp, yp, zp);
    arrays_eq(z, expected);
}

TEST(IDWInterp, Base) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.6363};
    auto interp = ni::_2d::i_idw<std::vector<double>>(xp, yp, zp);
    auto z = interp(x, y);
    arrays_eq(z, expected);
}

TEST(NNAlg, Base) {
    std::vector<double> xp{1, 0, 1, 0.5, 0.25}, yp{1, 1, 0, 0.5, 0.25}, zp{10, 10, 10, 5, 2.5}, x{0.75, 0.5},
            y{0.25, 1}, expected{10, 10};
    auto z = ni::_2d::nearest_neighbour(x, y, xp, yp, zp);
    arrays_eq(z, expected);
}

TEST(NNInterp, Base) {
    std::vector<double> xp{1, 0, 1, 0.5, 0.25}, yp{1, 1, 0, 0.5, 0.25}, zp{10, 10, 10, 5, 2.5}, x{0.75, 0.5},
            y{0.25, 1}, expected{10, 10};
    auto interp = ni::_2d::i_nearest_neighbour<std::vector<double>>(xp, yp, zp);
    auto z = interp(x, y);
    arrays_eq(z, expected);
}

TEST(TINAlg, Base) {
    std::vector<double> xp{1, 0, 1, 0.5, 0.25}, yp{1, 1, 0, 0.5, 0.25}, zp{10, 10, 10, 5, 2.5}, x{0.75, 0.5},
            y{0.25, 1}, expected{7.5, 10};
    auto z = ni::_2d::tin(x, y, xp, yp, zp);
    arrays_eq(z, expected);
}

TEST(TINInterp, Base) {
    std::vector<double> xp{1, 0, 1, 0.5, 0.25}, yp{1, 1, 0, 0.5, 0.25}, zp{10, 10, 10, 5, 2.5}, x{0.75, 0.5},
            y{0.25, 1}, expected{7.5, 10};
    auto interp = ni::_2d::i_tin<std::vector<double>>(xp, yp, zp);
    auto z = interp(x, y);
    arrays_eq(z, expected);
}

TEST(MakeScat, Base) {
    std::vector<double> xp{4, 1, 5, 2, 3}, yp{5, 2, 6, 3, 4}, zp{40, 10, 50, 20, 30}, x{2.5, 3, 5}, y{3.5, 4, 5},
            expected{25, 30, 43.6363};
    auto interp = ni::_2d::make_scat<ni::_2d::Type2DScatter::IDW>(xp, yp, zp);
    auto z = interp(x, y);
    arrays_eq(z, expected);
}

TEST(Interp2D, Traits) {
    using type = std::invoke_result_t<decltype(ni::_2d::make_scat<ni::_2d::Type2DScatter::IDW,
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