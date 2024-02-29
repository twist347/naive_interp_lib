#pragma once

#include "_1d/interp1d.h"
#include "_2d/interp2d.h"
#include "../utility/using_names.h"

namespace ni {

    template<auto type, class ... Args>
    constexpr auto make_i(Args && ... args) {
        using container_type = std::remove_cvref_t<std::tuple_element_t<0, std::tuple<Args...>>>;
        if constexpr (std::is_same_v<decltype(type), Type1D>) {
            return ni::i_1d<type, container_type>(std::forward<Args>(args)...);
        } else if constexpr (std::is_same_v<decltype(type), Type2DScat>) {
            return ni::i_2d_scat<type, container_type>(std::forward<Args>(args)...);
        } else if constexpr (std::is_same_v<decltype(type), Type2DRBF>) {
            return ni::i_2d_rbf<type, container_type>(std::forward<Args>(args)...);
        } else {
            throw std::invalid_argument("unknown interp type");
        }
    }

}