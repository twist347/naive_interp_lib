#pragma once

#include <interp2d.h>

namespace ni::_2d {
    template<class Container>
    constexpr auto idw(const Container &x,
                       const Container &y,
                       const Container &xp,
                       const Container &yp,
                       const Container &zp) -> Container {
        return i_idw < Container > (xp, yp, zp)(x, y);
    }

    template<class Container>
    constexpr auto nearest_neighbour(const Container &x,
                                     const Container &y,
                                     const Container &xp,
                                     const Container &yp,
                                     const Container &zp) -> Container {
        return i_nearest_neighbour < Container > (xp, yp, zp)(x, y);
    }

    template<class Container>
    constexpr auto tin(const Container &x,
                       const Container &y,
                       const Container &xp,
                       const Container &yp,
                       const Container &zp) -> Container {
        return i_tin < Container > (xp, yp, zp)(x, y);
    }

    template<class Container>
    constexpr auto i_func_scat(Type2DScatter type,
                               const Container &x,
                               const Container &y,
                               const Container &xp,
                               const Container &yp,
                               const Container &zp) {
        switch (type) {
            case Type2DScatter::IDW:
                return i_idw < Container > (xp, yp, zp)(x, y);
            case Type2DScatter::NearestNeighbour:
                return i_nearest_neighbour < Container > (xp, yp, zp)(x, y);
            case Type2DScatter::TIN:
                return i_tin < Container > (xp, yp, zp)(x, y);
        }
    }
}