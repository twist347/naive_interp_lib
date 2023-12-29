Some naive header-only interpolation library.

Dependencies: Boost, CGAL, Openmp, GSL(GNU Scientific Library), Eigen(included)

There are following namespaces: ```ni::_1d``` for 1-dimension space and ```ni::_2d``` for 2-dimension space
interpolators. All ```_1d``` interpolators have base class ```i_1d_base``` and
all ```_2d``` interpolators - ```i_2d_base```.

All interpolators are not copied, only moved.

All interpolators have the following traits: ```container_type```, ```value_type```, ```size_type```.

### 1D
In ```_1d``` namespace there are 
```Type1D { Prev, Next, NearestNeighbour, Linear, Quadratic, Cubic, CubicSpline, Akima, Steffen }```

```c++
    #include <interp_make.h>
    
    // ...

    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Type>(xp, yp);
    auto y = interp(x);
```


Points from ```xp``` array must be sorted strictly in ascending order.

### 2D

Scatter is suitable for regular and irregular grids.

In ```_2d``` namespace there are
```Type2DScat { IDW, NearestNeighbour, TIN }```, 

where ```IDW``` is Inverse Distance Weighted and ```TIN``` is Triangulated Irregular Network


```c++
    #include <interp_make.h>

    // ...
    
    auto interp = ni::_2d::make_scat_i<ni::_2d::Type2DScat::Type>(xp, yp, zp);
    auto z = interp(x, y);
```

And in ```_2d``` namespace there are
```Type2DRBF { Linear, Gauss }```

```c++
    #include <interp_make.h>

    // ...
    
    auto interp = ni::_2d::make_rbf_i<ni::_2d::Type2DRBF::Type>(xp, yp, zp);
    auto z = interp(x, y);
```


### Note:
Interpolator works with any container that supports ```[]``` and ```.size()``` operations.
For example std::vector, std::array, std::deque, Eigen::ArrayXd.