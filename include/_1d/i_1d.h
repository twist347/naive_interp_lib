#pragma once

#include <vector>

#include "do_i_1d.h"

namespace interp {

    template<Type1D type, typename Value>
    class i_1d final {
    public:
        using value_type = std::remove_cvref_t<Value>;

        i_1d() = default;

        i_1d(const i_1d &) = default;

        i_1d(i_1d &&) noexcept = default;

        auto operator=(const i_1d &) -> i_1d & = default;

        auto operator=(i_1d &&) noexcept -> i_1d & = default;

        template<
            std::random_access_iterator XpIter,
            std::random_access_iterator YpIter
        >
        auto set_data(
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            const params_1d<Value> &p = {}
        ) -> void {
            static_assert(std::convertible_to<utils::common_iter_val_t<XpIter, YpIter>, value_type>);
            VALIDATE_PARAMS(xp_first, xp_last, p);

            xp_ = { xp_first, xp_last };
            yp_ = { yp_first, yp_first + std::distance(xp_first, xp_last) };
            p_ = p;
        }

        template<
            RandomAccessContainer XpContainer,
            RandomAccessContainer YpContainer
        >
        auto set_data(
            XpContainer &&xp,
            YpContainer &&yp,
            const params_1d<Value> &p = {}
        ) -> void {
            static_assert(std::convertible_to<utils::common_cont_val_t<XpContainer, YpContainer>, value_type>);

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

        template<
            std::random_access_iterator XIter,
            std::random_access_iterator DestIter
        >
        auto operator()(
            XIter x_first, XIter x_last,
            DestIter dest_first
        ) const noexcept -> void {
            static_assert(std::convertible_to<utils::common_iter_val_t<XIter, DestIter>, value_type>);

            using value_type = utils::common_iter_val_t<XIter, DestIter>;
            detail::interp_dispatch<type, value_type>(
                x_first, x_last,
                std::cbegin(xp_), std::cend(xp_),
                std::cbegin(yp_),
                dest_first,
                p_
            );
        }

        template<
            RandomAccessContainer XContainer,
            RandomAccessContainer DestContainer
        >
        auto operator()(
            const XContainer &x,
            DestContainer &dest
        ) const noexcept -> void {
            static_assert(std::convertible_to<utils::common_cont_val_t<XContainer, DestContainer>, value_type>);
            assert(std::size(x) == std::size(dest));

            operator()(std::cbegin(x), std::cend(x), std::begin(dest));
        }

        template<
            RandomAccessContainer XContainer,
            RandomAccessContainer DestContainer = std::remove_cvref_t<XContainer>
        >
        auto operator()(const XContainer &x) const -> DestContainer {
            static_assert(std::convertible_to<utils::common_cont_val_t<XContainer, DestContainer>, value_type>);

            DestContainer dest(std::size(x));
            operator()(std::cbegin(x), std::cend(x), std::begin(dest));

            return dest;
        }

    private:
        using container_type = std::vector<value_type>;

        container_type xp_;
        container_type yp_;
        params_1d<value_type> p_;
    };

}
