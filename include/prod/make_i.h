#pragma once

#include "../_1d/i_1d.h"

namespace interp {

    template<Type1D type, typename XpIter, typename YpIter>
    auto make_i(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first, YpIter yp_last,
        const params_1d<typename std::iterator_traits<XpIter>::value_type> &p = {}
    ) -> i_1d<type, typename std::iterator_traits<XpIter>::value_type> {
        auto interp = i_1d<type, typename std::iterator_traits<XpIter>::value_type>();
        interp.set_data(xp_first, xp_last, yp_first, yp_last, p);
        return interp;
    }

    template<Type1D type, typename XpContainer, typename YpContainer>
    auto make_i(
        XpContainer &&xp,
        YpContainer &&yp,
        const params_1d<typename std::remove_cvref_t<XpContainer>::value_type> &p = {}
    ) {
        if constexpr (std::is_rvalue_reference_v<decltype(xp)> && std::is_rvalue_reference_v<decltype(yp)>) {
            return make_i(
                std::make_move_iterator(xp.begin()), std::make_move_iterator(xp.end()),
                std::make_move_iterator(yp.begin()), std::make_move_iterator(yp.end()),
                p
            );
        } else {
            return make_i<type>(std::cbegin(xp), std::cend(xp), std::cbegin(yp), std::cend(yp), p);
        }
    }

}