# Dimensional

## Overview

`Dimensional` is a C++ library for handling physical quantities such as length, time, mass, and more. Unlike other physical quantity libraries that are unit-centric, `Dimensional` focuses on dimensions, making it a robust tool for performing dimensional analysis in your C++ projects.

## Key Features

- Dimension-Centric Design: Central class `BaseDimension` is templated on two tuples representing the numerator and denominator unit types.
- Type-Trait Units: Units like `Meters`, `Seconds`, and `Grams` are implemented as type-traits derived from their respective base unit types (`LengthUnit`, `TimeUnit`, `MassUnit`).
- Dimensional Analysis in Arithmetic: Arithmetic operations automatically perform dimensional analysis, ensuring the correctness of physical computations.
- Clean Syntax: Simplified syntax for common dimensions, e.g., `Length<Meters>`.
- Conversions: Conversions can only occur between units of the same dimension.
  - Conversions are written from one unit to another, and contain a slope (the rate of change from one to the other) and an offset (or intercept) which is defaulted to `0.0`.
  - This means only linear conversions are supported, at least for now.
  - Conversions **MUST** exist from the "Primary" unit of each dimension to all other units of the dimension
  - Conversions **MUST** exist from each unit of a dimension to the "Primary" unit of the dimension
  - If a direct conversion exists between two units, it will be used
  - Otherwise, conversion to the "Primary" unit will occur, followed by conversion to the desired unit.
- Compile-time errors: All dimensionality is resolved at compile time, so errors can be addressed earlier in development.
- Efficiency: `Dimensional` performs on-par with simply using `double` arithematic in benchmarks.
  - TODO: Provide benchmarks
  - TODO: Perform more complex benchmarking
- Extensibility: `Dimensional` was designed with extensions in mind. Adding new Dimensions, Units, and converions simply requires a linking a user-provided header.


## Installation

- Clone with https: `git clone https://gitlab.com/drurya96/dimensional.git`
- Clone with ssh: `git clone git@gitlab.com:drurya96/dimensional.git`

### CMake

- Include `add_subdirectory(dimensional)` (or whatever folder name your chose if not defualted) in your top-level CMakeLists.txt
- Where needed, link `StaticDimension_LIB`.
  - Note this is an `INTERFACE` library.
- **IMPORTANT**: Since `Dimensional` is a header-only library, it cannot be pre-compiled as a `STATIC` or `OBJECT` library.
  - Eventually a pre-compiled, "common-uses" version may be provided.
- Eventually a find package will be added [issue #21](https://gitlab.com/drurya96/dimensional/-/issues/21)
- Setting the CMake STRING CACHE variable `DIMENSIONAL_PrecisionType` to `USE_DOUBLE`, `USE_FLOAT`, or `USE_LONG_DOUBLE` will store data as `float`, `double`, or `long double`, respectively.


## Usage
For the most robust usage examples, see the [Unit Tests](https://gitlab.com/drurya96/dimensional/-/tree/main/StaticDimension/UnitTest?ref_type=heads).

Below is an example usage of `Dimensional`

```cpp
#include "StaticLength.h"
#include "StaticTime.h"
#include "StaticSpeed.h"

int main() {
    using namespace std;
    namespace Dim = StaticDimension;

    // Define some quantities
    auto length = Dim::BaseDimension<tuple<Dim::Meters>, tuple<>>{25.0};
    auto time = Dim::BaseDimension<tuple<Dim::Minutes>, tuple<>>{10};

    // Perform arithmetic operations with dimensional analysis
    // Note this will result in a type BaseDimension<tuple<Meters>, tuple<Seconds>>
    auto velocity = length / time;

    cout << "Velocity: " << velocity.GetVal<tuple<Dim::Feet>, tuple<Dim::Seconds>>() << " f/sec" << endl;

    // Cleaner syntax for common dimensions
    Dim::Length<Dim::Meters> distance{100.0};
    Dim::Time<Dim::Seconds> duration{5.0};

    Dim::Speed speed = distance / duration;

    cout << "Speed: " << speed.GetVal<tuple<Dim::Feet>, tuple<Dim::Seconds>>() << " f/s" << endl;

    return 0;
}
```

## Extending Dimensional

For examples of adding Dimensions, Units, Conversions, and Literals, see [the Example Extensions](https://gitlab.com/drurya96/dimensional/-/tree/main/StaticDimension/ExampleExtensions?ref_type=heads) directory.

## Doxygen

See the doxygen output [here]() **This is not ready yet. This text is a placeholder.**

## Writing Issues

*Details for writing issues to come soon*

## Contributing

*Detailed Contributing information to come soon*

When making a branch, please use the `<issue#>-<Custom Description>` syntax.

If an issue does not exist for the feature being added, please add an issue first, and feel free to assign yourself.

## License
This library is open-source, and is licensed under [the MIT license](https://gitlab.com/drurya96/dimensional/-/blob/main/LICENSE)

## Project status
Active development.

This library is in beta release.
