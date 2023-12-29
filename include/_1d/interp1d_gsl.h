#pragma once

#include <stdexcept>
#include <memory>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_interp.h>
#include <interp1d_base.h>

namespace ni::_1d::detail {
    enum class TypeGSL {
        Linear,
        CubicSpline,
        Akima,
        Steffen
    };

    auto gsl_spline_alloc_type(TypeGSL type) {
        switch (type) {
            case TypeGSL::Linear:
                return gsl_interp_linear;
            case TypeGSL::CubicSpline:
                return gsl_interp_cspline;
            case TypeGSL::Akima:
                return gsl_interp_akima;
            case TypeGSL::Steffen:
                return gsl_interp_steffen;
            default:
                throw std::invalid_argument("unknown gsl interp type");
        }
    }

    template<TypeGSL type, class Container>
    class i_gsl : public ni::_1d::i_1d_base<Container> {
    public:
        using container_type = std::remove_cvref_t<Container>;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

        constexpr i_gsl(const container_type &xp, const container_type &yp) :
                interp_(gsl_spline_alloc(gsl_spline_alloc_type(type), xp.size()), gsl_spline_free) {
            init(xp, yp);
            gsl_spline_init(interp_.get(), xp_.data(), yp_.data(), xp_.size());
        }

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_gsl)

        constexpr virtual auto operator()(const container_type &x) const -> container_type {
            container_type y(x.size());
            const auto raw_ptr_interp = interp_.get();
#pragma omp parallel for firstprivate(raw_ptr_interp) schedule(guided)
            for (size_type i = 0; i < y.size(); ++i) {
                y[i] = gsl_spline_eval(raw_ptr_interp, x[i], nullptr);
            }
            return y;
        }

    private:
        constexpr void init(const container_type &xp, const container_type &yp) {
            if (xp.size() != yp.size()) {
                throw std::invalid_argument("size of xp must be equal to yp size");
            }

            if (xp.size() < min_points_gsl()) {
                throw std::invalid_argument(
                        "the number of points must be at least " + std::to_string(min_points_gsl())
                );
            }
            xp_ = xp;
            yp_ = yp;
        }

        constexpr static auto min_points_gsl() -> int {
            switch (type) {
                case TypeGSL::Linear:
                    return 2;
                case TypeGSL::CubicSpline:
                case TypeGSL::Steffen:
                    return 4;
                case TypeGSL::Akima:
                    return 5;
                default:
                    throw std::invalid_argument("unknown gsl interpolation type");
            }
        }

        container_type xp_;
        container_type yp_;
        std::unique_ptr<gsl_spline, decltype(&gsl_spline_free)> interp_;
    };
}