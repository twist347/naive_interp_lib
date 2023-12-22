#pragma once

#include <interp1d.h>

namespace ni::_1d {
    template<Type1D type, class ... Args>
    constexpr decltype(auto) make(Args && ... args) {
        using container_type = typename std::tuple_element_t<0, std::tuple<Args...>>;
        return i_1d<type, container_type>(std::forward<Args>(args)...);
    }
}