#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/function_output_iterator.hpp>

#include "interp2d_base.h"
#include "utility/utils.h"

namespace ni::_2d::impl {

    template<class Container, bool IsCached = true>
    class i_nearest_neighbour : public i_2d_base<Container, IsCached> {

    private:
        using base_t = i_2d_base<Container, IsCached>;
        using cref_or_value_c_t = base_t::cref_or_val_c_t;
        using cref_type = base_t::cref_type;

    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

    private:
        using point2_t = boost::geometry::model::point<value_type, 2, boost::geometry::cs::cartesian>;

    public:
        constexpr i_nearest_neighbour(cref_type xp, cref_type yp, cref_type zp) {
            if (xp.size() != yp.size() || xp.size() != zp.size()) {
                throw std::invalid_argument("all xp, yp, zp must be the same size");
            }

            for (size_type i = 0; i < zp.size(); ++i) {
                if (!std::isnan(zp[i])) {
                    rtree_.insert({{xp[i], yp[i]}, zp[i]});
                }
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