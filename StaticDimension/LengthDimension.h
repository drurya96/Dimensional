#ifndef DIMENSION_LENGTH_H
#define DIMENSION_LENGTH_H

#include "BaseDimension.h"
#include <type_traits>
#include <tuple>

#define SINGLE_SPECIALIZE(From, To) \
   template <> \
   struct convertible<From, To> : std::true_type {}; \
   template <> \
   struct convertible<To, From> : std::true_type {};





namespace Dimension
{

   struct LengthType {};
   class Meters;

   template<typename Unit>
   class LengthUnit : public BaseUnit<LengthUnit<Unit>> 
   { 
   public: 
      using BaseUnit::BaseUnit;

      using Dim = LengthType;
      using Primary = Meters;

      Meters GetPrimary() const;
      /*
      Meters GetPrimary()
      {
         return ConvertValue<Unit, Meters>(*this);
         //return value;
      }
      */

   };

   class Meters : public LengthUnit<Meters> { public: using LengthUnit::LengthUnit; };
   class Feet : public LengthUnit<Feet> { public: using LengthUnit::LengthUnit; };
   class Inches : public LengthUnit<Inches> { public: using LengthUnit::LengthUnit; };

   template<typename Unit>
   inline Meters LengthUnit<Unit>::GetPrimary() const
   {
      return GetPrimaryImpl<Unit, Primary>(*(static_cast<const Unit*>(this)));
   }





   template<>
   inline Feet ConvertValue<Meters, Feet>(const Meters& obj)
   {
      std::cout << "Converting Meters to Feet using Direct Conversion..." << std::endl;
      return Feet(obj.GetValue() * 3.28084);
   }
   
   template<>
   inline Meters ConvertValue<Feet, Meters>(const Feet& obj)
   {
      return Meters(obj.GetValue() / 3.28084);
   }
   
   template<>
   inline Inches ConvertValue<Meters, Inches>(const Meters& obj)
   {
      return Inches(obj.GetValue() * 39.37);
   }

   template<>
   inline Meters ConvertValue<Inches, Meters>(const Inches& obj)
   {
      return Meters(obj.GetValue() / 39.37);
   }
   



   template<typename Unit = Meters>
   class Length : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<Unit::Dim, Meters::Dim>, "Unit provided does not derive from LengthUnit");
      using BaseDimension::BaseDimension;

      Length(double val) : BaseDimension(1.0, std::tuple<Unit>{val}, std::tuple<>{}) {}

      template<typename T>
      double GetLength()
      {
         return value * ConvertValue<Unit, T>(std::get<0>(numList)).GetValue();
      }

   };




}

#endif //DIMENSION_LENGTH_H