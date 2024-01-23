#pragma once

#include "_1d/interp1d.h"
#include "_2d/interp2d.h"

namespace ni {

    /// auto interp = make_i<Type>(xp, yp)
    template<Type1D type, class ... Args>
    constexpr auto make_i(Args && ... args) {
        using container_type = std::tuple_element_t<0, std::tuple<Args...>>;
        return _1d::impl::i_1d<type, container_type>(std::forward<Args>(args)...);
    }

    /// auto interp = make_i<Type>(xp, yp, zp, /*options*/)
    template<Type2DScat type, class ... Args>
    constexpr auto make_i(Args && ... args) {
        using container_type = std::tuple_element_t<0, std::tuple<Args...>>;
        return _2d::impl::i_scat<type, container_type>(std::forward<Args>(args)...);
    }

    /// auto interp = make_i<Type>(xp, yp, zp)
    template<Type2DRBF type, class ... Args>
    constexpr auto make_i(Args && ... args) {
        using container_type = std::tuple_element_t<0, std::tuple<Args...>>;
        return _2d::impl::i_rbf<type, container_type>(std::forward<Args>(args)...);
    }
}