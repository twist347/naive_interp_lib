#pragma once

#include <concepts>

namespace interp {

    template<typename Value>
    concept Numeric = std::floating_point<Value> || std::integral<Value>;

    template<typename Container>
    concept RandomAccessContainer = requires(Container& c, const Container& cc, Container &&r) {
        { c.begin() } -> std::random_access_iterator;
        { c.end() } -> std::random_access_iterator;
        { c.size() } -> std::integral;

        { cc.cbegin() } -> std::random_access_iterator;
        { cc.cend() } -> std::random_access_iterator;
        { cc.size() } -> std::integral;

        { std::move(r).begin() } -> std::random_access_iterator;
        { std::move(r).end() } -> std::random_access_iterator;
        { std::move(r).size() } -> std::integral;
    };

    namespace detail {

        template<typename Iter>
        concept rai = std::random_access_iterator<Iter>;

    }

}