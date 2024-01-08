#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/function_output_iterator.hpp>

#include <interp2d_base.h>
#include <utils.h>

namespace ni::_2d::impl {
    template<class Container>
    class i_nearest_neighbour : public i_2d_base<Container> {
    private:
        using base_t = i_2d_base<Container>;
    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

    private:
        using point2_t = boost::geometry::model::point<value_type, 2, boost::geometry::cs::cartesian>;

    public:
        constexpr i_nearest_neighbour(const container_type &xp, const container_type &yp, const container_type &zp) {
            if (xp.size() != yp.size() || xp.size() != zp.size()) {
                std::cerr << "all xp, yp, zp must be the same size\n";
                std::terminate();
            }

            for (size_type i = 0; i < xp.size(); ++i) {
                rtree_.insert({{xp[i], yp[i]}, zp[i]});
            }
        }

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_nearest_neighbour)

        constexpr auto operator()(const container_type &x, const container_type &y) const -> container_type override {
            namespace bgi = boost::geometry::index;
            const auto sz = std::min(x.size(), y.size());
            container_type z(sz);

#pragma omp parallel for schedule(guided)
            for (size_type i = 0; i < sz; ++i) {
                const point2_t query_point{x[i], y[i]};
                rtree_.query(bgi::nearest(query_point, 1), boost::make_function_output_iterator(
                        [&z, i](const auto &p) { z[i] = p.second; }
                ));
            }
            return z;
        }

    private:
        boost::geometry::index::rtree<std::pair<point2_t, value_type>, boost::geometry::index::quadratic<16>> rtree_;
    };
}