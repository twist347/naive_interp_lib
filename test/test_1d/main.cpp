#include <gtest/gtest.h>
#include <vector>
#include "../test_utils.h"
#include <interp1d_algs.h>
#include <interp1d.h>
#include <interp_make.h>

TEST(PrevAlg, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 4, 16};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Prev>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 4, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Prev>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{4, 1, 16};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Prev>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Prev>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevAlg, NaninX) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan, 4.5}, expected{4, nan, 16};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Prev>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevAlg, NaninYp) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan, 16, 25}, x{2, 2.5, 3.5}, expected{4, 4, nan};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Prev>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 4, 16};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Prev, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 4, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Prev, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{4, 1, 16};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Prev, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Prev, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, BaseTest) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 4.5}, expected{1, 9, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Next>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Next>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Next>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Next>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, NaninX) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan, 4.5}, expected{4, nan, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Next>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, NaninYp) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan, 16, 25}, x{2, 2.5, 3.5}, expected{4, nan, 16};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Next>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 4.5}, expected{1, 9, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Next, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Next, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Next, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Next, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 9, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::NearestNeighbour>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::NearestNeighbour>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::NearestNeighbour>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::NearestNeighbour>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, NaninX) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan, 4.5}, expected{4, nan, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::NearestNeighbour>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, NaninYp) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan, 16, 25}, x{2, 2.5, 3.5}, expected{4, nan, 16};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::NearestNeighbour>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 9, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::NearestNeighbour, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::NearestNeighbour, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::NearestNeighbour, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::NearestNeighbour, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.5, 20.5};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Linear>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.5, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Linear>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.5, 1, 20.5};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Linear>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Linear>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, NaninX) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan, 4.5}, expected{4, nan, 20.5};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Linear>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, NaninYp) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan, 16, 25}, x{1, 2.5, 4.5}, expected{1, nan, 20.5};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Linear>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.5, 20.5};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Linear, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.5, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Linear, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.5, 1, 20.5};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Linear, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Linear, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticAlg, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.25, 20.25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Quadratic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.25, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Quadratic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.25, 1, 20.25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Quadratic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Quadratic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticAlg, NaninX) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan, 4.5}, expected{4, nan, 20.25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Quadratic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticAlg, NaninYp) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan, 16, 25}, x{1, 4.5, 5}, expected{nan, nan, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Quadratic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.25, 20.25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Quadratic, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.25, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Quadratic, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.25, 1, 20.25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Quadratic, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Quadratic, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicAlg, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.25, 20.25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Cubic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.25, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Cubic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.25, 1, 20.25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Cubic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Cubic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicAlg, NaninX) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan, 4.5}, expected{4, nan, 20.25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Cubic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicAlg, NaninYp) {
    auto nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan, 16, 25}, x{1, 4.5, 5}, expected{nan, nan, 25};
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Cubic>(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.25, 20.25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Cubic, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.25, 25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Cubic, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.25, 1, 20.25};
    auto interp = ni::_1d::i_1d<ni::_1d::Type1D::Cubic, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(Make, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.5, 20.5};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(Interp1D, Traits) {
    using type =
            std::invoke_result_t<decltype(ni::_1d::make_i<ni::_1d::Type1D::Linear,
                    std::vector<double>, std::vector<double>>), std::vector<double>, std::vector<double>>;
    static_assert(std::is_same_v<type::container_type, std::vector<double>>);
    static_assert(std::is_same_v<type::value_type , double>);
    static_assert(std::is_same_v<type::size_type , std::size_t>);
    ASSERT_TRUE(1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}