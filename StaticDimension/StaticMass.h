#ifndef STATIC_DIMENSION_MASS_H
#define STATIC_DIMENSION_MASS_H

#include "StaticBaseDimension.h"

namespace StaticDimension
{

   struct MassType {};
   struct Grams;

   template<typename Unit>
   struct MassUnit : public BaseUnit
   { 
   public: 
      using BaseUnit::BaseUnit;

      using Dim = MassType;
      using Primary = Grams;
   };

   struct Grams : public MassUnit<Grams> { public: using MassUnit::MassUnit; };
   struct Pounds : public MassUnit<Pounds> { public: using MassUnit::MassUnit; };
   struct Ounces : public MassUnit<Ounces> { public: using MassUnit::MassUnit; };

   template<typename Unit = Grams>
   class Mass : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<Unit::Dim, Grams::Dim>, "Unit provided does not derive from MassUnit");
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Mass() : BaseDimension<std::tuple<Unit>, std::tuple<>>(0.0) {}

      Mass(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>(val) {}

      template<typename T>
      Mass(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>(base.GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<typename T>
      double GetMass()
      {
         return this->GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   /*
   template<>
   inline double Convert<Grams, Pounds>(double input)
   {
      return input * 0.0022046226;
   }

   template<>
   inline double Convert<Pounds, Grams>(double input)
   {
      return input / 0.0022046226;
   }

   template<>
   inline double Convert<Grams, Ounces>(double input)
   {
      return input * 0.0352739619;
   }

   template<>
   inline double Convert<Ounces, Grams>(double input)
   {
      return input / 0.0352739619;
   }
   */

   template<> struct Conversion<Grams, Pounds> { static constexpr PrecisionType slope = 0.0022046226; };
   template<> struct Conversion<Pounds, Grams> { static constexpr PrecisionType slope = 453.5923745; };
   template<> struct Conversion<Grams, Ounces> { static constexpr PrecisionType slope = 0.0352739619; };
   template<> struct Conversion<Ounces, Grams> { static constexpr PrecisionType slope = 28.349523165; };

}

#endif //STATIC_DIMENSION_MASS_H