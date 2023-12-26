#pragma once

#include <interp1d.h>
#include <interp2d.h>

namespace ni::_1d {

    /// make(xp, yp)
    template<Type1D type, class ... Args>
    constexpr decltype(auto) make_i(Args && ... args) {
        using container_type = typename std::tuple_element_t<0, std::tuple<Args...>>;
        return i_1d<type, container_type>(std::forward<Args>(args)...);
    }
}

namespace ni::_2d {

    /// make(xp, yp, zp)
    template<Type2DScat type, class ... Args>
    constexpr decltype(auto) make_scat_i(Args && ... args) {
        using container_type = typename std::tuple_element_t<0, std::tuple<Args...>>;
        return i_scat<type, container_type>(std::forward<Args>(args)...);
    }
}