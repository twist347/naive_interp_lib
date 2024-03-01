#pragma once

#include "interp2d_types.h"
#include "interp2d_idw.h"
#include "interp2d_nn.h"
#include "interp2d_tin.h"

namespace ni::_2d::impl {

    namespace detail {
        template<Type2DScat type, class Container, bool IsCached = true>
        struct interp;

        template<class Container, bool IsCached>
        struct interp<Type2DScat::IDW, Container, IsCached> {
            using type = _2d::impl::i_idw<Container>;
        };

        template<class Container, bool IsCached>
        struct interp<Type2DScat::NearestNeighbour, Container, IsCached> {
            using type = _2d::impl::i_nearest_neighbour<Container>;
        };

        template<class Container, bool IsCached>
        struct interp<Type2DScat::TIN, Container, IsCached> {
            using type = _2d::impl::i_tin<Container>;
        };

        template<Type2DScat type, class Container, bool IsCached = true>
        using interp_t = interp<type, Container, IsCached>::type;

    }

    template<Type2DScat type, class Container, bool IsCached = true>
    class i_scat : public i_2d_base<Container, IsCached> {

    private:
        using base_t = i_2d_base<Container, IsCached>;
        using cref_or_value_c_t = base_t::cref_or_val_c_t;
        using cref_type = base_t::cref_type;

    public:
        using container_type = base_t::container_type;
        using value_type = base_t::value_type;
        using size_type = base_t::size_type;

        template<class ... Args>
        constexpr i_scat(Args && ... args) : interp_(std::forward<Args>(args)...) {}

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_scat)

        constexpr auto operator()(cref_type x, cref_type y) const -> container_type override {
            return interp_(x, y);
        }

    private:
        using interp_t = detail::interp_t<type, container_type, IsCached>;
        interp_t interp_;
    };

}