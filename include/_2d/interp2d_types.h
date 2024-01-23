#pragma once

namespace ni {

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