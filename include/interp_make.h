#pragma once

#include <interp1d.h>
#include <interp2d.h>

namespace ni::_1d {

    /// auto interp = make_i<Type>(xp, yp)
    template<Type1D type, class ... Args>
    constexpr auto make_i(Args && ... args) {
        using container_type = std::tuple_element_t<0, std::tuple<Args...>>;;
        return impl::i_1d<type, container_type>(std::forward<Args>(args)...);
    }
}

namespace ni::_2d {

    /// auto interp = make_scat_i<Type>(xp, yp, zp)
    template<Type2DScat type, class ... Args>
    constexpr auto make_scat_i(Args && ... args) {
        using container_type = std::tuple_element_t<0, std::tuple<Args...>>;
        return impl::i_scat<type, container_type>(std::forward<Args>(args)...);
    }

    /// auto interp = make_rbf_i<Type>(xp, yp, zp)
    template<Type2DRBF type, class ... Args>
    constexpr auto make_rbf_i(Args && ... args) {
        using container_type = std::tuple_element_t<0, std::tuple<Args...>>;
        return impl::i_rbf<type, container_type>(std::forward<Args>(args)...);
    }
}