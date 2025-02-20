# naive interp lib

Some header-only interpolation library.

In progress.

### Common notes:

* No explicit exceptions in Release build (except "any_i" interpolator)
* Iterator type must satisfy std::random_access_iterator concept
* Container type must satisfy custom RandomAccessContainer concept:

```c++
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
```

## 1D

#### Note:

* xp range must be sorted in non-decreasing order. ub otherwise (checked in Debug build, see below)
* No explicit exceptions in Release build (except any interpolator)
* Some kind of checks in Debug mode (xp is sorted, valid combination of params flags, min number of points for interp
  type)

### Types of interpolation:

```c++
    enum class Type1D {
        Prev,
        Next,
        NearestNeighbour,
        Linear,
        Quadratic,
        Cubic
    };
```

### Additional params:

```c++
    enum class Exec {
        SEQ,
        PAR,
        UNSEQ,
        PAR_UNSEQ
    };

    template<typename Value>
    struct params_1d {
        Exec exec = Exec::SEQ;
        bool extrapolate = false;
        bool bounds_check = false;
        std::pair<Value, Value> bounds = std::make_pair(utils::nan<Value>, utils::nan<Value>);
    };
```

### Public interface

#### Do

```c++
    // iterators interface
    template<
        Type1D type,
        std::random_access_iterator XIter,
        std::random_access_iterator XpIter,
        std::random_access_iterator YpIter,
        std::random_access_iterator DestIter,
        typename Value = utils::common_iter_val_t<XIter, XpIter, YpIter, DestIter>
    >
    auto do_i(
        XIter x_first, XIter x_last,
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        DestIter dest_first,
        const params_1d<Value> &p = {}
    ) noexcept -> void;
    
    template<
        Type1D type,
        RandomAccessContainer XContainer,
        RandomAccessContainer XpContainer,
        RandomAccessContainer YpContainer,
        RandomAccessContainer DestContainer,
        typename Value = utils::common_cont_val_t<XContainer, XpContainer, YpContainer, DestContainer>
    >
    auto do_i(
        const XContainer &x,
        const XpContainer &xp,
        const YpContainer &yp,
        DestContainer &dest,
        const params_1d<Value> &p = {}
    ) noexcept -> void;
    
    // containers interface
    template<
        Type1D type,
        RandomAccessContainer XContainer,
        RandomAccessContainer XpContainer,
        RandomAccessContainer YpContainer,
        RandomAccessContainer DestContainer = std::remove_cvref_t<XContainer>,
        typename Value = utils::common_cont_val_t<XContainer, XpContainer, YpContainer, DestContainer>
    >
    auto do_i(
        const XContainer &x,
        const XpContainer &xp,
        const YpContainer &yp,
        const params_1d<Value> &p = {}
    ) -> DestContainer;
```

#### Make

```c++
    // iterators interface
    template<
        Type1D type,
        std::random_access_iterator XpIter,
        std::random_access_iterator YpIter,
        typename Value = utils::common_iter_val_t<XpIter, YpIter>
    >
    auto make_i(
        XpIter xp_first, XpIter xp_last,
        YpIter yp_first,
        const params_1d<Value> &p = {}
    ) -> i_1d<type, Value>;
    
    // containers interface
    template<
        Type1D type,
        RandomAccessContainer XpContainer,
        RandomAccessContainer YpContainer,
        typename Value = utils::common_cont_val_t<XpContainer, YpContainer>
    >
    auto make_i(
        XpContainer &&xp,
        YpContainer &&yp,
        const params_1d<Value> &p = {}
    ) -> i_1d<type, Value>;
```

#### Classes

```c++
    template<Type1D type, typename Value>
    class i_1d { ... };
    
    template<typename Value>
    class any_i { ... };
```

## Usage

#### One header

```c++
    #include <interp.h>
```

#### Prepare data

```c++
    const std::vector<double> x{...}, xp{...}, yp{...};
```

### "make" interface

#### Iterator interface

```c++
    std::vector<double> y(std::size(x)); // to write data
    
    const auto interp = interp::make<interp::Type1D::Linear>(xp.cbegin(), xp.cend(), yp.cbegin());
    interp(x.cbegin(), x.cend(), y.begin()); // write to y
```

#### Container interface

```c++
    const auto interp = interp::make_i<interp::Type1D::Linear>(xp, yp);
    const auto y = interp(x); // get a vector
    // or
    interp(x, y); // write to y
```

### "do" interface

#### Iterator interface

```c++
    // write to y
    interp::do_i<interp::Type1D::Prev>(
        x.cbegin(), x.cend(),
        xp.cbegin(), xp.cend(),
        yp.cbegin(),
        y.begin()
    );
```

#### Container interface

```c++
    std::vector<double> y(std::size(x)); // to write data
    
    interp::do_i<interp::Type1D::Next>(x, xp, yp, y); // write to y
    // or
    const auto other_y = interp::do_i<interp::Type1D::Next>(x, xp, yp); // get a vector
```

### 2D

In progress
