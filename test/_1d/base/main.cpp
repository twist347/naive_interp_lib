#include <gtest/gtest.h>
#include <interp_make.h>
#include <test_utils.h>

constexpr auto nan_val = ni::utils::nan<double>;

TEST(PrevInterp, Base) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 4, 16};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, Edge) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 4, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, Unordered) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{4, 1, 16};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, EqArrays) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, NaninX) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 16};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(PrevInterp, NaninYp) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 16};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Base) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 4.5}, expected{1, 9, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Edge) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, Unordered) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, EqArrays) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, NaninX) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NextInterp, NaninYp) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan_val, 16, 25}, x{2, 2.5, 3.5}, expected{4, nan_val, 16};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, Edge) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 9, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, Unordered) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{9, 1, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, EqArrays) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, NaninX) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(NearestNeighbourInterp, NaninYp) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan_val, 16, 25}, x{2, 2.5, 3.5}, expected{4, nan_val, 16};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Base) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.5, 20.5};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Edge) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.5, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, Unordered) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.5, 1, 20.5};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, EqArrays) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, NaninX) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 20.5};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(LinearInterp, NaninYp) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan_val, 16, 25}, x{1, 2.5, 4.5}, expected{1, nan_val, 20.5};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, Base) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.25, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, Edge) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.25, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, Unordered) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.25, 1, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, EqArrays) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, NaninX) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(QuadraticInterp, NaninYp) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan_val, 16, 25}, x{1, 4.5, 5}, expected{nan_val, nan_val, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, Base) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.25, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, Edge) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.25, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, Unordered) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.25, 1, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, EqArrays) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, NaninX) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicInterp, NaninYp) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan_val, 16, 25}, x{1, 4.5, 5}, expected{nan_val, nan_val, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicSplineInterp, Base) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.23214, 20.33929};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected, 1e-5));
}

TEST(CubicSplineInterp, Edge) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.23214, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected, 1e-5));
}

TEST(CubicSplineInterp, Unordered) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.23214, 1, 20.33929};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected, 1e-5));
}

TEST(CubicSplineInterp, EqArrays) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(CubicSplineInterp, NaninX) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 20.33929};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected, 1e-5));
}

TEST(CubicSplineInterp, NaninYp) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, nan_val, 16, 25}, x{1, 4.5, 5}, expected{nan_val, nan_val, nan_val};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::CubicSpline>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected, 1e-5));
}

TEST(AkimaInterp, Base) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, 2.5, 4.5}, expected{4, 6.25, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(AkimaInterp, Edge) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, expected{1, 6.25, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(AkimaInterp, Unordered) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2.5, 1, 4.5}, expected{6.25, 1, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(AkimaInterp, EqArrays) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, expected{1, 4, 9, 16, 25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(AkimaInterp, NaninX) {
    const std::vector<double> xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{2, nan_val, 4.5}, expected{4, nan_val, 20.25};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(AkimaInterp, NaninYp) {
    const std::vector<double> xp{1, 2, 3, 4, 5, 6}, yp{1, 4, nan_val, 16, 25, 36}, x{1, 4.5, 5, 6}, expected{nan_val, nan_val,
                                                                                                       nan_val,
                                                                                                       nan_val};
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Akima>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, expected));
}

TEST(Interp1D, Traits) {
    using type =
            std::invoke_result_t<decltype(ni::_1d::make_i<ni::_1d::Type1D::Linear,
                    std::vector<double>, std::vector<double>>), std::vector<double>, std::vector<double>>;
    static_assert(std::is_same_v<type::container_type, std::vector<double>>);
    static_assert(std::is_same_v<type::value_type, double>);
    static_assert(std::is_same_v<type::size_type, std::size_t>);
    ASSERT_TRUE(1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}