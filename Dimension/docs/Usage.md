# Usage

[[_TOC_]]

**Note: All types in `Dimensional` reside within the Dimension namespace. For simplicity, `using namespace dimension;` is assumed in all examples.**

## Creating Dimension Objects

All Dimension objects can be created using the `base_dimension` class.

Dimension objects store a `PrecisionType` (`double` by default, see [Precision.md]() for more details) value which
represents a physical quantity.

base_dimension is templated on two tuples:
- `NumTuple`: A list of units in the numerator.
- `DenTuple`: A list of units in the denominator.

All combinations of units are valid. Simplification occurs internally in the library, but "unsimplified" units can be stored.

A `base_dimension` of no units (both numerator and denominator are empty tuples) is also valid and is treated as a scalar.

### Verbose instantiation of simple dimension object
```cpp
base_dimension<std::tuple<meters>, std::tuple<>> mylength(25.0); // Represents 25.0 meters
```

### Verbose instantiation of more complex dimension object
```cpp
base_dimension<std::tuple<KiloGrams, meters>, std::tuple<seconds, seconds>> myforce(25.0); // Represents 25.0 kg*m / s*s, or newtons
```

User-friendly wrapper classes can be used to instantiate objects more simply.
Each "named dimension", like `length`, `Time`, `speed`, etc. can be instantiated with simplified syntax.

### Simplified instantiation of simple dimension object
```cpp
length<meters> mylength(25.0); // Represents 25.0 meters
```

### Simplified instantiation of more complex dimension object
```cpp
force<KiloGrams, meters, seconds, seconds> myforce(25.0); // Represents 25.0 kg*m / s*s, or newtons
```

Named unit combinations can also be provided to further simplify the syntax.
For example, `newtons` resolves to `kg*m / s*s`

### Further simplified instantiation of more complex dimension object
```cpp
force<newtons> myforce(25.0); // Represents 25.0 kg*m / s*s, or newtons
```

## Casting between Dimension objects

Dimension objects that logically represent the same dimension (length, time, etc.), 
but are represented in different units are distinct types.

However, these types can be implicitly cast as needed, allowing the developer the flexibility to
treat the dimension similar to a type.

If a cast cannot occur due to incompatible dimensions, a compile-time error will be generated, ensuring safety.

### Simple implicit cast
```cpp
length<meters> originallength(5.0); // 5 meters
length<Feet> newlength= originallength; // ~16.4042 feet
```

This is especially useful when designing functions which accept dimensions as parameters or return dimension objects.

### Implicit cast in function parameter
```cpp
void printDimension(length<Feet> length)
{
   // Do logic with length
}

length<meters> mylength(5);

printDimension(mylength); // This will print "16.4042 [ft]"
```

### Implicit cast in function return
```cpp
length<meters> getlength()
{
   return length<meters>(5.0);
}

length<Feet> mylength = getlength();
```

Because the storage of Dimension objects is simply a `double` (or the selected `PrecisionType`),
there is generally no performance benefit to passing these objects by reference.

While Dimension objects are not truly immutable, general guidance is to treat these objects as immutable.

---

Finally, template deduction guides are provided that allow instantiation of a dimension without explicitly specifying its unit.

### Type deduction guide
```cpp
length<meters> getlength()
{
   return length<meters>(5.0);
}

length mylength = getlength(); // This will resolve to a length<meters>
```

Generally, when template deduction is possible, one could simply use `auto`.
However, specifying the dimension name can improve code maintainability.

Note that type deduction is not possible in some context, such as non-static member initialization.

### Disallowed template deduction example
```cpp
length<meters> getlength()
{
   return length<meters>(5.0);
}

class MyClass 
{
    length mylength = getlength; // Compilation error, the template parameter for length cannot be deduced during initialization of a non-static member  
};
```

## Dimensions as Parameters

When writing a function that takes a Dimension object as a parameter, consider the following approaches.

In all examples, signatures will be presented as if they are within a header, where `using name Dimension` would generally be discouraged.

### Dimension of Pre-Defined Unit

A function may accept a dimension of a pre-defined unit.
This allows the declaration to exist in the header, while the definition may be pre-compiled, reducing compile times for large projects.
However, calling this function with a different unit will invoke an implicit cast. This is expected, but does incur a runtime performance and precision cost.
The cost is small per-call, but in highly performance- or precision-critical contexts, this may not be ideal.

#### Fully Qualified Dimension

Fully defining the dimension in terms of units is highly verbose, but it creates a very clear API for the function.
This is especially useful when the function uses the object in terms of the defined unit.
For example, if within the function body, `getlength<meters>(obj)` is called, especially if its called multiple times, defining the unit can improve performance.

Unfortunately, for dimensions with longer names (i.e. `dimension::force<dimension::KiloGrams, dimension::meters, dimension::seconds, dimension::seconds>`), this approach is very verbose.

```cpp
void MyFunction(dimension::length<dimension::meters> obj);
```

#### Dimension with default unit

Dimensions have default template parameters for their units. This can be leveraged in function signatures to "ignore" the unit.
To the compiler, this is still a fully-qualified type. There are no compile-time or runtime costs or savings for this approach compared to the previous.
Use a Dimension with default unit(s) when the function does not need any particular unit, **and** the function won't be called in highly performance or precision sensitive contexts.

Typically, this form is ideal for *thinking* about the object in terms of its dimension, rather than its unit. "My function expects a length, but it doesn't matter what kind".
While the compiler treats this as a fully defined type, using the default template parameters abstracts the unit knowledge away from the author and caller of the function.
Whether this is seen as "good" or "bad" is at the discretion of the user.

```cpp
void MyFunction(dimension::length<> obj);
```

### Template dimensions

A function may accept a template-based Dimension.
In this case, typically the function definition must also exist in the header (i.e. inline), though it is possible to specialize the function in a source file that is pre-compiled.
A function template may use a dimension of any unit, resulting in maximum runtime performance and precision since no casting is needed.
However, invoking the function template with many different units and/or in many compilation units will incur some compile-time costs.
Changes to the implementation of these functions may also trigger re-compilation of dependent compilation units.
For functions that are not performance or precision critical, this may not be ideal for large projects.

#### Dimension templated on unit

The unit can be provided as a template parameter.
While the template will accept any unit, only those that satisfy the built-in constraints of the dimension will be allowed.

```cpp
template<typename T>
void MyFunction(dimension::length<T> obj){/*implementation*/}
```

#### Constrained templated dimension

The dimension itself can be a template parameter.
Using the `is_length` or similar concepts, the function template can be constrained to only accept length types.
This is functionally very similar to templating on the unit, however there is a small compile-time cost to checking the constraint.

The choice between these template approaches is predominately stylistic (with very slightly faster compile times when templating on the unit(s)).

```cpp
void MyFunction(dimension::is_length auto obj){/*implementation*/}
```

**Note**: This is the same as:
```cpp
template<dimension::is_length T>
void MyFunction(T obj){/*implementation*/}
```
and
```cpp
template<typename T>
requires dimension::is_length<T>
void MyFunction(T obj){/*implementation*/}
```

## Arithmetic

As a dimensional analysis library, arithmetic operations are at the core of `Dimensional`.

Operations involving dimension objects will produce the appropriate dimension type.

### Simple dimension arithmetic
```cpp
length<meters> length{25.0};
Time<seconds> time{10};

speed velocity = length / time; // Resolves to speed<meters, seconds>
```

```cpp
length<meters> length1{25.0};
length<Feet> length2{10.0};

length sumlength = length1 + length2;
```

When units can simplify during arithmetic operations, they will be simplified.

This occurs when units of the same dimension appear in both the numerator and denominator.

### Simplification
```cpp
speed<meters, seconds> speed{25.0};
Time<minutes> time{10};

length length = speed * time;
```

If all units cancel, the resulting "scalar" dimension object can be implicitly cast to a `double`.

### Scalar result
```cpp
length<meters> length1{25.0};
length<Feet> length2{10.0};

double ratio = length1 / length2;
```

All dimensional analysis is resolved at compile-time.
If an invalid operation is attempted (e.g., adding a length to a Time), the library will generate a compile-time error, ensuring correctness in your calculations.

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

Dimensional provides trigonometric functions that operate on the angle type or return a double for ratios:

- `sin`: Returns the sine of an angle as a double.
- `cos`: Returns the cosine of an angle as a double.
- `tan`: Returns the tangent of an angle as a double.
- `asin`: Returns the angle corresponding to a given sine ratio (double).
- `acos`: Returns the angle corresponding to a given cosine ratio (double).
- `atan`: Returns the angle corresponding to a given tangent ratio (double).
- `atan2`: Returns the angle from two sides of a right triangle (as double inputs for y and x).

## Subscript types

`Dimensional` supports "Subscripting", meaning two instances of the same unit can be used in one dimension without being combined.

This can be useful for situations such as arithmetic that involves Liters of water and Liters of air at the same time.

More information to be added when possible and after more thorough testing.

## Access via `get` methods

While it is recommended to maintain dimension objects throughout a codebase,
sometimes retrieving the value as a `double` is strictly necessary.
To that end, getters are provided.

`base_dimension` includes a `GetVal` member to retrieve the value as a double.
This method works for all dimensions, since the named dimensions inherit from `base_dimension`.

`GetVal` is templated the same as `base_dimension`, a `NumTuple` and `DenTuple`.

### Example GetVal
```cpp
speed<meters, seconds> speed{25.0};

double speedAsDouble = speed.GetVal<std::tuple<Feet>, std::tuple<minutes>>(); // Gets the value after converting to ft/min
```

**WARNING: Future versions MAY deprecate this method in favor of a `getVal` free function**
- If this occurs, the `GetVal` member function will remain in the library but deprecated until the next major release
- This has not yet been decided, but is being considered.

Named dimensions each have a custom getter with similar syntax.
These are free functions, with their member function counterparts deprecated.

### Simplified getters
```cpp
length<meters> length{25.0};
Time<seconds> time{10};

double timeAsDouble = getTime<minutes>(time);
double lengthAsDouble = getlength<Yards>(length);
```

## Setting values via `set` methods

Dimension objects should generally be treated as immutable to avoid unexpected
behavior in arithmetic operations.

However, the value of a dimension object can be modified using `SetVal` if needed.

`SetVal` is templated the same as `GetVal` and `base_dimension`, a `NumTuple` and `DenTuple`.

### Set value of existing dimension
```cpp
length<meters> length(0.0);

length.SetVal<std::tuple<Feet>, std::tuple<>>(5.0); // Sets value based on 5 ft, or 1.524 meters
```

## Streaming

Dimension objects can be streamed using `<<` syntax to any `ostream` buffer.
The resulting stream will be formatted as `"<value> [(numUnit1*numUnit2<*...>)/(denUnit1*denUnit2<*...>)]"`
- `"/"` will be omitted if the denominator is empty
- `"[]"` will be the unit for scalars
- Parenthesis will be omitted if only one unit is in either the numerator or denominator

### Streaming example
```cpp
force<newtons> force(10.0);

std::cout << force << std::endl; // prints "10.0 [(kg*m)/(s*s)]"
```

**For more usage examples, see the [Unit Tests](https://gitlab.com/dimensionalanalysis/dimensional/-/tree/main/Dimension/UnitTest?ref_type=heads).**
