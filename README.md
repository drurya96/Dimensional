# Dimensional

**WORK IN PROGRESS**

This library is in very early development and is not yet ready for any form of release.

**The following documentation is GitLab generated placeholder and is not yet populated**

## Overview
`Dimensional` is a C++ library for handling physical quantities such as length, time, mass, and more. Unlike other physical quantity libraries that are unit-centric, `Dimensional` focuses on dimensions, making it a robust tool for performing dimensional analysis in your C++ projects.

## Key Features
- Dimension-Centric Design: Central class `BaseDimension` is templated on two tuples representing the numerator and denominator unit types.
- Type-Trait Units: Units like `Meters`, `Seconds`, and `Grams` are implemented as type-traits derived from their respective base unit types (`LengthUnit`, `TimeUnit`, `MassUnit`).
- Dimensional Analysis in Arithmetic: Arithmetic operations automatically perform dimensional analysis, ensuring the correctness of physical computations.
- Clean Syntax: Simplified syntax for common dimensions, e.g., `Length<Meters>`.


## Installation

- Clone with https: `git clone https://gitlab.com/drurya96/dimensional.git`
- Clone with ssh: `git clone git@gitlab.com:drurya96/dimensional.git`

### CMake

- Include `add_subdirectory(dimensional)` (or whatever folder name your chose if not defualted) in your top-level CMakeLists.txt
- Where needed, link `StaticDimension_LIB`.
  - Note this is an `INTERFACE` library.
- **IMPORTANT**: Since `Dimensional` is a header-only library, it cannot be pre-compiled as a `STATIC` or `OBJECT` library.
  - Eventually a pre-compile, "common-uses" version may be provided.
- Eventually a find package will be added [issue #21](https://gitlab.com/drurya96/dimensional/-/issues/21)


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

*Details for extensibility to come ASAP (before merge)*

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

This library is still in beta release.
