#pragma once


#if __has_include(<execution>)

#include <execution>

#endif

#include <iterator>

#if __cpp_lib_execution
    #define PAR_UNSEQ std::execution::par_unseq,
    #define UNSEQ std::execution::unseq,
    #define PAR std::execution::par,
    #define SEQ std::execution::seq,
#else
#define PAR_UNSEQ
    #define UNSEQ
    #define PAR
    #define SEQ
#endif

namespace interp::utils {

    template<std::forward_iterator ForwardIt1, std::forward_iterator ForwardIt2, typename UnaryOp>
    auto custom_transform(
        std::size_t size_of_range,
        std::size_t threshold_value,
        ForwardIt1 first1, ForwardIt1 last1,
        ForwardIt2 d_first,
        UnaryOp unary_op
    ) -> ForwardIt2 {
        if (size_of_range >= threshold_value) {
            return std::transform(PAR_UNSEQ first1, last1, d_first, unary_op);
        }
        return std::transform(SEQ first1, last1, d_first, unary_op);
    }

}