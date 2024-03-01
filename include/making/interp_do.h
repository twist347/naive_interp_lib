#pragma once

#include "making/interp_make.h"
#include <tuple>

namespace ni {

    namespace detail {

        template<std::size_t N, class ... Args>
        constexpr decltype(auto) get_by_idx(Args && ... args) noexcept {
            return std::get<N>(std::forward_as_tuple(std::forward<Args>(args)...));
        }

    }

    template<auto type, class ... Args>
    constexpr auto do_i(Args && ... args) {
        if constexpr (std::is_same_v<decltype(type), ni::Type1D>) {
            auto && x = detail::get_by_idx<0>(std::forward<Args>(args)...);
            auto && xp = detail::get_by_idx<1>(std::forward<Args>(args)...);
            auto && yp = detail::get_by_idx<2>(std::forward<Args>(args)...);
            return ni::make_i<type, false>(xp, yp)(x);
        } else if constexpr (std::is_same_v<decltype(type), ni::Type2DScat>
                || std::is_same_v<decltype(type), ni::Type2DRBF>) {
            decltype(auto) x = detail::get_by_idx<0>(std::forward<Args>(args)...);
            decltype(auto) y = detail::get_by_idx<1>(std::forward<Args>(args)...);
            decltype(auto) xp = detail::get_by_idx<2>(std::forward<Args>(args)...);
            decltype(auto) yp = detail::get_by_idx<3>(std::forward<Args>(args)...);
            decltype(auto) zp = detail::get_by_idx<4>(std::forward<Args>(args)...);
            return ni::make_i<type, true>(xp, yp, zp)(x, y);
        } else {
            throw std::invalid_argument{"do_i exception"};
        }
    }

}
