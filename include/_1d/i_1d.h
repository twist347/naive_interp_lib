#pragma once

#include <vector>

#include "do_i_1d.h"

namespace interp {

    template<Type1D type, typename Value>
    class i_1d final {
    public:
        using value_type = std::remove_cvref_t<Value>;

        i_1d() = default;

        template<typename XpIter, typename YpIter>
        auto set_data(
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            const params_1d<utils::common_iter_val_type<XpIter, YpIter>> &p = {}
        ) -> void {
            static_assert(std::convertible_to<utils::common_iter_val_type<XpIter, YpIter>, value_type>);
            VALIDATE_PARAMS(xp_first, xp_last, p);

            xp_ = { xp_first, xp_last };
            yp_ = { yp_first, yp_first + std::distance(xp_first, xp_last) };
            p_ = p;
        }

        template<typename XpContainer, typename YpContainer>
        auto set_data(
            XpContainer &&xp,
            YpContainer &&yp,
            const params_1d<utils::common_cont_val_type<XpContainer, YpContainer>> &p = {}
        ) -> void {
            if constexpr (std::is_rvalue_reference_v<decltype(xp)> && std::is_rvalue_reference_v<decltype(yp)>) {
                set_data(
                    std::make_move_iterator(xp.begin()), std::make_move_iterator(xp.end()),
                    std::make_move_iterator(yp.begin()),
                    p
                );
            } else {
                set_data(std::cbegin(xp), std::cend(xp), std::cbegin(yp), p);
            }
        }

        template<typename XIter, typename DestIter>
        auto operator()(XIter x_first, XIter x_last, DestIter dest_first) const noexcept -> void {
            detail::interp_dispatch<type>(
                x_first, x_last,
                std::cbegin(xp_), std::cend(xp_),
                std::cbegin(yp_),
                dest_first,
                p_);
        }

        template<typename XContainer, typename DestContainer>
        auto operator()(const XContainer &x, DestContainer &dest) const noexcept -> void {
            assert(std::size(x) == std::size(dest));
            operator()(std::cbegin(x), std::cend(x), std::begin(dest));
        }

        template<typename XContainer, typename DestContainer = std::remove_cvref_t<XContainer>>
        auto operator()(const XContainer &x) const -> DestContainer {
            DestContainer dest(std::size(x));
            operator()(std::cbegin(x), std::cend(x), std::begin(dest));

            return dest;
        }

    private:
        std::vector<Value> xp_;
        std::vector<Value> yp_;
        params_1d<value_type> p_;
    };

}
