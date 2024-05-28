#ifndef LENGTH_EXTENSION_H
#define LENGTH_EXTENSION_H

#include "StaticLength.h"

namespace StaticDimension
{
   class Foo : public LengthUnit<Foo> { public: using LengthUnit::LengthUnit; };

   class Bar : public LengthUnit<Bar> { public: using LengthUnit::LengthUnit; };

   class Baz : public LengthUnit<Baz> { public: using LengthUnit::LengthUnit; };

   template<> struct Conversion<Meters, Foo> { static constexpr PrecisionType slope = 3.14; };
   template<> struct Conversion<Foo, Meters> { static constexpr PrecisionType slope = (1.0 / 3.14); };


   template<> struct Conversion<Meters, Bar> { static constexpr PrecisionType slope = 25.0; };
   template<> struct Conversion<Bar, Meters> { static constexpr PrecisionType slope = (1.0 / 25.0); };

   template<> struct Conversion<Meters, Baz>
   {
      static constexpr PrecisionType slope = (9.0/5.0);
      static constexpr PrecisionType offset = 32.0;
   };

   template<> struct Conversion<Baz, Meters> 
   { 
      static constexpr PrecisionType slope = (5.0/9.0);
      static constexpr PrecisionType offset = -17.777777778;
   };


   template<int I = 0>
   class BarSub : public Bar
   {
   public:
      using Bar::Bar;
      static constexpr int ID = I;
   };

   template<int I> struct Conversion<Meters, BarSub<I>> { static constexpr PrecisionType slope = Conversion<Meters, Bar>::slope; };
   template<int I> struct Conversion<BarSub<I>, Meters> { static constexpr PrecisionType slope = Conversion<Bar, Meters>::slope; };

}

#endif // LENGTH_EXTENSION_H
