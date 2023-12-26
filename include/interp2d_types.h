#pragma once

namespace ni::_2d {
    enum class Type2DScatter {
        IDW, // Inverse Distance Weighted
        NearestNeighbour,
        TIN // Triangulated Irregular Network
    };
}