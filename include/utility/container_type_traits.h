#pragma once

#include <utility>

namespace ni::detail {

    // type trait for size_type
    template<class Container, class = void>
    struct container_size_type {
        using type = std::size_t;
    };

    template<class Container>
    struct container_size_type<Container, std::void_t<typename Container::size_type>> {
        using type = typename Container::size_type;
    };

    template<class Container>
    using container_size_type_t = container_size_type<Container>::type;

    // type trait for value_type
    template<class Container, class = void>
    struct container_value_type {
        using type = std::remove_cvref_t<decltype(std::declval<Container>().operator[])>;
    };

    template<class Container>
    struct container_value_type<Container, std::void_t<typename Container::value_type>> {
        using type = typename Container::value_type;
    };

    template<class Container>
    using container_value_type_t = container_value_type<Container>::type;

}