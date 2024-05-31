#ifndef STATIC_DIMENSION_LENGTH_H
#define STATIC_DIMENSION_LENGTH_H

#include "StaticBaseDimension.h"

namespace StaticDimension
{
   struct LengthType {};
   struct Meters;

   template<typename Unit>
   struct LengthUnit : public BaseUnit
   { 
   public: 
      using BaseUnit::BaseUnit;

      using Dim = LengthType;
      using Primary = Meters;
   };

   struct Feet : public LengthUnit<Feet> { public: using LengthUnit::LengthUnit; };
   struct Inches : public LengthUnit<Inches> { public: using LengthUnit::LengthUnit; };
   struct Meters : public LengthUnit<Meters> { public: using LengthUnit::LengthUnit; };

   template<typename Unit = Meters>
   class Length : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<Unit::Dim, Meters::Dim>, "Unit provided does not derive from LengthUnit");
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Length() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      Length(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      template<typename T>
      Length(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.GetVal<std::tuple<Unit>, std::tuple<>>()){}


      template<typename T>
      double GetLength()
      {
         return this->GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   template<> struct Conversion<Meters, Feet> { static constexpr PrecisionType slope = 3.280839895; };
   template<> struct Conversion<Feet, Meters> { static constexpr PrecisionType slope = 0.304800000; };
   template<> struct Conversion<Meters, Inches> { static constexpr PrecisionType slope = 39.37; };
   template<> struct Conversion<Inches, Meters> { static constexpr PrecisionType slope = 0.0254000508; };
}

#endif //STATIC_DIMENSION_LENGTH_H