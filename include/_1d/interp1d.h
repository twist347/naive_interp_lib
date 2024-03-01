#pragma once

#include <stdexcept>
#include <iostream>

#include "interp1d_base.h"
#include "interp1d_types.h"
#include "interp1d_algs.h"

namespace ni::_1d::impl {

    template<Type1D type, class Container, bool IsCached = true>
    class i_1d : public i_1d_base<Container, IsCached> {

    private:
        using base_t = i_1d_base<Container, IsCached>;
        using cref_or_value_c_t = base_t::cref_or_val_c_t;
        using cref_type = base_t::cref_type;

    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

        constexpr i_1d(cref_type xp, cref_type yp) : xp_(xp), yp_(yp) {
            if (xp.size() != yp.size()) {
                throw std::invalid_argument("size of xp must be equal to yp size");
            }

            if (xp.size() < min_num_points) {
                throw std::invalid_argument("the number of points must be at least " + std::to_string(min_num_points));
            }
        }

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_1d)

        constexpr auto operator()(cref_type x) const -> container_type override {
            switch (type) {
                case Type1D::Prev:
                    return impl::prev<container_type>(x.begin(), x.end(),
                                                      xp_.begin(), xp_.end(),
                                                      yp_.begin(), yp_.end());
                case Type1D::Next:
                    return impl::next<container_type>(x.begin(), x.end(),
                                                      xp_.begin(), xp_.end(),
                                                      yp_.begin(), yp_.end());
                case Type1D::NearestNeighbour:
                    return impl::nearest_neighbour<container_type>(x.begin(), x.end(),
                                                                   xp_.begin(), xp_.end(),
                                                                   yp_.begin(), yp_.end());
                case Type1D::Linear:
                    return impl::linear<container_type>(x.begin(), x.end(),
                                                        xp_.begin(), xp_.end(),
                                                        yp_.begin(), yp_.end());
                case Type1D::Quadratic:
                    return impl::quadratic<container_type>(x.begin(), x.end(),
                                                           xp_.begin(), xp_.end(),
                                                           yp_.begin(), yp_.end());
                case Type1D::Cubic:
                    return impl::cubic<container_type>(x.begin(), x.end(),
                                                       xp_.begin(), xp_.end(),
                                                       yp_.begin(), yp_.end());
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

        cref_or_value_c_t xp_;
        cref_or_value_c_t yp_;
    };

}