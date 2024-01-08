#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <interp2d_base.h>

namespace ni::_2d::detail {
    /// Triangulated Irregular Network
    template<class Container>
    class i_tin : public i_2d_base<Container> {
    private:
        using base_t = i_2d_base<Container>;
    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

    private:
        using kernel_t = CGAL::Exact_predicates_inexact_constructions_kernel;
        using point2_t = kernel_t::Point_2;
        using delaunay_t = CGAL::Delaunay_triangulation_2<kernel_t>;

    public:
        constexpr i_tin(const container_type &xp, const container_type &yp, const container_type &zp) {
            if (xp.size() != yp.size() || xp.size() != zp.size()) {
                throw std::invalid_argument("all xp, yp, zp must be the same size");
            }
            std::vector<point2_t> points(xp.size());
            z_vals_.reserve(xp.size());
            for (size_type i = 0; i < xp.size(); ++i) {
                const point2_t p{xp[i], yp[i]};
                points[i] = p;
                z_vals_[p] = zp[i];
            }
            // much faster than do d_.insert(points[i]) in loop
            d_.insert(points.begin(), points.end());
        }

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_tin)

        constexpr auto operator()(const container_type &x, const container_type &y) const -> container_type override {
            const auto sz = std::min(x.size(), y.size());
            container_type z(sz);
            delaunay_t::Face_handle prev{};

#pragma omp parallel for firstprivate(prev) schedule(guided)
            for (size_type i = 0; i < sz; ++i) {
                // nearest triangle to current point {x[i],y[i]}. prev is a start hint
                const auto nearest_tr = d_.locate({x[i], y[i]}, prev);
                prev = nearest_tr;
                z[i] = calc(nearest_tr, x[i], y[i]);
            }
            return z;
        }

    private:
        constexpr auto
        calc(const delaunay_t::Face_handle &nearest_tr, value_type xi, value_type yi) const -> value_type {
            // get vertices coords of found triangle
            const auto [x1, y1, z1] = get_coords(nearest_tr, 0);
            const auto [x2, y2, z2] = get_coords(nearest_tr, 1);
            const auto [x3, y3, z3] = get_coords(nearest_tr, 2);

            const value_type denominator = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
            const value_type dx = xi - x3, dy = yi - y3;
            // barycentric coordinates
            const value_type alpha = ((y2 - y3) * dx + (x3 - x2) * dy) / denominator;
            const value_type beta = ((y3 - y1) * dx + (x1 - x3) * dy) / denominator;
            const value_type gamma = 1 - alpha - beta;
            return alpha * z1 + beta * z2 + gamma * z3;
        }

        constexpr auto get_coords(const delaunay_t::Face_handle &nearest, int n_vert) const ->
        std::tuple<value_type, value_type, value_type> {
            const auto point = nearest->vertex(n_vert)->point();
            const auto it = z_vals_.find(point);
            return {point.x(), point.y(), it == z_vals_.end() ? 0 : it->second};
        }

        struct point2_t_hash {
            constexpr auto operator()(const point2_t &p) const -> std::size_t {
                std::size_t hash = 0;
                boost::hash_combine(hash, p.x());
                boost::hash_combine(hash, p.y());
                return hash;
            }
        };

        // for z values of 2-d points in network
        std::unordered_map<point2_t, value_type, point2_t_hash> z_vals_;
        delaunay_t d_;
    };
}