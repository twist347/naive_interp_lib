#pragma once

#include <interp2d.h>

namespace ni::_2d {

    // run time
    template<class Container>
    constexpr auto func_i_scat(Type2DScat type,
                               const Container &x,
                               const Container &y,
                               const Container &xp,
                               const Container &yp,
                               const Container &zp) {
        switch (type) {
            case Type2DScat::IDW:
                return i_idw<Container>(xp, yp, zp)(x, y);
            case Type2DScat::NearestNeighbour:
                return i_nearest_neighbour<Container>(xp, yp, zp)(x, y);
            case Type2DScat::TIN:
                return i_tin<Container>(xp, yp, zp)(x, y);
            default:
                throw std::invalid_argument("unknown 2d scatter interpolation type");
        }
    }

    // compile time
    template<Type2DScat type, class Container>
    constexpr auto func_i_scat(const Container &x,
                               const Container &y,
                               const Container &xp,
                               const Container &yp,
                               const Container &zp) {
        switch (type) {
            case Type2DScat::IDW:
                return i_idw<Container>(xp, yp, zp)(x, y);
            case Type2DScat::NearestNeighbour:
                return i_nearest_neighbour<Container>(xp, yp, zp)(x, y);
            case Type2DScat::TIN:
                return i_tin<Container>(xp, yp, zp)(x, y);
            default:
                throw std::invalid_argument("unknown 2d scatter interpolation type");
        }
    }
}