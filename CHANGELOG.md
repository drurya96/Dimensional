# Changelog

All notable changes to this project will be documented in this file. Semantic versioning is followed.

## [Unreleased]

### Changed
- 

### Added
- 

### Deprecated
- 

### Removed
- 

### Fixed
- 

## [2.3.0] - 2024-11-10
### Changed
- Reorganized implementation headers
  - `Dimension_Impl` contains implementations for each dimension, nested in either
    - `FundamentalDimensions` (Mass, Time, etc.)
    - `DerivedDimensions` which are derived from Fundamental (Speed, Force, etc.)
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
  - These work on the named dimension and corresponding BaseDimensions

### Deprecated
- Get<Dimension> member functions of named dimensions. These include
  - GetLength
  - GetTime
  - GetMass
  - GetAngle
  - GetTemperature
  - GetSpeed
  - GetAcceleration
  - GetForce
  - GetVolume

### Removed
- 

### Fixed
- Corrected requirements for derived dimensions
- Added constraints for BaseDimension functions

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
- Instantiation of a `BaseDimension` using any units that cannot be converted to the appropriate `Primary` unit results in a compiler error.
  - In previous behavior, the object could be instantiated but conversion would fail at compile time.
- Added include for `stdexcept`

## [2.1.0] - 2024-10-31

### Changed
- 

### Added
- Negative unary operator (allows `-BaseDimension<...>` syntax)

### Deprecated
- 

### Removed
- 

### Fixed
- Constrained dimensions so overloads resolve correctly
  - Prior, overloaded functions that use different dimensions would fail due to ambiguous signatures.

## [2.0.0] - 2024-10-30

### Changed
- Renamed `Degree` to `Degrees` and `Radian` to `Radians`
- Templated `BaseUnit` for CRTP
- "Derived" dimensions (Newtons, Gallon, Liter, etc.) do not inherit from `DerivedUnit` in favor of CRTP in `BaseUnit`
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
- Marked `GetAcceleration` `const`

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