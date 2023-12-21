#pragma once

#include <exception>
#include <interp1d_algs.h>
#include <stdexcept>

namespace naive_interp::_1d {

#define GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(class_name) \
    class_name(const class_name &) = delete; \
    class_name(class_name &&) noexcept = default; \
    auto operator=(const class_name &) -> class_name & = delete; \
    auto operator=(class_name &&) noexcept -> class_name & = default; \

    enum class Type1D {
        Prev,
        Next,
        NearestNeighbour,
        Linear
    };

    template<class Container>
    class i_1d_base {
    public:
        using container_type = Container;
        using value_type = Container::value_type;
        using size_type = Container::size_type;

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_1d_base)

        i_1d_base() = default;

        virtual auto operator()(const Container &) const -> container_type = 0;

        virtual ~i_1d_base() = default;
    };


    template<Type1D type, class Container>
    class i_1d : public i_1d_base<Container> {
    public:
        constexpr i_1d(const Container &xp, const Container &yp) {
            init(xp, yp);
        }

        using container_type = typename i_1d_base<Container>::container_type;
        using value_type = typename i_1d_base<Container>::value_type;
        using size_type = typename i_1d_base<Container>::size_type;

        constexpr auto operator()(const Container &x) const -> container_type override {
            switch (type) {
                case Type1D::Prev:
                    return naive_interp::prev(x, xp_, yp_);
                case Type1D::Next:
                    return naive_interp::next(x, xp_, yp_);
                case Type1D::NearestNeighbour:
                    return naive_interp::nearest_neighbour(x, xp_, yp_);
                case Type1D::Linear:
                    return naive_interp::linear(x, xp_, yp_);
            }
        }

    private:
        constexpr void init(const Container &xp, const Container &yp) {
            if (xp.size() != yp.size()) {
                throw std::invalid_argument("size of xp must be equal to yp size");
            }

            if (xp.size() < min_num_points) {
                throw std::invalid_argument(
                        "the number of points must be at least " + std::to_string(min_num_points)
                );
            }
            xp_ = xp;
            yp_ = yp;
        }

        template<class Type>
        constexpr static auto min_points_extr(Type type_t) -> int {
            switch (type_t) {
                case Type1D::Prev:
                case Type1D::Next:
                case Type1D::NearestNeighbour:
                    return 1;
                case Type1D::Linear:
                    return 2;
                default:
                    throw std::invalid_argument("unknown 1d interpolation type");
            }
        }

        constexpr static int min_num_points = min_points_extr(type);

        Container xp_;
        Container yp_;
    };
}