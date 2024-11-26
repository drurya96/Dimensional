# Usage

[[_TOC_]]

**Note: All types in `Dimensional` reside within the Dimension namespace. For simplicity, `using namespace Dimension;` is assumed in all examples.**

## Creating Dimension Objects

All Dimension objects can be created using the `BaseDimension` class.

Dimension objects store a `PrecisionType` (`double` by default, see [Precision.md]() for more details) value which
represents a physical quantity.

BaseDimension is templated on two tuples:
- `NumTuple`: A list of units in the numerator.
- `DenTuple`: A list of units in the denominator.

All combinations of units are valid. Simplification occurs internally in the library, but "unsimplified" units can be stored.

A `BaseDimension` of no units (both numerator and denominator are empty tuples) is also valid and is treated as a scalar.

### Verbose instantiation of simple dimension object
```cpp
BaseDimension<std::tuple<Meters>, std::tuple<>> myLength(25.0); // Represents 25.0 Meters
```

### Verbose instantiation of more complex dimension object
```cpp
BaseDimension<std::tuple<KiloGrams, Meters>, std::tuple<Seconds, Seconds>> myForce(25.0); // Represents 25.0 kg*m / s*s, or Newtons
```

User-friendly wrapper classes can be used to instantiate objects more simply.
Each "named dimension", like `Length`, `Time`, `Speed`, etc. can be instantiated with simplified syntax.

### Simplified instantiation of simple dimension object
```cpp
Length<Meters> myLength(25.0); // Represents 25.0 Meters
```

### Simplified instantiation of more complex dimension object
```cpp
Force<KiloGrams, Meters, Seconds, Seconds> myForce(25.0); // Represents 25.0 kg*m / s*s, or Newtons
```

Named unit combinations can also be provided to further simplify the syntax.
For example, `Newton` resolves to `kg*m / s*s`

### Further simplified instantiation of more complex dimension object
```cpp
Force<Newtons> myForce(25.0); // Represents 25.0 kg*m / s*s, or Newtons
```

## Casting between Dimension objects

Dimension objects that logically represent the same dimension (length, time, etc.), 
but are represented in different units are distinct types.

However, these types can be implicitly cast as needed, allowing the developer the flexibility to
treat the dimension similar to a type.

If a cast cannot occur due to incompatible dimensions, a compile-time error will be generated, ensuring safety.

### Simple implicit cast
```cpp
Length<Meters> originalLength(5.0); // 5 Meters
Length<Feet> newLength= originalLength; // ~16.4042 feet
```

This is especially useful when designing functions which accept dimensions as parameters or return dimension objects.

### Implicit cast in function parameter
```cpp
void printDimension(Length<Feet> length)
{
   std::cout << length << std::endl;
}

Length<Meters> myLength(5);

printDimension(myLength); // This will print "16.4042 [ft]"
```

### Implicit cast in function return
```cpp
Length<Meters> getLength()
{
   return Length<Meters>(5.0);
}

Length<Feet> myLength = getLength();
```

Because the storage of Dimension objects is simply a `double` (or the selected `PrecisionType`),
there is generally no performance benefit to passing these objects by reference.

While Dimension objects are not truly immutable, general guidance is to treat these objects as immutable.

---

Finally, template deduction guides are provided that allow instantion of a dimension without explicitly specifying its unit.

### Type deduction guide
```cpp
Length<Meters> getLength()
{
   return Length<Meters>(5.0);
}

Length myLength = getLength(); // This will resolve to a Length<Meters>
```

Generally, when template deduction is possible, one could simply use `auto`.
However, specifying the dimension name can improve code maintainability.

Note that type deduction is not possible in some context, such as non-static member initialization.

### Disallowed template deduction example
```cpp
Length<Meters> getLength()
{
   return Length<Meters>(5.0);
}

class MyClass 
{
    Length myLength = getLength; // Compilation error, the template parameter for Length cannot be deduced during initialization of a non-static member  
};
```

## Arithmetic

As a dimensional analysis library, arithmetic operations are at the core of `Dimensional`.

Operations involving dimension objects will produce the appropriate dimension type.

### Simple dimension arithmetic
```cpp
Length<Meters> length{25.0};
Time<Seconds> time{10};

Speed velocity = length / time; // Resolves to Speed<Meters, Seconds>
```

```cpp
Length<Meters> length1{25.0};
Length<Feet> length2{10.0};

Length sumLength = length1 + length2;
```

When units can simplify during arithmetic operations, they will be simplified.

This occurs when units of the same dimension appear in both the numerator and denominator.

### Simplification
```cpp
Speed<Meters, Seconds> speed{25.0};
Time<Minutes> time{10};

Length length = speed * time;
```

If all units cancel, the resulting "scalar" dimension object can be implicitly cast to a `double`.

### Scalar result
```cpp
Length<Meters> length1{25.0};
Length<Feet> length2{10.0};

double ratio = length1 / length2;
```

All dimensional analysis is resolved at compile-time.
If an invalid operation is attempted (e.g., adding a Length to a Time), the library will generate a compile-time error, ensuring correctness in your calculations.

## Available functions

Dimensional provides many functions similar to those in the C++ Standard Template Library (STL), ensuring familiarity and convenience for users working with dimension objects.
These functions operate directly on dimension types while preserving dimensional correctness.

- `Pow`: Raises a dimension to a positive integer power
  - Fractional powers are not yet supported, see [issue #34](https://gitlab.com/dimensionalanalysis/dimensional/-/issues/34) 
- `hypot`: Returns the hypotenuse using two sides of a right triangle
  - The two parameter types must match, and the return will be the same type
- `abs`: Returns the absolute value of a dimension object.
- `floor`: Rounds a dimension object down to the nearest integer value.
- `ceil`: Rounds a dimension object up to the nearest integer value.
- `round`: Rounds a dimension object to the nearest integer value.
- `modf`: Splits a dimension object into its integer and fractional parts.
- `fmod`: Computes the remainder of division between two dimension objects.

Dimensional provides trigonometric functions that operate on the Angle type or return a double for ratios:

- `sin`: Returns the sine of an Angle as a double.
- `cos`: Returns the cosine of an Angle as a double.
- `tan`: Returns the tangent of an Angle as a double.
- `asin`: Returns the Angle corresponding to a given sine ratio (double).
- `acos`: Returns the Angle corresponding to a given cosine ratio (double).
- `atan`: Returns the Angle corresponding to a given tangent ratio (double).
- `atan2`: Returns the Angle from two sides of a right triangle (as double inputs for y and x).

## Subscript types

`Dimensional` supports "Subscripting", meaning two instances of the same unit can be used in one dimension without being combined.

This can be useful for situations such as arithmetic that involves Liters of water and Liters of air at the same time.

More information to be added when possible and after more thorough testing.

## Access via `get` methods

While it is recommended to maintain dimension objects throughout a codebase,
sometimes retrieving the value as a `double` is strictly necessary.
To that end, getters are provided.

`BaseDimension` includes a `GetVal` member to retrieve the value as a double.
This method works for all dimensions, since the named dimensions inherit from `BaseDimension`.

`GetVal` is templated the same as `BaseDimension`, a `NumTuple` and `DenTuple`.

### Example GetVal
```cpp
Speed<Meters, Seconds> speed{25.0};

double speedAsDouble = speed.GetVal<std::tuple<Feet>, std::tuple<Minutes>>(); // Gets the value after converting to ft/min
```

**WARNING: Future versions MAY deprecate this method in favor of a `getVal` free function**
- If this occurs, the `GetVal` member function will remain in the library but deprecated until the next major release
- This has not yet been decided, but is being considered.

Named dimensions each have a custom getter with similar syntax.
These are free functions, with their member function counterparts deprecated.

### Simplified getters
```cpp
Length<Meters> length{25.0};
Time<Seconds> time{10};

double timeAsDouble = getTime<Minutes>(time);
double lengthAsDouble = getLength<Yards>(length);
```

## Setting values via `set` methods

Dimension objects should generally be treated as immutable to avoid unexpected
behavior in arithmetic operations.

However, the value of a dimension object can be modified using `SetVal` if needed.

`SetVal` is templated the same as `GetVal` and `BaseDimension`, a `NumTuple` and `DenTuple`.

### Set value of existing dimension
```cpp
Length<Meters> length(0.0);

length.SetVal<std::tuple<Feet>, std::tuple<>>(5.0); // Sets value based on 5 ft, or 1.524 meters
```

## Streaming

Dimension objects can be streamed using `<<` syntax to any `ostream` buffer.
The resulting stream will be formated as `"<value> [(numUnit1*numUnit2<*...>)/(denUnit1*denUnit2<*...>)]"`
- `"/"` will be omitted if the denominator is empty
- `"[]"` will be the unit for scalars
- Parenthesis will be omitted if only one unit is in either the numerator or denominator

### Streaming example
```cpp
Force<Newtons> force(10.0);

std::cout << force << std::endl; // prints "10.0 [(kg*m)/(s*s)]"
```

**For more usage examples, see the [Unit Tests](https://gitlab.com/dimensionalanalysis/dimensional/-/tree/main/Dimension/UnitTest?ref_type=heads).**
