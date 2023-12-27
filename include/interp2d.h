#pragma once

#include <utils.h>
#include <interp2d_types.h>
#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/function_output_iterator.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(class_name) \
    constexpr class_name(const class_name &) = delete; \
    constexpr class_name(class_name &&) noexcept = default; \
    constexpr auto operator=(const class_name &) -> class_name & = delete; \
    constexpr auto operator=(class_name &&) noexcept -> class_name & = default; \

namespace ni::_2d {

    template<class Container>
    class i_2d_base {
    public:
        using container_type = std::remove_cvref_t<Container>;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_2d_base)

        constexpr i_2d_base() = default;

        constexpr virtual auto operator()(const container_type &, const container_type &) const -> container_type = 0;

        constexpr virtual ~i_2d_base() = default;
    };

    /// Inverse Distance Weighted
    template<class Container>
    class i_idw : public i_2d_base<Container> {
    public:
        using container_type = i_2d_base<Container>::container_type;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

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

            for (size_type i = 0; i < xp.size(); ++i) {
                rtree_.insert({{xp[i], yp[i]}, zp[i]});
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
        boost::geometry::index::rtree<std::pair<point2_t, value_type>, boost::geometry::index::quadratic<16>> rtree_;

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

    template<class Container>
    class i_nearest_neighbour : public i_2d_base<Container> {
    public:
        using container_type = i_2d_base<Container>::container_type;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

    private:
        using point2_t = boost::geometry::model::point<value_type, 2, boost::geometry::cs::cartesian>;

    public:
        constexpr i_nearest_neighbour(const container_type &xp, const container_type &yp, const container_type &zp) {
            if (xp.size() != yp.size() || xp.size() != zp.size()) {
                throw std::invalid_argument("all xp, yp, zp must be the same size");
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

    /// Triangulated Irregular Network
    template<class Container>
    class i_tin : public i_2d_base<Container> {
    public:
        using container_type = i_2d_base<Container>::container_type;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

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
                auto nearest_tr = d_.locate({x[i], y[i]}, prev);
                prev = nearest_tr;
                z[i] = calc(nearest_tr, x[i], y[i]);
            }
            return z;
        }

    private:
        constexpr auto calc(const delaunay_t::Face_handle &nearest_tr, value_type xi, value_type yi) const -> value_type {
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
            return {point.x(), point.y(), z_vals_.find(point)->second};
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

    namespace detail {
        template<Type2DScat type, class Container>
        struct interp;

        template<class Container>
        struct interp<Type2DScat::IDW, Container> {
            using type = i_idw<Container>;
        };

        template<class Container>
        struct interp<Type2DScat::NearestNeighbour, Container> {
            using type = i_nearest_neighbour<Container>;
        };

        template<class Container>
        struct interp<Type2DScat::TIN, Container> {
            using type = i_tin<Container>;
        };

        template<Type2DScat type, class Container>
        using interp_t = interp<type, Container>::type;
    }

    template<Type2DScat type, class Container>
    class i_scat : public i_2d_base<Container> {
    public:
        using container_type = i_2d_base<Container>::container_type;
        using value_type = container_type::value_type;
        using size_type = container_type::size_type;

        template<class ... Args>
        constexpr i_scat(Args &&... args) : interp_(std::forward<Args>(args)...) {}

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_scat)

        constexpr auto operator()(const container_type &x, const container_type &y) const -> container_type override {
            return interp_(x, y);
        }

    private:
        using interp_t = detail::interp_t<type, container_type>;
        interp_t interp_;
    };
}