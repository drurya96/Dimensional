# Changelog

All notable changes to this project will be documented in this file. Semantic versioning is followed.

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