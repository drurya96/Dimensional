# Extending Dimensional

`Dimensional` is designed to be highly extensible, allowing users to customize dimensions to meet their needs.

This document covers how to add new units, dimensions, conversions, and more to tailor Dimensional to your specific needs.

**`Dimensional` source does not need to be edited to add any functionality described in this document!**

When extending `Dimensional`, consider contributing the change to the open-source library following [the Contributing Guidelines](../CONTRIBUTING.md)

[[_TOC_]]

## Direct Floating Point Conversion

`Dimensional` is intended to be used for type-safety throughout a codebase.
However, situations arise in which simply converting a `double` that represents one unit to
a `double` that represents a different unit is strictly necessary.

`Dimensional` **does not** provide this capability natively, but `Dimensional` can still be leveraged to achieve this goal.

Adding the following templated functions to any header that is then made available in your codebase achieves direct floating-point conversion.

```cpp
template<typename FromNum, typename FromDen, typename ToNum, typename ToDen>
double Convert(double val)
{
   return BaseDimension<FromNum, FromDen>(val).GetVal<ToNum, ToDen>();
}
```

For more concise usage, this additional function reduces so boilerplate code for common cases.

```cpp
template<typename FromUnit, typename ToUnit>
double Convert(double val)
{
   return Convert<std::tuple<FromUnit>, std::tuple<>, std::tuple<ToUnit>, std::tuple<>>(val);
}
```

Then, conversions can occur as follows:

```cpp
double myLengthInMeters = 25.0;
double myLengthInFeet = Convert<Meters, Feet>(myLengthInMeters);


double mySpeedInMetersPerSecond = 25.0;
double mySpeedInMilesPerHour = Convert<std::tuple<Meters>, std::tuple<Seconds>, std::tuple<Miles>, std::tuple<Hour>>(mySpeedInMetersPerSecond);
```

These functions bypass type safety for direct floating-point conversions but still leverage the library's underlying conversion mechanisms to ensure correctness.

## User-Defined Literals

User-defined literals can introduce more concise syntax.
However, user-defined literals are not packaged with `Dimensional`.
This design decision was made from a few factors:
- As the library grows, ensuring no conflicts emerge may become unmanageable.
- When users add new units, they must ensure no conflicts exist with existing units, including when pulling updates
- Users may prefer different literals for niche use-cases

Instead, users of `Dimensional` are encouraged to add user-defined literals to meet their needs.

### User-Defined Literal Example

To avoid naming conflicts, it is strongly recommended to wrap user-defined literals in a dedicated namespace.

```cpp
constexpr Length<Meters> operator"" _m (PrecisionType);
constexpr Length<Feet>   operator"" _ft(PrecisionType);
constexpr Length<Inches> operator"" _in(PrecisionType);
```

Usage:
```cpp
Length<Meters> length1 = 25.0_m;
Length length2 = 10.0_ft;
auto length3 = 5.0_in;
```

## Adding a Direct Conversion

In `Dimensional`, all units of a given dimension are considered "convertible" because a conversion must exist connecting each unit to
a `PrimaryUnit` of its dimension.

However, if a direct conversion exists between two units, it will be used rather than "stepping through" the `PrimaryUnit`.

Most direct conversions are not currently packaged in `Dimensional`. Direct conversions can/will be added as the library grows.

Adding a direct conversion improves runtime performance and precision by avoiding intermediate conversions through the PrimaryUnit.

To add a direct conversion, simply add a specialization of the `Conversion` struct to any header. Note this needs to be in the `Dimension` namespace.

### Add direct conversion from Inches to CentiMeters
```cpp
namespace Dimension
{
   template<> struct Conversion<Inches, CentiMeters> { static constexpr PrecisionType slope = 2.54; };
}
```

As long as the compiler can find this specialization, it will be used automatically by the `Dimensional` library.

## Adding a New Unit to Existing Dimension

Adding a new unit to an existing dimension simply requires creating a new struct for the unit, which inherits from the corresponding unit type.
This unit type is templated on the new struct (CRTP), along with two strings representing the unit name and abbreviation, respectively.
- Note: CRTP (Curiously Recurring Template Pattern) allows for generic management of units by inheriting from a type templated on the inherited type.
These strings are used for streaming and serialization operations, but otherwise are not used in the library.

Conflicting strings may cause problems with serialization, but unless serialization capabilities are used, no conflicts should arise.

Next, a conversion must be provided (see [above]()) to convert to or from the `PrimaryUnit`. Only one direction is strictly required, and either works.

### Add `Yards` unit to `Length`
Note `Yards` already exists in `Length` this is just for demonstration purposes.
```cpp
namespace Dimension
{
   struct Yards : public LengthUnit<Yards, "Yards", "yd"> {};

   template<> struct Conversion<Yards, Meters> { static constexpr PrecisionType slope = 0.9144; }; // NIST
}
```

## Adding a New Derived Dimension

Dimensions which represent combinations of other dimensions are considered "Derived".

### Autogeneration

The simplest way to add a new derived dimension is by calling the python-based autogeneration script

`python GenerateDerivedDimension <dimension name> <numerator dimension list> <denominator dimension list>` 

For example: `python GenerateFundamentalDimension Force Mass,Length Time,Time`

If the numerator or denominator are empty, use `[]`.

For example: `python GenerateFundamentalDimension Frequency [] Time`

### Alias for Derived Dimension

For simple usage, a `using` alias is sufficient to get started

Note `Force` already exists in dimension, this is for demonstration purposes.

```cpp
template<typename Mass, typename Length, typename Time1, typename Time2>
using Force = BaseDimension<std::tuple<Mass, Length>, std::tuple<Time1, Time2>>;

Force<KiloGrams, Meters, Seconds, Seconds> force(25.0); // 25.0 kg*m/s*s, or Newtons
```

Using this approach, some features of `Dimensional` may not behave as expected with `Force`.
Type-deduction will not function, and constraints are not applied to restrict the types to only those
appropriate for a force value.

### Full definition of Derived Dimension

For full `Dimensional` support, a new derived dimension should instead inherit from the appropriate `BaseDimension` type.
Within this class, constraints should be added enforcing only the appropriate unit types are valid.

Class template argument deduction guides can be added to allow class template deduction.

Constructors should be added both for construction from a floating-point value and a conversion constructor from other `BaseDimension` types
that adhere to the same constraints.

See [Force implementation](Dimension_Impl/DerivedDimensions/ForceDimension_Impl.h) for a detailed example of a derived dimension

## Adding a New Fundamental Dimension

Fundamental dimensions are those that are not composed of any other dimensions.
Currently, the following fundamental dimensions exist in `Dimensional`
- Angle
- Length
- Mass
- Temperature
- Time

New fundamental dimensions (such as Charge (or maybe Current? Unclear at this point) and Luminous Intensity) may be added over time.

A unit type struct should inherit from `BaseUnit` (not strictly necessary, so long as constraints are met).

This unit type must include a `Dim` and a `Primary` type.
`Dim` must be a struct which represents the new dimension. This struct is a type-trait and does not need anymore information.
`Primary` must be the "Primary" unit of the dimension.
- All units of this dimension must be directly convertible to or from the `Primary` unit.

A new class can be inherited from `BaseDimension` similar to the Derived Dimension above.

See [existing fundamental dimensions](Dimension_Impl/FundamentalDimensions/) as examples.

## Adding a New Named-Unit Combination

Named combinations of units can be added to allow cleaner syntax for common, complex units.

For example, `Newtons` is much more concise than `kg*m/s*s`

These only apply to "Derived Dimensions" since they are combinations of units.

These simply exist as structs which contain types `NumTuple` and `DenTuple` for the correct units.
As long as the struct is available where it is being used, it will be fully functional in `Dimensional`.

### Add Named-Unit Combination

```cpp
   struct Newton
   {
      using NumTuple = std::tuple<KiloGrams, Meters>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };
```

----

For more examples of adding Dimensions, Units, Conversions, and Literals, see [the Example Extensions](ExampleExtensions/) directory.
