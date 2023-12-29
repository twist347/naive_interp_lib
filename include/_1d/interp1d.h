#pragma once

#include <stdexcept>
#include <interp1d_base.h>
#include <interp1d_types.h>
#include <interp1d_algs.h>

namespace ni::_1d {

    template<Type1D type, class Container>
    class i_1d : public i_1d_base<Container> {
    public:
        using container_type = i_1d_base<Container>::container_type;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

        constexpr i_1d(const container_type &xp, const container_type &yp) {
            init(xp, yp);
        }

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_1d)

        constexpr auto operator()(const container_type &x) const -> container_type override {
            switch (type) {
                case Type1D::Prev:
                    return detail::prev(x, xp_, yp_);
                case Type1D::Next:
                    return detail::next(x, xp_, yp_);
                case Type1D::NearestNeighbour:
                    return detail::nearest_neighbour(x, xp_, yp_);
                case Type1D::Linear:
                    return detail::linear(x, xp_, yp_);
                case Type1D::Quadratic:
                    return detail::quadratic(x, xp_, yp_);
                case Type1D::Cubic:
                    return detail::cubic(x, xp_, yp_);
                case Type1D::CubicSpline:
                    return detail::cubic_spline(x, xp_, yp_);
                case Type1D::Akima:
                    return detail::akima(x, xp_, yp_);
                case Type1D::Steffen:
                    return detail::steffen(x, xp_, yp_);
                default:
                    throw std::invalid_argument("unknown 1d interpolation type");
            }
        }

    private:
        constexpr void init(const container_type &xp, const container_type &yp) {
            if (xp.size() != yp.size()) {
                throw std::invalid_argument("size of xp must be equal to yp size");
            }

            if (xp.size() < min_num_points) {
                throw std::invalid_argument(
                        "the number of points must be at least " + std::to_string(min_num_points)
                );
            }
            xp_ = xp;
            yp_ = yp;
        }

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

        container_type xp_;
        container_type yp_;
    };
}