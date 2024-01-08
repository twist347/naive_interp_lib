#pragma once

#include <stdexcept>
#include <interp1d_base.h>
#include <interp1d_types.h>
#include <interp1d_algs.h>

namespace ni::_1d::impl {

    template<Type1D type, class Container>
    class i_1d : public i_1d_base<Container> {
    private:
        using base_t = i_1d_base<Container>;
    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

        constexpr i_1d(const container_type &xp, const container_type &yp) : xp_(xp), yp_(yp) {
            if (xp.size() != yp.size()) {
                std::cerr << "size of xp must be equal to yp size\n";
                std::terminate();
            }

            if (xp.size() < min_num_points) {
                std::cerr << "the number of points must be at least " + std::to_string(min_num_points) << '\n';
                std::terminate();
            }
        }

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_1d)

        constexpr auto operator()(const container_type &x) const -> container_type override {
            switch (type) {
                case Type1D::Prev:
                    return impl::prev(x, xp_, yp_);
                case Type1D::Next:
                    return impl::next(x, xp_, yp_);
                case Type1D::NearestNeighbour:
                    return impl::nearest_neighbour(x, xp_, yp_);
                case Type1D::Linear:
                    return impl::linear(x, xp_, yp_);
                case Type1D::Quadratic:
                    return impl::quadratic(x, xp_, yp_);
                case Type1D::Cubic:
                    return impl::cubic(x, xp_, yp_);
                case Type1D::CubicSpline:
                    return impl::cubic_spline(x, xp_, yp_);
                case Type1D::Akima:
                    return impl::akima(x, xp_, yp_);
                case Type1D::Steffen:
                    return impl::steffen(x, xp_, yp_);
                default:
                    throw std::invalid_argument("unknown 1d interpolation type");
            }
        }

    private:
        constexpr static auto min_points_extr() -> int {
            switch (type) {
                case Type1D::Prev:
                case Type1D::Next:
                case Type1D::NearestNeighbour:
                    return 1;
                case Type1D::Linear:
                    return 2;
                case Type1D::Quadratic:
                    return 3;
                case Type1D::Cubic:
                case Type1D::CubicSpline:
                case Type1D::Steffen:
                    return 4;
                case Type1D::Akima:
                    return 5;
                default:
                    throw std::invalid_argument("unknown 1d interpolation type");
            }
        }

        constexpr static int min_num_points = min_points_extr();

        const container_type xp_;
        const container_type yp_;
    };
}