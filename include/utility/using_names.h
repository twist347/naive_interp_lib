#pragma once

#include "../_1d/interp1d.h"
#include "../_2d/interp2d.h"

namespace ni {

    template<class Container>
    using i_1d_base = _1d::i_1d_base<Container>;

    template<class Container>
    using i_2d_base = _2d::i_2d_base<Container>;

    template<ni::Type1D type, class Container>
    using i_1d = _1d::impl::i_1d<type, Container>;

    template<ni::Type2DScat type, class Container>
    using i_2d_scat = _2d::impl::i_scat<type, Container>;

    template<ni::Type2DRBF type, class Container>
    using i_2d_rbf = _2d::impl::i_rbf<type, Container>;

}