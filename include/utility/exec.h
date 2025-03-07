#pragma once

#if __has_include(<execution>)

#include <execution>
#include <cstdio> // for warning message

#endif

#include <algorithm>

#include "params.h"

#if __cpp_lib_execution
    #define PAR_UNSEQ_EX std::execution::par_unseq,
    #define UNSEQ_EX std::execution::unseq,
    #define PAR_EX std::execution::par,
    #define SEQ_EX std::execution::seq,
#else
    #define PAR_UNSEQ_EX
    #define UNSEQ_EX
    #define PAR_EX
    #define SEQ_EX
#endif

namespace interp::utils {

    // noexcept despite on std::transform is not noexcept
    template<std::forward_iterator ForwardIt1, std::forward_iterator ForwardIt2, typename UnaryOp>
    auto custom_transform(
        Exec exec,
        ForwardIt1 first1, ForwardIt1 last1,
        ForwardIt2 d_first,
        UnaryOp unary_op
    ) noexcept -> ForwardIt2 {
#ifndef NDEBUG
#ifndef __cpp_lib_execution
        fprintf(stderr, "execution policies are not supported. falling back to sequential execution.\n");
#endif
#endif

        switch (exec) {
            case Exec::UNSEQ:
                return std::transform(UNSEQ_EX first1, last1, d_first, unary_op);
            case Exec::PAR:
                return std::transform(PAR_EX first1, last1, d_first, unary_op);
            case Exec::PAR_UNSEQ:
                return std::transform(PAR_UNSEQ_EX first1, last1, d_first, unary_op);
            default:
                return std::transform(SEQ_EX first1, last1, d_first, unary_op);
        }
    }

    /* unused
    template<typename ForwardIt, typename Value, typename Compare>
    auto custom_upper_bound(
        ForwardIt first, ForwardIt last,
        const Value& value,
        Compare cmp = std::less{}
    ) noexcept -> ForwardIt {
        if constexpr (std::floating_point<std::remove_cvref_t<Value>>) {
            return std::upper_bound(first, last, value, cmp);
        } else {
            return std::upper_bound(first, last, value);
        }
    }
    */

}