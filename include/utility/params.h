#pragma once

#include "utils.h"

namespace interp {

    enum class Exec {
        SEQ,
        PAR,
        UNSEQ,
        PAR_UNSEQ
    };

    template<typename Value>
    struct params_1d {
        Exec exec = Exec::SEQ;
        bool extrapolate = false;
        bool bounds_check = false;
        std::pair<Value, Value> bounds = std::pair{utils::nan<Value>, utils::nan<Value>};
    };

}
