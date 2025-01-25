#include <gtest/gtest.h>

#include "../../test_utils.h"
#include "interp.h"

constexpr auto nan_v = interp::utils::nan<double>;

using container = std::vector<double>;

TEST(PrevInterp, Regular) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 16};
    container y(x.size());
    auto interp = interp::make_i<interp::Type1D::Prev>(xp, yp);
    interp(x, y);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(PrevInterp, Edge) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, exp{1, 4, 25};
    auto interp = interp::make_i<interp::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(PrevInterp, EqArrays) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, exp{1, 4, 9, 16, 25};
    auto interp = interp::make_i<interp::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(PrevInterp, UnorderedX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{4.5, 1, 3}, exp{16, 1, 9};
    auto interp = interp::make_i<interp::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(PrevInterp, NanInX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, nan_v, 4.5}, exp{1, nan_v, 16};
    auto interp = interp::make_i<interp::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(PrevInterp, NanInYp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, nan_v, 16, 25}, x{1, 3, 4.5}, exp{1, nan_v, 16};
    auto interp = interp::make_i<interp::Type1D::Prev>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(PrevInterp, BoundsCheck) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{-10, 9, 10};
    auto interp = interp::make_i<interp::Type1D::Prev>( xp, yp,{ .bounds_check = true, .bounds = {-10, 10} });
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(PrevInterp, Extrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::Prev>(xp, yp, { .extrapolate = true });
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(PrevInterp, XpSizeNotEqYpSize) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16};
    ASSERT_THROW((interp::make_i<interp::Type1D::Prev>(xp, yp)), std::invalid_argument);
}

TEST(PrevInterp, MinNumPoints) {
    const container xp, yp;
    ASSERT_THROW((interp::make_i<interp::Type1D::Prev>(xp, yp)), std::invalid_argument);
}

TEST(PrevInterp, BoundsCheckAndExtrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25};
    ASSERT_THROW(
            (interp::make_i<interp::Type1D::Prev>(xp, yp, { .extrapolate = true, .bounds_check = true })),
            std::invalid_argument
    );
}

// next tests

TEST(NextInterp, Regular) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NextInterp, Edge) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::Next>(xp, yp);
    auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NextInterp, EqArrays) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, exp{1, 4, 9, 16, 25};
    auto interp = interp::make_i<interp::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NextInterp, UnorderedX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{4.5, 1, 3}, exp{25, 1, 9};
    auto interp = interp::make_i<interp::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NextInterp, NanInX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, nan_v, 4.5}, exp{1, nan_v, 25};
    auto interp = interp::make_i<interp::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NextInterp, NanInYp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, nan_v, 16, 25}, x{1, 3, 4.5}, exp{1, nan_v, 25};
    auto interp = interp::make_i<interp::Type1D::Next>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NextInterp, BoundsCheck) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{-10, 9, 10};
    auto interp = interp::make_i<interp::Type1D::Next>(xp, yp,{ .bounds_check = true, .bounds = {-10, 10} });
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NextInterp, Extrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::Next>(xp, yp, { .extrapolate = true });
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NextInterp, XpSizeNotEqYpSize) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16};
    ASSERT_THROW((interp::make_i<interp::Type1D::Next>(xp, yp)), std::invalid_argument);
}

TEST(NextInterp, MinNumPoints) {
    const container xp, yp;
    ASSERT_THROW((interp::make_i<interp::Type1D::Next>(xp, yp)), std::invalid_argument);
}

TEST(NextInterp, BoundsCheckAndExtrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25};
    ASSERT_THROW(
            (interp::make_i<interp::Type1D::Next>(xp, yp, { .extrapolate = true, .bounds_check = true })),
            std::invalid_argument
    );
}

// nearest_neighbour tests

TEST(NearestNeighbourInterp, Regular) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NearestNeighbourInterp, Edge) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NearestNeighbourInterp, EqArrays) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, exp{1, 4, 9, 16, 25};
    auto interp = interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NearestNeighbourInterp, UnorderedX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{4.5, 1, 3}, exp{25, 1, 9};
    auto interp = interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NearestNeighbourInterp, NanInX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, nan_v, 4.5}, exp{1, nan_v, 25};
    auto interp = interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NearestNeighbourInterp, NanInYp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, nan_v, 16, 25}, x{1, 3, 4.5}, exp{1, nan_v, 25};
    auto interp = interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NearestNeighbourInterp, BoundsCheck) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{-10, 9, 10};
    auto interp = interp::make_i<interp::Type1D::NearestNeighbour>(
            xp, yp, { .bounds_check = true, .bounds = {-10, 10} }
    );
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NearestNeighbourInterp, Extrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp, { .extrapolate = true });
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(NearestNeighbourInterp, XpSizeNotEqYpSize) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16};
    ASSERT_THROW((interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp)), std::invalid_argument);
}

TEST(NearestNeighbourInterp, MinNumPoints) {
    const container xp, yp;
    ASSERT_THROW((interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp)), std::invalid_argument);
}

TEST(NearestNeighbourInterp, BoundsCheckAndExtrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25};
    ASSERT_THROW(
            (interp::make_i<interp::Type1D::NearestNeighbour>(xp, yp, { .extrapolate = true, .bounds_check = true })),
            std::invalid_argument
    );
}

// linear tests

TEST(LinearInterp, Regular) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 20.5};
    auto interp = interp::make_i<interp::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(LinearInterp, Edge) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2.5, 5}, exp{1, 6.5, 25};
    auto interp = interp::make_i<interp::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(LinearInterp, EqArrays) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, exp{1, 4, 9, 16, 25};
    auto interp = interp::make_i<interp::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(LinearInterp, UnorderedX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{4.5, 1, 3}, exp{20.5, 1, 9};
    auto interp = interp::make_i<interp::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(LinearInterp, NanInX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, nan_v, 4.5}, exp{1, nan_v, 20.5};
    auto interp = interp::make_i<interp::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(LinearInterp, NanInYp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, nan_v, 16, 25}, x{1, 3, 4.5}, exp{1, nan_v, 20.5};
    auto interp = interp::make_i<interp::Type1D::Linear>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(LinearInterp, BoundsCheck) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{-10, 9, 10};
    auto interp = interp::make_i<interp::Type1D::Linear>(
            xp, yp,{ .bounds_check = true, .bounds = {-10, 10} }
    );
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(LinearInterp, Extrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{-2, 9, 34};
    auto interp = interp::make_i<interp::Type1D::Linear>(xp, yp, { .extrapolate = true });
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(LinearInterp, XpSizeNotEqYpSize) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16};
    ASSERT_THROW((interp::make_i<interp::Type1D::Linear>(xp, yp)), std::invalid_argument);
}

TEST(LinearInterp, MinNumPoints) {
    const container xp{1}, yp{1};
    ASSERT_THROW((interp::make_i<interp::Type1D::Linear>(xp, yp)), std::invalid_argument);
}

TEST(LinearInterp, BoundsCheckAndExtrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25};
    ASSERT_THROW(
            (interp::make_i<interp::Type1D::Linear>(xp, yp, { .extrapolate = true, .bounds_check = true })),
            std::invalid_argument
    );
}

// quadratic tests

TEST(QuadraticInterp, Regular) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 20.25};
    auto interp = interp::make_i<interp::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(QuadraticInterp, Edge) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 5}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(QuadraticInterp, EqArrays) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, exp{1, 4, 9, 16, 25};
    auto interp = interp::make_i<interp::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(QuadraticInterp, UnorderedX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{4.5, 1, 3}, exp{20.25, 1, 9};
    auto interp = interp::make_i<interp::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(QuadraticInterp, NanInX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, nan_v, 4.5}, exp{1, nan_v, 20.25};
    auto interp = interp::make_i<interp::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(QuadraticInterp, NanInYp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, nan_v, 16, 25}, x{1, 3, 4.5}, exp{1, nan_v, nan_v};
    auto interp = interp::make_i<interp::Type1D::Quadratic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(QuadraticInterp, BoundsCheck) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{-10, 9, 10};
    auto interp = interp::make_i<interp::Type1D::Quadratic>(
            xp, yp,{ .bounds_check = true, .bounds = {-10, 10} }
    );
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(QuadraticInterp, Extrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{0, 9, 36};
    auto interp = interp::make_i<interp::Type1D::Quadratic>(xp, yp, {.extrapolate = true});
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(QuadraticInterp, XpSizeNotEqYpSize) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16};
    ASSERT_THROW((interp::make_i<interp::Type1D::Quadratic>(xp, yp)), std::invalid_argument);
}

TEST(QuadraticInterp, MinNumPoints) {
    const container xp{1, 2}, yp{1, 4};
    ASSERT_THROW((interp::make_i<interp::Type1D::Quadratic>(xp, yp)), std::invalid_argument);
}

TEST(QuadraticInterp, BoundsCheckAndExtrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25};
    ASSERT_THROW(
            (interp::make_i<interp::Type1D::Quadratic>(xp, yp, { .extrapolate = true, .bounds_check = true })),
            std::invalid_argument
    );
}

// cubic tests

TEST(CubicInterp, Regular) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 20.25};
    auto interp = interp::make_i<interp::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(CubicInterp, Edge) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 5}, exp{1, 9, 25};
    auto interp = interp::make_i<interp::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(CubicInterp, EqArrays) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 2, 3, 4, 5}, exp{1, 4, 9, 16, 25};
    auto interp = interp::make_i<interp::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(CubicInterp, UnorderedX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{4.5, 1, 3}, exp{20.25, 1, 9};
    auto interp = interp::make_i<interp::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(CubicInterp, NanInX) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, nan_v, 4.5}, exp{1, nan_v, 20.25};
    auto interp = interp::make_i<interp::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(CubicInterp, NanInYp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, nan_v, 16, 25}, x{1, 3, 4.5}, exp{1, nan_v, nan_v};
    auto interp = interp::make_i<interp::Type1D::Cubic>(xp, yp);
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(CubicInterp, BoundsCheck) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{-10, 9, 10};
    auto interp = interp::make_i<interp::Type1D::Cubic>(
            xp, yp,{ .bounds_check = true, .bounds = {-10, 10} }
    );
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(CubicInterp, Extrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{0, 3, 6}, exp{0, 9, 36};
    auto interp = interp::make_i<interp::Type1D::Cubic>(xp, yp, { .extrapolate = true });
    const auto y = interp(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(CubicInterp, XpSizeNotEqYpSize) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16};
    ASSERT_THROW((interp::make_i<interp::Type1D::Cubic>(xp, yp)), std::invalid_argument);
}

TEST(CubicInterp, MinNumPoints) {
    const container xp{1, 2, 3}, yp{1, 4, 9};
    ASSERT_THROW((interp::make_i<interp::Type1D::Cubic>(xp, yp)), std::invalid_argument);
}

TEST(CubicInterp, BoundsCheckAndExtrapolate) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25};
    ASSERT_THROW(
            (interp::make_i<interp::Type1D::Cubic>(xp, yp, { .extrapolate = true, .bounds_check = true })),
            std::invalid_argument
    );
}

TEST(AnyInterp, SyntaxTest) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5};
    const container exp_lin{1, 9, 20.5}, exp_quad{1, 9, 20.25}, exp_cub{1, 9, 20.25};
    interp::any_i interp = interp::make_i<interp::Type1D::Linear>(xp, yp);
    const auto y_lin = interp(x);
    ASSERT_TRUE(arrays_eq(y_lin, exp_lin));

    interp = interp::make_i<interp::Type1D::Quadratic>(xp, yp);
    const auto y_quad = interp(x);
    ASSERT_TRUE(arrays_eq(y_quad, exp_quad));

    interp = interp::make_i<interp::Type1D::Cubic>(xp.cbegin(), xp.cend(), yp.cbegin(), yp.cend());
    container y_cub(exp_cub.size());
    interp(x.cbegin(), x.cend(), y_cub.begin());
    ASSERT_TRUE(arrays_eq(y_cub, exp_cub));
}

int main(int argc, char **argv) {
    std::cout << "Test 'accuracy 1d interps' starts in ";
#ifndef NDEBUG
    std::cout << "Debug build ";
#else
    std::cout << "Release build ";
#endif
    std::cout << "with Address, Undefined and Leak sanitizers\n";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}