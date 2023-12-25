#pragma once

#include <exception>
#include <interp1d_algs.h>
#include <stdexcept>

#define GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(class_name) \
    constexpr class_name(const class_name &) = delete; \
    constexpr class_name(class_name &&) noexcept = default; \
    constexpr auto operator=(const class_name &) -> class_name & = delete; \
    constexpr auto operator=(class_name &&) noexcept -> class_name & = default; \

namespace ni::_1d {
    enum class Type1D {
        Prev,
        Next,
        NearestNeighbour,
        Linear,
        Quadratic,
        Cubic
    };

    template<class Container>
    class i_1d_base {
    public:
        using container_type = typename std::remove_cvref_t<Container>;
        using value_type = typename container_type::value_type;
        using size_type = typename container_type::size_type;

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_1d_base)

        constexpr i_1d_base() = default;

        constexpr virtual auto operator()(const container_type &) const -> container_type = 0;

        constexpr virtual ~i_1d_base() = default;
    };


    template<Type1D type, class Container>
    class i_1d : public i_1d_base<Container> {
    public:
        using container_type = typename i_1d_base<Container>::container_type;
        using value_type = typename container_type::value_type;
        using size_type = typename container_type::size_type;

        constexpr i_1d(const container_type &xp, const container_type &yp) {
            init(xp, yp);
        }

        GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(i_1d)

        constexpr auto operator()(const container_type &x) const -> container_type override {
            switch (type) {
                case Type1D::Prev:
                    return ni::prev(x, xp_, yp_);
                case Type1D::Next:
                    return ni::next(x, xp_, yp_);
                case Type1D::NearestNeighbour:
                    return ni::nearest_neighbour(x, xp_, yp_);
                case Type1D::Linear:
                    return ni::linear(x, xp_, yp_);
                case Type1D::Quadratic:
                    return ni::quadratic(x, xp_, yp_);
                case Type1D::Cubic:
                    return ni::cubic(x, xp_, yp_);
            }
        }

    private:
        constexpr void init(const container_type &xp, const container_type &yp) {
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
                case Type1D::Quadratic:
                    return 3;
                case Type1D::Cubic:
                    return 4;
                default:
                    throw std::invalid_argument("unknown 1d interpolation type");
            }
        }

        constexpr static int min_num_points = min_points_extr(type);

        container_type xp_;
        container_type yp_;
    };
}