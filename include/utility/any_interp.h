#pragma once

#include <variant>
#include <stdexcept>

#include "_1d/i_1d.h"

namespace interp {

    template<typename Value>
    class any_i;

}

namespace interp::detail {

    struct fake_interp {

        template<
            std::random_access_iterator XIter,
            std::random_access_iterator DestIter
        >
        auto operator()(XIter, XIter, DestIter) const -> void {
            throw std::logic_error("operator() called on uninitialized any_i");
        }

        template<
            RandomAccessContainer XContainer,
            RandomAccessContainer DestContainer = std::remove_cvref_t<XContainer>
        >
        auto operator()(const XContainer &) const -> DestContainer {
            throw std::logic_error("operator()(container) called on uninitialized any_i");

            return DestContainer{};
        }

        template<
            RandomAccessContainer XContainer,
            RandomAccessContainer DestContainer
        >
        auto operator()(const XContainer &, DestContainer &) const -> DestContainer {
            throw std::logic_error("operator()(container, dest) called on uninitialized any_i");

            return DestContainer{};
        }

    };

    template<typename Value, typename Interp>
    concept NonSelfInterp = !std::same_as<any_i<Value>, std::remove_cvref_t<Interp>>;

}

namespace interp {

    template<typename Value>
    class any_i final {
    public:
        using value_type = Value;

        any_i() = default;

        any_i(const any_i &) = default;

        any_i(any_i &&) noexcept = default;

        auto operator=(const any_i &) -> any_i & = default;

        auto operator=(any_i &&) noexcept -> any_i & = default;

        // ctor and assignment from interps

        template<typename Interp>
        requires detail::NonSelfInterp<Value, Interp>
        any_i(Interp &&interp) : interp_(std::forward<Interp>(interp)) {}

        template<typename Interp>
        requires detail::NonSelfInterp<Value, Interp>
        auto operator=(Interp &&interp) -> any_i & {
            interp_ = std::forward<Interp>(interp);

            return *this;
        }

        // func operators

        template<std::random_access_iterator XIter, std::random_access_iterator DestIter>
        auto operator()(XIter x_first, XIter x_last, DestIter dest_first) const -> void {
            std::visit([&](auto &&interp) { interp(x_first, x_last, dest_first); }, interp_);
        }

        template<RandomAccessContainer XContainer>
        auto operator()(const XContainer &x) const {
            return std::visit([&](auto &&interp) { return interp(x); }, interp_);
        }

        template<RandomAccessContainer XContainer, RandomAccessContainer DestContainer>
        auto operator()(const XContainer &x, DestContainer &dest) const {
            std::visit([&](auto &&interp) { interp(x, dest); }, interp_);
        }

    private:
        using variant_type = std::variant<
            detail::fake_interp,
            i_1d<Type1D::Prev, Value>,
            i_1d<Type1D::Next, Value>,
            i_1d<Type1D::NearestNeighbour, Value>,
            i_1d<Type1D::Linear, Value>,
            i_1d<Type1D::Quadratic, Value>,
            i_1d<Type1D::Cubic, Value>
        >;

        variant_type interp_;
    };

    // deduction guide
    template<typename Interp>
    any_i(Interp &&interp) -> any_i<typename std::remove_cvref_t<Interp>::value_type>;

}