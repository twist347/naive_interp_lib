#include <gtest/gtest.h>

#include "../../test_utils.h"
#include "interp.h"

constexpr auto nan_v = interp::utils::nan<double>;

using container = std::vector<double>;

TEST(AnyInterp, FakeInterp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 16};
    interp::any_i<double> any;
    ASSERT_THROW(any(x), std::logic_error);
}

TEST(AnyInterp, ContainersInit) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 16};
    const interp::any_i any = interp::make_i<interp::Type1D::Prev>(xp, yp);
    const auto y = any(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(AnyInterp, ItersInit) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 16};
    interp::any_i<double> any;
    any = interp::make_i<interp::Type1D::Prev>(xp.begin(), xp.end(), yp.begin());
    auto y = any(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(AnyInterp, ContainersInterp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 20.5};
    interp::any_i any = interp::make_i<interp::Type1D::Linear>(xp, yp);
    auto y = any(x);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(AnyInterp, ContainersDestInterp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 20.5};
    container y(std::size(x));
    const interp::any_i any = interp::make_i<interp::Type1D::Linear>(xp, yp);
    any(x, y);
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(AnyInterp, ItersInterp) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp{1, 9, 20.5};
    container y(std::size(x));
    const interp::any_i any = interp::make_i<interp::Type1D::Linear>(xp, yp);
    any(x.begin(), x.end(), y.begin());
    ASSERT_TRUE(arrays_eq(y, exp));
}

TEST(AnyInterp, CopySemantics) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp_prev{1, 9, 16}, exp_lin{1, 9, 20.5};
    interp::any_i any = interp::make_i<interp::Type1D::Prev>(xp, yp);
    auto y = any(x);
    ASSERT_TRUE(arrays_eq(y, exp_prev));

    any = interp::make_i<interp::Type1D::Linear>(xp, yp);
    y = any(x);
    ASSERT_TRUE(arrays_eq(y, exp_lin));
}

TEST(AnyInterp, MoveSemantics) {
    const container xp{1, 2, 3, 4, 5}, yp{1, 4, 9, 16, 25}, x{1, 3, 4.5}, exp_prev{1, 9, 16}, exp_lin{1, 9, 20.5};
    interp::i_1d<interp::Type1D::Prev, double> i1 = interp::make_i<interp::Type1D::Prev>(xp, yp);
    interp::any_i any = std::move(i1);
    auto y = any(x);
    ASSERT_TRUE(arrays_eq(y, exp_prev));

    interp::i_1d<interp::Type1D::Linear, double> i2 = interp::make_i<interp::Type1D::Linear>(xp, yp);
    any = std::move(i2);
    y = any(x);
    ASSERT_TRUE(arrays_eq(y, exp_lin));
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