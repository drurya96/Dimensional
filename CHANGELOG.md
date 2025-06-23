# Changelog

All notable changes to this project will be documented in this file. Semantic versioning is followed.

## [2.6.2] - 2025-2-7

### Changed
- 

### Added
- 

### Deprecated
- 

### Removed
- 

### Fixed
- Fixed `operator<` for `StringLiteral` to specify `size_t` as the type for `std::min`

## [2.6.1] - 2025-2-3

### Changed
- 

### Added
- 

### Deprecated
- 

### Removed
- 

### Fixed
- Addition and Subtraction between quantities works as documented now
  - Error messages still aren't ideal, more improvements to follow


## [2.6.0] - 2025-2-3

### Changed
- Internal changes to address static analysis warnings
- Internal code generation mechanism updated

### Added
- Default template parameters for Fundamental and Derived dimensions
- CppCheck static analysis on pipelines
- Torque
- More Volume named units
  - FluidOunces, Cups, Pints, Quarts, Teaspoons, Tablespoons
- More Pressure named units
  - Atmospheres, Bar, Torr, mmHg, inHg
- JSON metadata encoding existing units
  - This is used internally for autogeneration, but may be useful to users

### Deprecated
- 

### Removed
- Removed Doxygen folder
  - And artifact will be available instead at a later date.

### Fixed
- Constructors from doubles now correctly marked `explicit`

## [2.5.0] - 2025-1-5

### Changed
- Automatic Delta handling removed
  - **DOUBLE CHECK TEMPERATURE MATH AFTER PULLING THIS VERSION!**
    - Likely didn't behave as expected before
  - Not a major version since this didn't behave as expected, this is a bugfix, but the concept has also been removed.
- `Kelvin` is the `Primary` temperature unit rather than `Celsius`
- Marked most functions `constexpr`
  - This enables compile-time evaluation when possible
  - Streaming and serialization are not marked `constexpr` 

### Added
- Quantity wrapper
  - This is a large, but non-breaking change. Strongly recommend reading the section.
  - See [documentation in README](./README.md#deltas-and-quantities)
- `is_absolute` to distinguish absolute from non-absolute units
- `Amount` as a fundamental unit, with `Moles` and `PoundMoles`
- `Energy` as a named compound unit with `Joules`, `Calories`, etc.
- `Pressure` as a named compound unit with `pascals`
- `DimensionalConstants.h`
  - Contains many universal and commonly used constants
- `DimensionalMolarmasses.h`
  - Contains the molar mass (as `Molarmass<Grams,Moles>`) of every element as well as some common compounds
- Python scripts to autogenerate Fundamental and Derived units
  - Generate fundamental dimensions by calling `python GenerateFundamentalDimension <dimension name> <primary unit>` (e.g. `python GenerateFundamentalDimension length meters`)
  - Generate derived dimensions by calling `python GenerateDerivedDimension <dimension name> <numerator> <denominator>` (e.g. `python GenerateFundamentalDimension force mass,length Time,Time`)
- Added fundamental dimension `charge` with base unit `Coulombs`
- Added many new derived dimensions

### Deprecated
- 

### Removed
- 

### Fixed
- Small value error in `Gallon` (factor changed from `0.15562` to `0.155849128`)

## [2.4.0] - 2024-11-26

### Changed
- Reorganized core library implementation further
- Renamed core library implementation as `Dimension_Core`
- Reorganized Dimension logic
  - Moved dimension logic to `Dimension_Impl`
  - Retained units at top-level

### Added
- Streaming operator to `ostream`
- Serialization/Deserialization
- Documentation for Contributing, Usage, Extension, and Serialization
- `StringLiteral` type and methods for handling string-like types at compile-time

### Deprecated
- 

### Removed
- 

### Fixed
- Bug-fix for unit conversions from a single denominator unit

## [2.3.0] - 2024-11-10
### Changed
- Reorganized implementation headers
  - `Dimension_Impl` contains implementations for each dimension, nested in either
    - `FundamentalDimensions` (mass, Time, etc.)
    - `DerivedDimensions` which are derived from Fundamental (speed, force, etc.)
- Constructor inheritance removed from units
  - Minimal effect since the BaseUnit constructor was already deleted.
- Units do not need to inherit from BaseUnit anymore, instead they simply must meet the concept `IsUnitType` 

### Added
- All units are templated on two string literals representing Name and Abbreviation
  - These values are available as constexpr

### Deprecated
- 

### Removed
- 

### Fixed
- 

## [2.2.0] - 2024-11-9

### Changed
- 

### Added
- Constructor from dimension to similar dimension of different units
- Getters for each named dimension as free functions.
  - These work on the named dimension and corresponding base_dimensions

### Deprecated
- Get<Dimension> member functions of named dimensions. These include
  - Getlength
  - GetTime
  - Getmass
  - Getangle
  - Gettemperature
  - Getspeed
  - Getacceleration
  - Getforce
  - GetVolume

### Removed
- 

### Fixed
- Corrected requirements for derived dimensions
- Added constraints for base_dimension functions

## [2.1.1] - 2024-11-3

### Changed
- **Internal**: Conversions in both direction (to/from) are not needed. Now, if a direct conversion exists in one direction, its inverse will be used for the other direction.
  - Direct conversions will stil be used if one exists.

### Added
- **Internal**: Unit tests for every fundamental conversion

### Deprecated
- 

### Removed
- 

### Fixed
- Instantiation of a `base_dimension` using any units that cannot be converted to the appropriate `Primary` unit results in a compiler error.
  - In previous behavior, the object could be instantiated but conversion would fail at compile time.
- Added include for `stdexcept`

## [2.1.0] - 2024-10-31

### Changed
- 

### Added
- Negative unary operator (allows `-base_dimension<...>` syntax)

### Deprecated
- 

### Removed
- 

### Fixed
- Constrained dimensions so overloads resolve correctly
  - Prior, overloaded functions that use different dimensions would fail due to ambiguous signatures.

## [2.0.0] - 2024-10-30

### Changed
- Renamed `Degree` to `Degrees` and `Radian` to `radians`
- Templated `BaseUnit` for CRTP
- "Derived" dimensions (newtons, Gallon, Liter, etc.) do not inherit from `DerivedUnit` in favor of CRTP in `BaseUnit`
- Split `DimensionUtilities` into several files under the directory `Dimension_Impl`
- Some metaprogramming structs now require a new template parameter (first position) indicating a comparison type-trait
  - For example, `std::is_same`, `dimension::is_same_dim`
  - Must have a `::value` field
  - Effected structs: `RemoveOneInstance`, `tuple_diff`, `get_first_match`
- Conan package passes `USE_DOUBLE` as compiler flag
  - This may later be made into an option
- **Internal**: Removed several type traits in favor of concepts, not used throughout.

### Added
- New concepts
  - `HasSlope`, `HasOffset`

### Deprecated
- 

### Removed
- C++17 Support
- `tuple_diff_dim` (in favor of `tuple_diff<is_same_dim, ...>`)

### Fixed
- Marked `Getacceleration` `const`

## [1.0.1] - 2024-10-24

### Changed
- Enabled C++17 support
  - Unintentionally broken due to usage of `std::numbers::pi` and an unguarded `requires`.
  
### Added
- Type trait for each dimension of the form `is_<dimension>`, such as `is_mass`.
- Concept for each dimension of the form `<dimension>_type`, such as `mass_type`.

### Deprecated
- C++17 support
  - Moving forward, C++20 will be the required standard until C++23 is adopted.

## [1.0.0] - 2024-10-20

- First Release