#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "test_utils.h"
#include <interp1d_algs.h>
#include <interp1d.h>


TEST(PrevAlg, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 4, 16};
    auto y = naive_interp::prev(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 4, 25};
    auto y = naive_interp::prev(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{4, 1, 16};
    auto y = naive_interp::prev(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = naive_interp::prev(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 4, 16};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Prev, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 4, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Prev, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{4, 1, 16};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Prev, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Prev, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, BaseTest) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 4.5}, expected{1, 9, 25};
    auto y = naive_interp::next(x, xp, yp);
    print(y);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto y = naive_interp::next(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto y = naive_interp::next(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = naive_interp::next(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 4.5}, expected{1, 9, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Next, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Next, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Next, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Next, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 9, 25};
    auto y = naive_interp::nearest_neighbour(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto y = naive_interp::nearest_neighbour(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto y = naive_interp::nearest_neighbour(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = naive_interp::prev(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 9, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::NearestNeighbour, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::NearestNeighbour, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::NearestNeighbour, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::NearestNeighbour, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.5, 20.5};
    auto y = naive_interp::linear(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.5, 25};
    auto y = naive_interp::linear(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.5, 1, 20.5};
    auto y = naive_interp::linear(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearAlg, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto y = naive_interp::linear(x, xp, yp);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Base) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.5, 20.5};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Linear, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Edge) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.5, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Linear, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Unordered) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.5, 1, 20.5};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Linear, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, EqArrays) {
    std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = naive_interp::_1d::i_1d<naive_interp::_1d::Type1D::Linear, std::vector<double>>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}