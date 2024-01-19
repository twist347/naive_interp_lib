#pragma once

#include <container_type_traits.h>

#define GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(class_name) \
    constexpr class_name(const class_name &) = delete; \
    constexpr class_name(class_name &&) noexcept = default; \
    constexpr auto operator=(const class_name &) -> class_name & = delete; \
    constexpr auto operator=(class_name &&) noexcept -> class_name & = default; \

namespace ni::_2d {
    
    template<class Container>
    class i_2d_base {
    public:
        using container_type = std::remove_cvref_t<Container>;
        using value_type = detail::container_value_type_t<container_type>;
        using size_type = detail::container_size_type_t<container_type>;

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_2d_base)

        constexpr i_2d_base() = default;

        constexpr virtual auto operator()(const container_type &, const container_type &) const -> container_type = 0;

        constexpr virtual ~i_2d_base() = default;
    };
}