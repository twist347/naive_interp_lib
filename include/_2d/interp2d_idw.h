#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/function_output_iterator.hpp>

#include <interp2d_base.h>
#include <utils.h>

namespace ni::_2d::impl {
    /// Inverse Distance Weighted
    template<class Container>
    class i_idw : public i_2d_base<Container> {
    private:
        using base_t = i_2d_base<Container>;
    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

    private:
        using point2_t = boost::geometry::model::point<value_type, 2, boost::geometry::cs::cartesian>;
        using point3_t = std::pair<point2_t, value_type>;

    public:
        constexpr i_idw(const container_type &xp,
                        const container_type &yp,
                        const container_type &zp,
                        std::size_t count = 5,
                        std::size_t power = 2) : count_(count), power_(power) {

            if (xp.size() != yp.size() || xp.size() != zp.size()) {
                throw std::invalid_argument("all xp, yp, zp must be the same size");
            }

            for (size_type i = 0; i < zp.size(); ++i) {
                if (!std::isnan(zp[i])) {
                    rtree_.insert({{xp[i], yp[i]}, zp[i]});
                }
            }
        }

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_idw)

        constexpr auto operator()(const container_type &x, const container_type &y) const -> container_type override {
            namespace bgi = boost::geometry::index;
            const auto sz = std::min(x.size(), y.size());
            container_type z(sz);
#pragma omp parallel for schedule(guided)
            for (size_type i = 0; i < sz; ++i) {
                const point2_t current_p(x[i], y[i]);
                // small_vector is allocated in stack
                boost::container::small_vector<point3_t, 0> neighbours;
                neighbours.reserve(count_);
                rtree_.query(
                        bgi::nearest(current_p, count_), std::back_inserter(neighbours));
                z[i] = idw_impl(neighbours, current_p);
            }
            return z;
        }

    private:
        const std::size_t count_;
        const std::size_t power_;
        boost::geometry::index::rtree <std::pair<point2_t, value_type>, boost::geometry::index::quadratic<16>> rtree_;

        constexpr auto calc_radius(const auto &neighbours, const point2_t &current_p) const -> value_type {
            namespace bg = boost::geometry;
            value_type radius = utils::to<value_type>(0.0);

            for (const auto &neighbour: neighbours) {
                radius += bg::distance(neighbour.first, current_p);
            }
            return radius / count_;
        }

        constexpr auto idw_impl(const auto &neighbours, const point2_t &current_p) const -> value_type {
            namespace bg = boost::geometry;

            const value_type radius = calc_radius(neighbours, current_p);
            auto numerator = utils::to<value_type>(0.0);
            auto denominator = utils::to<value_type>(0.0);

            // O(log(n)), in practice: count_ iterations
            for (const auto &neighbor: neighbours) {
                const value_type distance = bg::distance(current_p, neighbor.first);
                if (utils::eq_flt(distance, utils::to<value_type>(0.0))) {
                    return neighbor.second;
                }
                if (utils::less_eq_flt(distance, radius)) {
                    const value_type weight = utils::to<value_type>(1.0) / std::pow(distance, power_);
                    numerator += weight * neighbor.second;
                    denominator += weight;
                }
            }
            return !utils::eq_flt(denominator, utils::to<value_type>(0.0)) ? numerator / denominator
                                                                           : neighbours.back().second;
        }
    };
}