#pragma once

#include <interp2d.h>

namespace ni::_2d {
    template<class Container>
    constexpr auto idw(const Container &x,
                       const Container &y,
                       const Container &xp,
                       const Container &yp,
                       const Container &zp) -> Container {
        return i_idw<Container>(xp, yp, zp)(x, y);
    }
}