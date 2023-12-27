Some naive header-only interpolation library.

Dependencies: Boost, CGAL, Openmp.

There are following namespaces: ```ni::_1d``` for 1-dimension space and ```ni::_2d``` for 2-dimension space
interpolators. All ```_1d``` interpolators have base class ```i_1d_base``` and
all ```_2d``` interpolators - ```i_2d_base```.

All interpolators are not copied, only moved.

All interpolators have the following traits: ```container_type```, ```value_type```, ```size_type```.

### 1D
In ```_1d``` namespace there are 
```Type1D { Prev, Next, NearestNeighbour, Linear, Quadratic, Cubic }```

```c++
    auto y = ni::_1d::func_i<ni::_1d::Type1D::Type>(x, xp, yp); // compile time
    auto y = ni::_1d::func_i(ni::_1d::Type1D::Type, x, xp, yp); // runtime time
```

or

```c++
    auto interp = ni::_1d::make_i<ni::_1d::Type1D::Type>(xp, yp);
    auto y = interp(x);
```


Points from ```xp``` array must be sorted strictly in ascending order.

### 2D

Scatter is suitable for regular and irregular grids.

In ```_2d``` namespace there are
```Type2DScat { IDW, NearestNeighbour, TIN }```

```c++
    auto z = ni::_2d::func_i_scat<ni::_2d::Type2DScat::Type>(x, y, xp, yp, zp); // compile time
    auto z = ni::_2d::func_i_scat(ni::_2d::Type2DScat::Type, x, y, xp, yp, zp); // runtime time
```

or

```c++
    auto interp = ni::_2d::make_i<ni::_2d::Type2DScat::Type>(xp, yp, zp);
    auto z = interp(x, y);
```
