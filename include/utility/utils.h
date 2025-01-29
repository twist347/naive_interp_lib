#pragma once

#include <concepts>
#include <limits>
#include <type_traits>

#ifdef NDEBUG
    #define INTERP_RELEASE_NOEXCEPT noexcept
#else
    #define INTERP_RELEASE_NOEXCEPT
#endif

namespace interp::utils {

    namespace detail {

        inline constexpr int Precision = 8;

        template<std::floating_point F, std::integral I>
        consteval auto fast_pow(F base, I n) noexcept -> F {
            F res = 1.0;

            if (n < 0) {
                base = 1.0 / base;
                n = -n;
            }

            while (n > 0) {
                if (n & 1) {
                    res *= base;
                }
                base *= base;
                n >>= 1;
            }

            return res;
        }

        template<int Prec = 0, std::floating_point Value = double>
        constexpr auto eps = (Prec == 0) ? std::numeric_limits<Value>::epsilon() : fast_pow<Value, int>(10.0, Prec);

        template<typename Value>
        concept Arithmetic = std::floating_point<Value> || std::integral<Value>;

    }

    template<typename... Containers>
    using common_cont_val_type = std::common_type_t<typename Containers::value_type...>;

    template<typename... Iters>
    using common_iter_val_type = std::common_type_t<typename std::iterator_traits<Iters>::value_type...>;

    template<std::floating_point Value = double>
    constexpr auto nan = std::numeric_limits<Value>::quiet_NaN();

    /// lhs == rhs
    template<std::floating_point Value>
    constexpr auto flt_eq(Value lhs, Value rhs, Value eps_ = detail::eps<-detail::Precision>) noexcept -> bool {
        return std::abs(lhs - rhs) < eps_;
    }

    /// lhs != rhs
    template<std::floating_point Value>
    constexpr auto flt_neq(Value lhs, Value rhs, Value eps_ = detail::eps<-detail::Precision>) noexcept -> bool {
        return !flt_eq(lhs, rhs, eps_);
    }

    /// lhs < rhs
    template<std::floating_point Value>
    constexpr auto flt_less(Value lhs, Value rhs, Value eps_ = detail::eps < -detail::Precision>) noexcept -> bool {
        return rhs - lhs > eps_;
    }

    /// lhs > rhs
    template<std::floating_point Value>
    constexpr auto flt_greater(Value lhs, Value rhs, Value eps_ = detail::eps<-detail::Precision>) noexcept -> bool {
        return lhs - rhs > eps_;
    }

    /// lhs <= rhs
    template<std::floating_point Value>
    constexpr auto flt_less_eq(Value lhs, Value rhs, Value eps_ = detail::eps<-detail::Precision>) noexcept -> bool {
        return flt_less(lhs, rhs, eps_) || flt_eq(lhs, rhs, eps_);
    }

    /// lhs >= rhs
    template<std::floating_point Value>
    constexpr auto flt_greater_eq(Value lhs, Value rhs, Value eps_ = detail::eps<-detail::Precision>) noexcept -> bool {
        return flt_greater(lhs, rhs, eps_) || flt_eq(lhs, rhs, eps_);
    }

    /// lhs == rhs
    template<detail::Arithmetic Value>
    constexpr auto eq(Value lhs, Value rhs) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_eq(lhs, rhs);
        } else {
            return lhs == rhs;
        }
    }

    /// lhs == rhs
    template<detail::Arithmetic Value>
    constexpr auto eq(Value lhs, Value rhs, Value eps_) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_eq(lhs, rhs, eps_);
        } else {
            return lhs == rhs;
        }
    }

    /// lhs != rhs
    template<detail::Arithmetic Value>
    constexpr auto neq(Value lhs, Value rhs) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_neq(lhs, rhs);
        } else {
            return lhs != rhs;
        }
    }

    /// lhs != rhs
    template<detail::Arithmetic Value>
    constexpr auto neq(Value lhs, Value rhs, Value eps_) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_neq(lhs, rhs, eps_);
        } else {
            return lhs != rhs;
        }
    }

    /// lhs < rhs
    template<detail::Arithmetic Value>
    constexpr auto less(Value lhs, Value rhs) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_less(lhs, rhs);
        } else {
            return lhs < rhs;
        }
    }

    /// lhs < rhs
    template<detail::Arithmetic Value>
    constexpr auto less(Value lhs, Value rhs, Value eps_) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_less(lhs, rhs, eps_);
        } else {
            return lhs < rhs;
        }
    }

    /// lhs > rhs
    template<detail::Arithmetic Value>
    constexpr auto greater(Value lhs, Value rhs) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_greater(lhs, rhs);
        } else {
            return lhs > rhs;
        }
    }

    /// lhs > rhs
    template<detail::Arithmetic Value>
    constexpr auto greater(Value lhs, Value rhs, Value eps_) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_greater(lhs, rhs, eps_);
        } else {
            return lhs > rhs;
        }
    }

    /// lhs <= rhs
    template<detail::Arithmetic Value>
    constexpr auto less_eq(Value lhs, Value rhs) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_less_eq(lhs, rhs);
        } else {
            return lhs <= rhs;
        }
    }

    /// lhs <= rhs
    template<detail::Arithmetic Value>
    constexpr auto less_eq(Value lhs, Value rhs, Value eps_) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_less_eq(lhs, rhs, eps_);
        } else {
            return lhs <= rhs;
        }
    }

    /// lhs >= rhs
    template<detail::Arithmetic Value>
    constexpr auto greater_eq(Value lhs, Value rhs) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_greater_eq(lhs, rhs);
        } else {
            return lhs >= rhs;
        }
    }

    /// lhs >= rhs
    template<detail::Arithmetic Value>
    constexpr auto greater_eq(Value lhs, Value rhs, Value eps_) noexcept -> bool {
        if constexpr (std::is_floating_point_v<Value>) {
            return flt_greater_eq(lhs, rhs, eps_);
        } else {
            return lhs >= rhs;
        }
    }

}