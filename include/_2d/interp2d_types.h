#pragma once

namespace ni::_2d {

    enum class Type2DScat {
        IDW, // Inverse Distance Weighted
        NearestNeighbour,
        TIN // Triangulated Irregular Network
    };

    enum class Type2DRBF {
        Linear,
        Cubic,
        Quintic,
        Multiquadric,
        InverseMultiquadric,
        Gaussian,
        ThinPlate
    };
}