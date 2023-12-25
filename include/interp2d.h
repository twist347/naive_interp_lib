#pragma once

#include <utils.h>
#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/container/small_vector.hpp>

#define GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(class_name) \
    constexpr class_name(const class_name &) = delete; \
    constexpr class_name(class_name &&) noexcept = default; \
    constexpr auto operator=(const class_name &) -> class_name & = delete; \
    constexpr auto operator=(class_name &&) noexcept -> class_name & = default; \

namespace ni::_2d {

    enum class Type2DScatter {
        IDW
    };

    template<class Container>
    class i_2d_base {
    public:
        using container_type = typename std::remove_cvref_t<Container>;
        using value_type = typename container_type::value_type;
        using size_type = typename container_type::size_type;

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_2d_base)

        constexpr i_2d_base() = default;

        constexpr virtual auto operator()(const container_type &, const container_type &) const -> container_type = 0;

        constexpr virtual ~i_2d_base() = default;
    };

    /// Inverse Distance Weighted
    template<class Container>
    class i_idw : public i_2d_base<Container> {
    public:
        using container_type = typename i_2d_base<Container>::container_type;
        using value_type = typename container_type::value_type;
        using size_type = typename container_type::size_type;

    private:
        using point2_t = boost::geometry::model::point<value_type, 2, boost::geometry::cs::cartesian>;
        using point3_t = std::pair<point2_t, value_type>;

    public:
        i_idw(const container_type &xp,
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
            auto sz = std::min(x.size(), y.size());
            container_type z(sz);
#pragma omp parallel for
            for (size_type i = 0; i < sz; ++i) {
                point2_t current_p(x[i], y[i]);
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
        std::size_t count_;
        std::size_t power_;
        boost::geometry::index::rtree<std::pair<point2_t, value_type>, boost::geometry::index::quadratic<16>> rtree_;

        auto calc_radius(const auto &neighbours, const point2_t &current_p) const -> value_type {
            namespace bg = boost::geometry;
            value_type radius = utils::to<value_type>(0.0);

            for (const auto &neighbor: neighbours) {
                radius += bg::distance(neighbor.first, current_p);
            }
            return radius / count_;
        }

        auto idw_impl(const auto &neighbours, const point2_t &current_p) const -> value_type {
            namespace bg = boost::geometry;

            const value_type radius = calc_radius(neighbours, current_p);
            auto numerator = utils::to<value_type>(0.0);
            auto denominator = utils::to<value_type>(0.0);

            // O(log(n)), in practice: n_count_ iterations
            for (const auto &neighbor: neighbours) {
                value_type distance = bg::distance(current_p, neighbor.first);
                if (utils::eq_flt(distance, utils::to<value_type>(0.0))) {
                    return neighbor.second;
                }
                if (utils::less_eq_flt(distance, radius)) {
                    value_type weight = utils::to<value_type>(1.0) / std::pow(distance, power_);
                    numerator += weight * neighbor.second;
                    denominator += weight;
                }
            }
            return !utils::eq_flt(denominator, utils::to<value_type>(0.0)) ? numerator / denominator
                                                                           : neighbours.back().second;
        }
    };

    namespace detail {
        template<Type2DScatter type, class Container>
        struct interp;

        template<class Container>
        struct interp<Type2DScatter::IDW, Container> {
            using type = i_idw<Container>;
        };

        template<Type2DScatter type, class Container>
        using interp_t = interp<type, Container>::type;
    }

    template<Type2DScatter type, class Container>
    class i_scat : public i_2d_base<Container> {
    public:
        using container_type = typename i_2d_base<Container>::container_type;
        using value_type = typename container_type::value_type;
        using size_type = typename container_type::size_type;

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