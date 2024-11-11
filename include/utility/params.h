#pragma once

#include <utility>

namespace interp {

    template<typename Value>
    struct params_1d {
        bool extrapolate = false;
        bool bounds_check = false;
        std::pair<Value, Value> bounds =
                std::pair{ std::numeric_limits<Value>::quiet_NaN(), std::numeric_limits<Value>::quiet_NaN() };
    };

}