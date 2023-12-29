#pragma once

#include <Eigen/Dense>

#include <interp2d_base.h>
#include <interp2d_types.h>
#include <utils.h>

namespace ni::_2d::detail {

    namespace kernel {

        template<class Value>
        auto dist2D(Value x1, Value x2, Value y1, Value y2) -> Value {
            return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
        }

        template<class Value>
        auto linear_k(Value x1, Value x2, Value y1, Value y2) -> Value {
            return dist2D(x1, x2, y1, y2);
        }

        template<class Value>
        auto gauss_k(Value x1, Value x2, Value y1, Value y2) -> Value {
            const auto r = dist2D(x1, x2, y1, y2);
            return std::exp(-std::pow(r, 2));
        }
    }

    template<Type2DRBF type, class Container>
    class i_rbf : public i_2d_base<Container> {
    public:
        using container_type = i_2d_base<Container>::container_type;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

        constexpr i_rbf(const container_type &xp, const container_type &yp, const container_type &zp) : xp_(xp),
                                                                                                        yp_(yp),
                                                                                                        zp_(zp) {
            const auto N = xp_.size();
            Eigen::MatrixX<value_type> phi_m(N, N);
#pragma omp parallel for simd collapse(2)
            for (size_type i = 0; i < N; ++i) {
                for (size_type j = i; j < N; ++j) {
                    const auto val = rbf_func_(xp[i], xp[j], yp[i], yp[j]);
                    phi_m(i, j) = val;
                    phi_m(j, i) = val;
                }
            }

            const Eigen::Map<Eigen::MatrixX<value_type>> zp_m(zp_.data(), zp_.size(), 1);
            // TODO: HouseholderQR is very slow
            weights_ = Eigen::HouseholderQR<Eigen::MatrixX<value_type>>(phi_m).solve(zp_m);
        }

        constexpr auto operator()(const container_type &x, const container_type &y) const -> container_type override {
            const auto N = x.size();
            const auto M = weights_.size();
            container_type z(N);
#pragma omp parallel for simd schedule(guided)
            for (size_type i = 0; i < N; ++i) {
                value_type val = utils::to<value_type>(0.0);
                for (size_type j = 0; j < M; ++j) {
                    val += weights_[j] * rbf_func_(x[i], xp_[j], y[i], yp_[j]);
                }
                z[i] = val;
            }
            return z;
        }

    private:
        constexpr auto rbf_func_(value_type x1, value_type x2, value_type y1, value_type y2) const {
            if constexpr (type == Type2DRBF::Linear) {
                return kernel::linear_k(x1, x2, y1, y2);
            } else if constexpr (type == Type2DRBF::Gauss) {
                return kernel::gauss_k(x1, x2, y1, y2);
            } else {
                throw std::invalid_argument("unknown rbf interpolation type");
            }
        }

        container_type xp_;
        container_type yp_;
        container_type zp_;
        Eigen::VectorX<value_type> weights_;
    };
}