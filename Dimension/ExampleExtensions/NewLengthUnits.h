#ifndef LENGTH_EXTENSION_H
#define LENGTH_EXTENSION_H

#include "length_dimension.h"

namespace dimension
{
   class Foo : public lengthUnit<Foo, "Foo", "zzz"> {};

   class Bar : public lengthUnit<Bar, "Bar", "zzz"> {};

   class Baz : public lengthUnit<Baz, "Baz", "zzz"> {};

   class Fail : public lengthUnit<Fail, "Fail", "zzz"> {};


   template<> struct Conversion<Feet, Fail> { static constexpr PrecisionType slope = 3.14; };
   template<> struct Conversion<Fail, Feet> { static constexpr PrecisionType slope = (1.0 / 3.14); };

   template<> struct Conversion<meters, Foo> { static constexpr PrecisionType slope = 3.14; };
   template<> struct Conversion<Foo, meters> { static constexpr PrecisionType slope = (1.0 / 3.14); };

   template<> struct Conversion<meters, Bar> { static constexpr PrecisionType slope = 25.0; };
   template<> struct Conversion<Bar, meters> { static constexpr PrecisionType slope = (1.0 / 25.0); };

   template<> struct Conversion<meters, Baz>
   {
      static constexpr PrecisionType slope = (9.0/5.0);
      static constexpr PrecisionType offset = 32.0;
   };

   template<> struct Conversion<Baz, meters> 
   { 
      static constexpr PrecisionType slope = (5.0/9.0);
      static constexpr PrecisionType offset = -17.777777778;
   };

   template<int I = 0>
   class BarSub : public lengthUnit<BarSub<I>, "BarSub", "zzz", I>
   {
   public:
      using lengthUnit<BarSub<I>, "BarSub", "zzz", I>::lengthUnit;
   };

   template<int I> struct Conversion<meters, BarSub<I>> { static constexpr PrecisionType slope = Conversion<meters, Bar>::slope; };
   template<int I> struct Conversion<BarSub<I>, meters> { static constexpr PrecisionType slope = Conversion<Bar, meters>::slope; };

   /// @todo Move this to a different example
   struct grault
   {
      using NumTuple = std::tuple<Foo, Bar>;
      using DenTuple = std::tuple<Baz>;
   };
}

#endif // LENGTH_EXTENSION_H
