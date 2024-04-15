# Dimensional

**WORK IN PROGRESS**

This library is in very early development and is not yet ready for any form of release.

**The following documentatin is GitLab generated placeholder and is not yet populated**

## Description
Dimensional is a Physical Unit of Measure library with a heavy emphasis on storing data as a Dimension, rather than a Unit.

Using this library, developers may create objects any Dimension. When creating these objects, the Unit being used must be known,
but object is intended to be used from that point forward without the knowledge of the unit used to create the object.

Strong typing is critical in Dimensional; inappropriate operations lead to compile-time errors rather than runtime crashes.

### Fundamental and Derived Units
Dimensions come in two flavors in `Dimensional` - Fundamental and Derived.

Fundamental dimensions are those that are not made up of other dimensions, such as `Length`, `Time`, and `Mass`.

Derived dimensions are those that are made up of some number of Fundamental dimensions, like `Speed`, `Acceleration`, and `Force`

In Dimensions, a Dimension object can be created using any combination of Units, but dedicated Derived dimensions can improve
code readibility.

## Usage
For now, see [the provided unit tests for usage](https://gitlab.com/drurya96/dimensional/-/tree/main/UnitTest).

**Detailed Usage to be added soon**

## Local Modifications

## Writing Issues

## Contributing

## License
This library is open-source, and is licensed under [the MIT license](https://gitlab.com/drurya96/dimensional/-/blob/main/LICENSE)

## Project status
Active development.

This library is still in early development.
