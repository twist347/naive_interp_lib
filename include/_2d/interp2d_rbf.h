#pragma once

#include <armadillo>

#include "interp2d_base.h"
#include "interp2d_types.h"
#include "utility/utils.h"

namespace ni::_2d::impl {

#if 1

    // Pure RBF interpolator
    template<Type2DRBF type, class Container>
    class i_rbf : public i_2d_base<Container> {
    private:
        using base_t = i_2d_base<Container>;
    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

        constexpr i_rbf(const container_type &xp,
                        const container_type &yp,
                        const container_type &zp) {
            if (xp.size() != yp.size() || xp.size() != zp.size()) {
                throw std::invalid_argument("all xp, yp, zp must be the same size");
            }

            filter_nans(xp, yp, zp);

            const auto A = make_mat_for_SLAE();

            // Solve SLAE: A * weights_ = zp_. Find weights_
            if (!arma::solve(weights_, A, zp_, arma::solve_opts::no_approx + arma::solve_opts::fast)) {
                throw std::runtime_error("the SLAE has no solution. Conditions are poor in " + get_type_name());
            }
        }

        constexpr auto operator()(const container_type &x, const container_type &y) const -> container_type override {
            const auto n = x.size();
            const auto m = xp_.size();

            container_type z(n);

            for (size_type i = 0; i < n; ++i) {
                for (size_type j = 0; j < m; ++j) {
                    const value_type dx = x[i] - xp_[j];
                    const value_type dy = y[i] - yp_[j];
                    z[i] += weights_(j) * radial_func(std::sqrt(dx * dx + dy * dy));
                }
            }
            return z;
        }

    private:
        constexpr void filter_nans(const container_type &xp, const container_type &yp, const container_type &zp) {
            const auto nans_count = std::count_if(zp_.begin(), zp_.end(),
                                                  [](value_type val) { return std::isnan(val); });
            const auto sz = zp.size() - nans_count;

            xp_ = container_type(sz);
            yp_ = container_type(sz);
            zp_ = arma_vec(sz);

            for (size_type i = 0, j = 0; i < zp.size(); ++i) {
                if (!std::isnan(zp[i])) {
                    xp_[j] = xp[i];
                    yp_[j] = yp[i];
                    zp_(j) = zp[i];
                    ++j;
                }
            }
        }

        constexpr auto make_mat_for_SLAE() const {
            // TODO: check it
            // value for matrix regularization
            const value_type lambda = 0.001;

            const auto n = xp_.size();
            arma_mat A(n, n);

            for (size_type i = 0; i < n; ++i) {
                for (size_type j = i; j < n; ++j) {
                    const value_type dx = xp_[i] - xp_[j];
                    const value_type dy = yp_[i] - yp_[j];
                    const value_type val = radial_func(std::sqrt(dx * dx + dy * dy));
                    // optimization due to matrix symmetry
                    A(i, j) = val;
                    A(j, i) = val;
                }
                A(i, i) += lambda;
            }
            return A;
        }

        constexpr value_type radial_func(value_type r, value_type epsilon = 1.0) const {
            switch (type) {
                case Type2DRBF::Linear:
                    return r;
                case Type2DRBF::Cubic:
                    return r * r * r;
                case Type2DRBF::Quintic:
                    return r * r * r * r * r;
                case Type2DRBF::Multiquadric:
                    return std::sqrt(1.0 + epsilon * epsilon * r * r);
                case Type2DRBF::InverseMultiquadric:
                    return 1.0 / std::sqrt(1.0 + epsilon * epsilon * r * r);
                case Type2DRBF::Gaussian:
                    return std::exp(-epsilon * r * r);
                case Type2DRBF::ThinPlate:
                    return r * r * std::log(r);
                default:
                    throw std::invalid_argument("Unknown rbf interpolation type");
            }
        }

        constexpr std::string get_type_name() const {
            switch (type) {
                case Type2DRBF::Linear:
                    return "Linear";
                case Type2DRBF::Cubic:
                    return "Cubic";
                case Type2DRBF::Quintic:
                    return "Quintic";
                case Type2DRBF::Multiquadric:
                    return "Multiquadric";
                case Type2DRBF::InverseMultiquadric:
                    return "InverseMultiquadric";
                case Type2DRBF::Gaussian:
                    return "Gaussian";
                case Type2DRBF::ThinPlate:
                    return "ThinPlate";
                default:
                    throw std::invalid_argument("Unknown rbf interpolation type");
            }
        }

        using arma_vec = arma::Col<value_type>; // arma::vec is arma::Col<double>
        using arma_mat = arma::Mat<value_type>; // arma::mat is arma::Mat<double>

        container_type xp_;
        container_type yp_;
        arma_vec zp_;
        arma_vec weights_;
    };

#endif

#if 0

    // RBF (With Adding Polynomial Term)
    template<Type2DRBF type, class Container>
    class i_rbf : public i_2d_base<Container> {
    public:
        using container_type = i_2d_base<Container>::container_type;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

        constexpr i_rbf(const container_type &xp,
                        const container_type &yp,
                        const container_type &zp,
                        int poly_degree = 2) : xp_(xp),
                                               yp_(yp),
                                               zp_(arma_vec(zp.data(), zp.size()).eval()),
                                               poly_degree_(poly_degree) {
            // TODO: check it
            // value for matrix regularization
            const value_type lambda = 0.001;

            const auto n = xp_.size();
            const size_type poly_terms = (poly_degree_ + 1) * (poly_degree_ + 2) / 2;
            const size_type augmented_size = n + poly_terms;

            arma_mat A(augmented_size, augmented_size, arma::fill::zeros);

            // RBF part
            for (size_type i = 0; i < n; ++i) {
                for (size_type j = i; j < n; ++j) {
                    const value_type dx = xp_[i] - xp_[j];
                    const value_type dy = yp_[i] - yp_[j];
                    const value_type val = radial_func(std::sqrt(dx * dx + dy * dy));
                    A(i, j) = val;
                    A(j, i) = val;
                }
                A(i, i) += lambda;
            }

            // polynomial part
            arma_vec poly_zp = arma::zeros<arma_vec>(augmented_size);
            poly_zp(arma::span(0, n - 1)) = zp_;

            for (size_type i = 0; i < n; ++i) {
                size_type idx = n;
                for (int px = 0; px <= poly_degree_; ++px) {
                    for (int py = 0; py <= poly_degree_ - px; ++py) {
                        A(i, idx) = std::pow(xp_[i], px) * std::pow(yp_[i], py);
                        A(idx, i) = A(i, idx);
                        ++idx;
                    }
                }
            }

            // solve A * weights_ = poly_zp, find weights_
            if (!arma::solve(weights_, A, poly_zp, arma::solve_opts::no_approx + arma::solve_opts::fast)) {
                throw std::runtime_error("the SLAE has no solution. Conditions are poor in " + get_type_name());
            }
        }

        constexpr auto operator()(const container_type &x, const container_type &y) const -> container_type override {
            const auto n = x.size();
            const size_type m = xp_.size();
            const size_type poly_terms = (poly_degree_ + 1) * (poly_degree_ + 2) / 2;
            const size_type augmented_size = m + poly_terms;

            arma_vec z_arma(n);

            for (size_type i = 0; i < n; ++i) {
                for (size_type j = 0; j < m; ++j) {
                    const value_type dx = x[i] - xp_[j];
                    const value_type dy = y[i] - yp_[j];
                    z_arma(i) += weights_(j) * radial_func(std::sqrt(dx * dx + dy * dy));
                }
                size_type idx = m;
                for (int px = 0; px <= poly_degree_; ++px) {
                    for (int py = 0; py <= poly_degree_ - px; ++py) {
                        z_arma(i) += weights_(idx) * std::pow(x[i], px) * std::pow(y[i], py);
                        ++idx;
                    }
                }
            }

            // copy (from arma to container_type)
            return container_type{z_arma.memptr(), z_arma.memptr() + z_arma.n_elem};
        }

    private:
        constexpr value_type radial_func(value_type r, value_type epsilon = 1.0) const {
            switch (type) {
                case Type2DRBF::Linear:
                    return r;
                case Type2DRBF::Cubic:
                    return r * r * r;
                case Type2DRBF::Quintic:
                    return r * r * r * r * r;
                case Type2DRBF::Multiquadric:
                    return std::sqrt(1.0 + epsilon * epsilon * r * r);
                case Type2DRBF::InverseMultiquadric:
                    return 1.0 / std::sqrt(1.0 + epsilon * epsilon * r * r);
                case Type2DRBF::Gaussian:
                    return std::exp(-epsilon * r * r);
                case Type2DRBF::ThinPlate:
                    return r * r * std::log(r);
                default:
                    throw std::invalid_argument("Unknown rbf interpolation type");
            }
        }

        constexpr std::string get_type_name() const {
            switch (type) {
                case Type2DRBF::Linear:
                    return "Linear";
                case Type2DRBF::Cubic:
                    return "Cubic";
                case Type2DRBF::Quintic:
                    return "Quintic";
                case Type2DRBF::Multiquadric:
                    return "Multiquadric";
                case Type2DRBF::InverseMultiquadric:
                    return "InverseMultiquadric";
                case Type2DRBF::Gaussian:
                    return "Gaussian";
                case Type2DRBF::ThinPlate:
                    return "ThinPlate";
                default:
                    throw std::invalid_argument("Unknown rbf interpolation type");
            }
        }

        using arma_vec = arma::Col<value_type>; // arma::vec is arma::Col<double>
        using arma_mat = arma::Mat<value_type>; // arma::mat is arma::Mat<double>

        container_type xp_;
        container_type yp_;
        arma_vec zp_;
        arma_vec weights_;
        int poly_degree_;
    };

#endif
}