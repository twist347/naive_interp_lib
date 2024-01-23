#pragma once

#include "interp2d_types.h"
#include "interp2d_idw.h"
#include "interp2d_nn.h"
#include "interp2d_tin.h"

namespace ni::_2d::impl {

    namespace detail {
        template<Type2DScat type, class Container>
        struct interp;

        template<class Container>
        struct interp<Type2DScat::IDW, Container> {
            using type = _2d::impl::i_idw<Container>;
        };

        template<class Container>
        struct interp<Type2DScat::NearestNeighbour, Container> {
            using type = _2d::impl::i_nearest_neighbour<Container>;
        };

        template<class Container>
        struct interp<Type2DScat::TIN, Container> {
            using type = _2d::impl::i_tin<Container>;
        };

        template<Type2DScat type, class Container>
        using interp_t = interp<type, Container>::type;
    }

    template<Type2DScat type, class Container>
    class i_scat : public i_2d_base<Container> {
    private:
        using base_t = i_2d_base<Container>;
    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

        template<class ... Args>
        constexpr i_scat(Args &&... args) : interp_(std::forward<Args>(args)...) {}

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_scat)

        constexpr auto operator()(const container_type &x, const container_type &y) const -> container_type override {
            return interp_(x, y);
        }

    private:
        using interp_t = detail::interp_t<type, container_type>;
        interp_t interp_;
    };
}