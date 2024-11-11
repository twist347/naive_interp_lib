#pragma once

#include <vector>

#include "i_1d_types.h"
#include "i_1d_prev_alg.h"
#include "i_1d_next_alg.h"
#include "i_1d_nearest_neighbour_alg.h"
#include "i_1d_quadratic_alg.h"
#include "i_1d_cubic_alg.h"
#include "i_1d_utils.h"
#include "../utility/params.h"

namespace interp {

    template<Type1D type, typename Value>
    class i_1d final {
    public:
        using value_type = std::remove_cvref_t<Value>;

        i_1d() = default;

        template<typename XpIter, typename YpIter>
        auto set_data(
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first, YpIter yp_last,
            const params_1d<typename utils::common_iter_val_type<XpIter, YpIter>> &p = {}
        ) -> void {
            static_assert(std::convertible_to<typename utils::common_iter_val_type<XpIter, YpIter>, value_type>);
            detail::check_input_data(
                xp_first, xp_last, yp_first, yp_last, p.extrapolate, p.bounds_check, detail::min_num_points_<type>()
            );
            xp_ = { xp_first, xp_last };
            yp_ = { yp_first, yp_last };
            p_ = p;
        }

        template<typename XpContainer, typename YpContainer>
        auto set_data(
            const XpContainer &xp,
            const YpContainer &yp,
            const params_1d<typename utils::common_cont_val_type<XpContainer, YpContainer>> &p = {}
        ) -> void {
            set_data(std::cbegin(xp), std::cend(xp), std::cbegin(yp), std::cend(yp), p);
        }

        template<typename XIter, typename DestIter>
        auto operator()(XIter x_first, XIter x_last, DestIter dest_first) const -> void {
            interp_dispatch(
                x_first, x_last,
                std::cbegin(xp_), std::cend(xp_),
                std::cbegin(yp_),
                dest_first,
                p_
            );
        }

        template<typename XContainer>
        auto operator()(const XContainer &x) const {
            using container_type = typename std::remove_cvref_t<XContainer>;
            container_type dest(std::size(x));
            operator()(std::cbegin(x), std::cend(x), std::begin(dest));
            return dest;
        }

    private:
        template<typename ... Args>
        auto interp_dispatch(Args &&... args) const -> void {
            if constexpr (type == Type1D::Prev) {
                prev(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Next) {
                next(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::NearestNeighbour) {
                nearest_neighbour(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Linear) {
                linear(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Quadratic) {
                quadratic(std::forward<Args>(args)...);
            } else if constexpr (type == Type1D::Cubic) {
                cubic(std::forward<Args>(args)...);
            } else {
                throw std::invalid_argument("unsupported interp type");
            }
        }

        std::vector<Value> xp_;
        std::vector<Value> yp_;
        params_1d<value_type> p_;
    };

}
