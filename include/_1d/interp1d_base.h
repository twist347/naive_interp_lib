#pragma once

#include "utility/container_type_traits.h"
#include "utility/macros.h"

namespace ni::_1d {

    template<class Container, bool IsCached = true>
    class i_1d_base {

    private:
        using pure_container = std::remove_cvref_t<Container>;

    public:
        using container_type = pure_container;
        using cref_type = const container_type &;
        using cref_or_val_c_t = std::conditional_t<IsCached, container_type, cref_type>;
        using value_type = detail::container_value_type_t<container_type>;
        using size_type = detail::container_size_type_t<container_type>;

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_1d_base)

        constexpr i_1d_base() = default;

        constexpr virtual auto operator()(cref_type) const -> container_type = 0;

        constexpr virtual ~i_1d_base() = default;
    };

}