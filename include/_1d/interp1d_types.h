#pragma once

namespace ni {

    enum class Type1D {
        Prev,
        Next,
        NearestNeighbour,
        Linear,
        Quadratic,
        Cubic,
        CubicSpline,
        Akima,
        Steffen
    };

    namespace _1d::impl {

        enum class TypeGSL {
            Linear,
            CubicSpline,
            Akima,
            Steffen
        };

    }

}