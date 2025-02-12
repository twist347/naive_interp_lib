#pragma once

#include <variant>

#include "../_1d/i_1d.h"

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
        requires (!std::same_as<any_i, std::remove_cvref_t<Interp>>)
        any_i(Interp &&interp) : interp_(std::forward<Interp>(interp)) {}

        template<typename Interp>
        requires (!std::same_as<any_i, std::remove_cvref_t<Interp>>)
        auto operator=(Interp &&interp) -> any_i & {
            interp_ = std::forward<Interp>(interp);
            return *this;
        }

        // set methods

        template<typename XpIter, typename YpIter>
        auto set_data(
            XpIter xp_first, XpIter xp_last,
            YpIter yp_first,
            const params_1d<typename std::iterator_traits<XpIter>::value_type> &p = {}
        ) -> void {
            std::visit([&](auto &&interp) { interp.set_data(xp_first, xp_last, yp_first, p); }, interp_);
        }

        template<typename XpContainer, typename YpContainer>
        auto set_data(
            const XpContainer &xp,
            const YpContainer &yp,
            const params_1d<typename XpContainer::value_type> &p = {}
        ) -> void {
            std::visit([&](auto &&interp) { interp.set_data(xp, yp, p); }, interp_);
        }

        // func operators

        template<typename XIter, typename DestIter>
        auto operator()(XIter x_first, XIter x_last, DestIter dest_first) const -> void {
            std::visit([&](auto &&interp) { interp(x_first, x_last, dest_first); }, interp_);
        }

        template<typename XContainer>
        auto operator()(const XContainer &x) const {
            return std::visit([&](auto &&interp) { return interp(x); }, interp_);
        }

        template<typename XContainer, typename DestContainer>
        auto operator()(const XContainer &x, DestContainer &dest) const {
            return std::visit([&](auto &&interp) { return interp(x, dest); }, interp_);
        }

    private:
        using variant_type = std::variant<
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