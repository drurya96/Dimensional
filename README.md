# Dimensional

## Overview

### `Dimensional`: A modern C++ library for dimensional analysis and type-safe calculations.

`Dimensional` is a C++ library for handling physical quantities such as length, time, mass, and more. Unlike other physical quantity libraries that are unit-centric, `Dimensional` focuses on dimensions, making it a robust tool for performing dimensional analysis in your C++ projects.

[[_TOC_]]

## Key Features

- Dimension-Centric Design: Central class `base_dimension` is templated on two tuples representing the numerator and denominator unit types.
- Type-Trait Units: Units like `meters`, `seconds`, and `Grams` are implemented as type-traits derived from their respective base unit types (`lengthUnit`, `TimeUnit`, `massUnit`).
- Dimensional Analysis in Arithmetic: Arithmetic operations automatically perform dimensional analysis, ensuring the correctness of physical computations.
- Clean Syntax: Simplified syntax for common dimensions, e.g., `length<meters>`.
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
- Distinction between Quantity and Delta types.
  - See [Deltas and Quantities section](#deltas-and-quantities)
- Common constants provided in `DimensionalConstants.h`
  - For example, to use the `ideal_gas_constant`, use `dimension::Constants::ideal_gas_constant`
- Molar mass of every element and some compounds provided in `DimensionalMolarmasses.h`
- Compile-time errors: All dimensionality is resolved at compile time, so errors can be addressed earlier in development.
- Efficiency: `Dimensional` performs on-par with simply using `double` arithmetic in benchmarks.
  - TODO: Perform more complex benchmarking [issue #43](https://gitlab.com/dimensionalanalysis/dimensional/-/issues/43)
- `constexpr`-friendly: Most operations involving dimensions work as `constexpr`.
- Extensibility: `Dimensional` was designed with extensions in mind. Adding new Dimensions, Units, and conversions simply requires including a user-provided header.
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
#include "length_dimension.h"
#include "timespan_dimension.h"
#include "speed_dimension.h"

int main() {
   using namespace std;

   // Define some quantities
   auto length = dimension::base_dimension<tuple<dimension::meters>, tuple<>>{25.0};
   auto time = dimension::base_dimension<tuple<dimension::minutes>, tuple<>>{10};

   // Perform arithmetic operations with dimensional analysis
   // Note this will result in a type base_dimension<tuple<meters>, tuple<seconds>>
   auto velocity = length / time;

   // Note: Dimensions can be streamed directly, this is to illustrate `GetVal`
   cout << "Velocity: " << velocity.GetVal<tuple<dimension::Feet>, tuple<dimension::seconds>>() << " f/sec" << endl;

   // Cleaner syntax for common dimensions
   dimension::length<dimension::meters> distance{100.0};
   dimension::Time<dimension::seconds> duration{5.0};

   dimension::speed speed = distance / duration;

   // Note: Dimensions can be streamed directly, this is to illustrate `getspeed`
   cout << "speed: " << getspeed<dimension::Feet, dimension::seconds>(speed) << " f/s" << endl;

   return 0;
}
```

## Deltas and Quantities

### Overview

In the context of this library, units within a dimension are treated as deltas—representing differences or durations—unless explicitly wrapped by the Quantity type.
For example, `base_dimension<tuple<meters>, tuple<seconds>>` interprets `meters` and `seconds` as delta units.
This default behavior is suitable for most basic applications where only changes or durations are relevant.

However, any fundamental unit can be wrapped by the `Quantity` type to indicate that the unit represents a state rather than a difference.
For example, `Quantity<Kelvin>` represents a specific temperature, whereas `Kelvin` alone represents a change in temperature.
Quantities are especially important when dealing with units like temperature where distinguishing between current temperatures and temperature differences is crucial.

*Note: Named-Compound dimensions, like `Joules`, `newtons`, `Liters`, etc. cannot (yet) be wrapped by `Quantity`. If this behavior is desired, please [open an issue](https://gitlab.com/dimensionalanalysis/dimensional/-/issues/new)*

### Conversions

When performing unit conversions, quantities and deltas handle offsets differently:

- **Quantity to Quantity Conversion**: Converting between two quantity units (e.g., `Quantity<Celsius>` to `Quantity<Kelvin>`) applies the offset. For instance, `0 C` converts to `273.15 K`.
- **Delta to Delta Conversion**: Converting between two delta units (e.g., delta `Celsius` to delta `Kelvin`) does not apply the offset. This means that a temperature difference of `10 C` is equivalent to a temperature difference of `10 K`.
Note: This distinction is only significant for units with non-zero offsets, such as temperature scales like `Celsius` and `Fahrenheit`.

### Addition and Subtraction Rules

Quantities and deltas interact differently during arithmetic operations. For any given dimension, the following rules apply:

#### Subtraction

- **Delta - Delta -> Delta**: Subtracting two differences yields another difference.
- **Quantity - Quantity -> Delta**: Subtracting two absolute quantities yields a difference.
- **Quantity - Delta -> Quantity**: Subtracting a difference from an absolute quantity results in an absolute quantity.
- **Delta - Quantity -> INVALID**: Subtracting an absolute quantity from a difference is undefined and results in a compilation error.

#### Addition

- **Delta + Delta -> Delta**: Adding two differences yields another difference.
- **Quantity + Delta -> Quantity**: Adding a difference to an absolute quantity results in an absolute quantity.
- **Delta + Quantity -> Quantity**: Adding an absolute quantity to a difference results in an absolute quantity.
- **Quantity + Quantity -> INVALID**: Adding two absolute quantities is undefined and results in a compilation error.

These rules **are maintained** for compound units. For example:
`base_dimension<tuple<meters>, tuple<Quantity<Kelvin>>> - base_dimension<tuple<meters>, tuple<Quantity<Kelvin>>>` -> `base_dimension<tuple<meters>, tuple<Kelvin>>`

### Absolute vs. Non-Absolute Units:

#### Absolute Units
These units have a defined zero point that represents the absence of the measurement.
For example, `Kelvin` and `Rankine` are absolute temperature scales where `0 K` and `0 R` represent absolute zero.

#### Non-Absolute Units
These units do not have an inherent zero point and are defined relative to another scale.
For example, `Celsius` and `Fahrenheit` are non-absolute temperature scales where `0 C` and `32 F` represent specific points on their respective scales, not the absence of temperature.

### Primary Unit Requirement

The `Primary` unit of any dimension must be an absolute unit. This ensures consistency and correctness in calculations involving absolute quantities.

### Constraints on Compound Dimensions

While compound dimensions can include non-absolute units, they cannot include non-absolute quantities.
This means that you cannot have a `Quantity` wrapping a non-absolute unit within a compound dimension.
**Invalid Example**:
```cpp
base_dimension<tuple<meters>, tuple<Quantity<Celsius>>> // INVALID
```

Here, `Quantity<Celsius>` is invalid because `Celsius` is a non-absolute unit.

### Multiplication and Division with Quantities

#### General Behavior

Multiplying or dividing units behaves similarly for both deltas and quantities in most scenarios.

#### Non-Absolute Quantities

When performing multiplication or division involving non-absolute quantities (e.g., `Quantity<Celsius>`), the unit will be implicitly cast to its `Primary` unit, which must be absolute, (e.g., `Kelvin`) during the operation.
- **Cancelling Out**: If the operation results in the non-absolute quantity being canceled out (e.g., multiplying by its reciprocal), the behavior is as expected.
- **Retained Units**: If the operation results in the non-absolute unit being retained in the result type, a warning will be emitted to alert the user of the potential inconsistency.

### Best Practices:

**Avoid Implicit Casting**: To maintain type consistency and avoid unexpected results, explicitly cast non-absolute quantities to an absolute unit before performing multiplication or division operations.
- Example:
```cpp
temperature<Quantity<Celsius>> currentTemp(25.0);

Time<seconds> time(20.0);

// This will emit a warning
base_dimension<tuple<Quantity<Kelvin>>, tuple<seconds>> resultBad = currentTemp / time; // Notice the Kelvin in the result type rather than Celsius

// This is preferred, no warning emitted
base_dimension<tuple<Quantity<Kelvin>>, tuple<seconds>> resultBad = temperature<Quantity<Kelvin>>(currentTemp) / time;
```

## Serialization

See [Serialization.md](Dimension/Serialization.md) for information on Serializing Dimension objects

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
`Dimensional` is under **active development** and is considered **stable for production use**. The library is ready for integration into your projects, but as capabilities are improved and expanded, minor updates and new features will be added regularly.

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