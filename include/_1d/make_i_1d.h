#pragma once

#include <iterator>

#include "i_1d.h"
#include "utility/utils.h"

namespace interp {

    // iterator interface

    template<
        Type1D type,
        std::random_access_iterator XpIter,
        std::random_access_iterator YpIter,
        typename Value = utils::common_iter_val_t<XpIter, YpIter>
    >
    auto make_i(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        const params_1d<Value> &p = {}
    ) -> i_1d<type, Value> {
        auto interp = i_1d<type, Value>();
        interp.set_data(xp_first, xp_last, yp_first, p);

        return interp;
    }

    // container interface

    template<
        Type1D type,
        RandomAccessContainer XpContainer,
        RandomAccessContainer YpContainer,
        typename Value = utils::common_cont_val_t<XpContainer, YpContainer>
    >
    auto make_i(
        XpContainer &&xp,
        YpContainer &&yp,
        const params_1d<Value> &p = {}
    ) -> i_1d<type, Value> {
        if constexpr (std::is_rvalue_reference_v<decltype(xp)> && std::is_rvalue_reference_v<decltype(yp)>) {
            return make_i(
                std::make_move_iterator(xp.begin()), std::make_move_iterator(xp.end()),
                std::make_move_iterator(yp.begin()),
                p
            );
        } else {
            return make_i<type>(std::cbegin(xp), std::cend(xp), std::cbegin(yp), p);
        }
    }

}