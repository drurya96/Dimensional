# Dimensional

## Overview

### `Dimensional`: A modern C++ library for dimensional analysis and type-safe calculations.

`Dimensional` is a C++ library for handling physical quantities such as length, time, mass, and more. Unlike other physical quantity libraries that are unit-centric, `Dimensional` focuses on dimensions, making it a robust tool for performing dimensional analysis in your C++ projects.

[[_TOC_]]

## Key Features

- Dimension-Centric Design: Central class `BaseDimension` is templated on two tuples representing the numerator and denominator unit types.
- Type-Trait Units: Units like `Meters`, `Seconds`, and `Grams` are implemented as type-traits derived from their respective base unit types (`LengthUnit`, `TimeUnit`, `MassUnit`).
- Dimensional Analysis in Arithmetic: Arithmetic operations automatically perform dimensional analysis, ensuring the correctness of physical computations.
- Clean Syntax: Simplified syntax for common dimensions, e.g., `Length<Meters>`.
- **Conversions**
  - Supported only between units of the **same dimension**.
  - Define conversions using:
    - **Slope**: Rate of change between units.
    - **Offset (Intercept)**: The additive adjustment needed for certain conversions. Default is `0.0`, but for cases like temperature, it represents the fixed offset (e.g., `32.0` for Celsius to Fahrenheit).
  - Conversion Requirements:
    - A direct conversion **must** exist from the "Primary" unit to all other units of the dimension.
    - A direct conversion **must** exist from each unit to the "Primary" unit.
    - If no direct conversion exists between two units, the library will:
      1. Convert to the "Primary" unit.
      2. Convert to the desired unit.
- Compile-time errors: All dimensionality is resolved at compile time, so errors can be addressed earlier in development.
- Efficiency: `Dimensional` performs on-par with simply using `double` arithmetic in benchmarks.
  - TODO: Perform more complex benchmarking [isuee #43](https://gitlab.com/dimensionalanalysis/dimensional/-/issues/43)
- Extensibility: `Dimensional` was designed with extensions in mind. Adding new Dimensions, Units, and converions simply requires including a user-provided header.
- Compiler support: (tested, others versions likely work): MSVC 19.41, gcc-12, clang-17
- Language support: C++20 (Prior to version 2.0.0, C++17 and C++20) 

## Installation

- Clone with https: `git clone https://gitlab.com/dimensionalanalysis/dimensional.git`
- Clone with ssh: `git clone git@gitlab.com:dimensionalanalysis/dimensional.git`

### CMake

- Include `add_subdirectory(dimensional)` (or whatever folder name your chose if not defaulted) in your top-level CMakeLists.txt
- Where needed, link `Dimension_LIB`.
  - Note this is an `INTERFACE` library.
- **IMPORTANT**: Since `Dimensional` is a header-only library, it cannot be pre-compiled as a `STATIC` or `OBJECT` library.
  - Eventually a pre-compiled, "common-uses" version may be provided.
- Eventually a `find_package` will be added [issue #21](https://gitlab.com/dimensionalanalysis/dimensional/-/issues/21)
- Setting the CMake STRING CACHE variable `DIMENSIONAL_PrecisionType` to `USE_DOUBLE`, `USE_FLOAT`, or `USE_LONG_DOUBLE` will store data as `float`, `double`, or `long double`, respectively.

## Getting Started

See [Usage.md](https://gitlab.com/dimensionalanalysis/dimensional/-/blob/main/Dimension/Usage.md) for a detailed usage guide

For the most robust usage examples, see the [Unit Tests](https://gitlab.com/dimensionalanalysis/dimensional/-/tree/main/Dimension/UnitTest?ref_type=heads).

Below is an example usage of `Dimensional`

```cpp
#include "LengthDimension.h"
#include "TimeDimension.h"
#include "SpeedDimension.h"

int main() {
   using namespace std;

   // Define some quantities
   auto length = Dimension::BaseDimension<tuple<Dimension::Meters>, tuple<>>{25.0};
   auto time = Dimension::BaseDimension<tuple<Dimension::Minutes>, tuple<>>{10};

   // Perform arithmetic operations with dimensional analysis
   // Note this will result in a type BaseDimension<tuple<Meters>, tuple<Seconds>>
   auto velocity = length / time;

   // Note: Dimensions can be streamed directly, this is to illustrate `GetVal`
   cout << "Velocity: " << velocity.GetVal<tuple<Dimension::Feet>, tuple<Dimension::Seconds>>() << " f/sec" << endl;

   // Cleaner syntax for common dimensions
   Dimension::Length<Dimension::Meters> distance{100.0};
   Dimension::Time<Dimension::Seconds> duration{5.0};

   Dimension::Speed speed = distance / duration;

   // Note: Dimensions can be streamed directly, this is to illustrate `getSpeed`
   cout << "Speed: " << getSpeed<Dimension::Feet, Dimension::Seconds>(speed) << " f/s" << endl;

   return 0;
}
```

## Extending Dimensional

See [Extensions.md](https://gitlab.com/dimensionalanalysis/dimensional/-/blob/main/Dimension/Extensions.md) for information on extending `Dimensional` capabilities for custom usage.

// Brief example of just a custom unit

## Doxygen

See the doxygen output [here]() **This is not ready yet. This text is a placeholder.**

## Writing Issues

We value your feedback and ideas! Writing issues is a great way to improve `Dimensional` and contribute to its development. Whether it's a bug report, a feature request, or just a thought for improvement, your input helps create a better library.

### Before Writing an Issue
- Ensure your issue is related to the library and not an unrelated tool or dependency.
- Check [existing issues](https://gitlab.com/dimensionalanalysis/dimensional/-/issues) to avoid duplicates.
- If possible, reproduce the issue and provide steps to replicate it.

### Writing a New Issue
If no existing issue matches your concern, [create a new issue](https://gitlab.com/dimensionalanalysis/dimensional/-/issues/new) and include:
- **Informative Title**: Summarize the problem or request clearly.
- **Thorough Description**: Explain the issue, feature, or idea in detail.
- Add any relevant context, links, or examples to help understand the issue.

### Issue Types and Labels
Use appropriate labels to classify your issue:
- **Feature Request**: Proposing a new feature for the library.
- **Enhancements**: Suggestions to improve performance, internal code, or usability.
- **Dimension/Unit Request**: Requests for new physical dimensions or units.
- **DevOps**: Changes to pipelines, build systems, or other automation.
- **Bug**: Reporting errors or unexpected behavior.
- **Documentation**: Issues related to improving or updating the documentation.

#### Bug Reports
When reporting a bug, include the following:
- **Environment Details**: OS, compiler version, and library version.
- **Steps to Reproduce**: A minimal code snippet that demonstrates the issue.
- **Expected vs. Actual Behavior**: What did you expect? What happened instead?

#### Feature Requests
- Clearly describe the feature or functionality you want added.
- Include examples or use cases if applicable.

### Assigning Issues
If you're working on an issue, feel free to assign yourself to it so others know it's being worked on. This helps avoid duplication and streamlines collaboration.

---

Thank you for helping to improve `Dimensional`!

## Contributing
See [CONTRIBUTING.md](https://gitlab.com/dimensionalanalysis/dimensional/-/blob/main/CONTRIBUTING.md) for information on contributing to `Dimensional`

## License
This library is open-source, and is licensed under [the Apache License 2.0](https://gitlab.com/dimensionalanalysis/dimensional/-/blob/main/LICENSE)

## Project Status
`Dimensional` is under **active development** and is considered **stable for production use**. The library is ready for integration into your projects, but as capabilities are improvided and expanded, minor updates and new features will be added regularly.

Planned improvements include:
- Adding support for additional dimensions and units.
- Expanding documentation and usage examples.
- Improving serialization logic

For more details on planned features, see [the issues list](https://gitlab.com/dimensionalanalysis/dimensional/-/issues).

## Versioning
`Dimensional` follows [Semantic Versioning](https://semver.org/), starting with version `1.0.0`. Versions are structured as:
- **MAJOR**: Introduces breaking API changes.
- **MINOR**: Adds new functionality in a backward-compatible manner.
- **PATCH**: Fixes bugs without breaking backward compatibility.

**Disclaimer**: Semantic versioning started at 1.0.0, prior versions do **not** follow this standard.