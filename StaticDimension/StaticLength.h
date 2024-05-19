#ifndef STATIC_DIMENSION_LENGTH_H
#define STATIC_DIMENSION_LENGTH_H

#include "StaticBaseDimension.h"

namespace StaticDimension
{
   struct LengthType {};
   class Meters;

   template<typename Unit>
   class LengthUnit : public BaseUnit<LengthUnit<Unit>> 
   { 
   public: 
      using BaseUnit<LengthUnit<Unit>>::BaseUnit;

      using Dim = LengthType;
      using Primary = Meters;

      Primary GetPrimary() const;
   };

   class Feet : public LengthUnit<Feet> { public: using LengthUnit::LengthUnit; };
   class Inches : public LengthUnit<Inches> { public: using LengthUnit::LengthUnit; };
   class Meters : public LengthUnit<Meters> { public: using LengthUnit::LengthUnit; };


   template<typename Unit>
   inline typename LengthUnit<Unit>::Primary LengthUnit<Unit>::GetPrimary() const
   {
      return GetPrimaryImpl<Unit>(*(static_cast<const Unit*>(this)));
   }

   template<>
   inline Feet ConvertValue<Meters, Feet>(const Meters& obj)
   {
      return Feet(obj.GetValue() * 3.280839895);
   }
   
   template<>
   inline Meters ConvertValue<Feet, Meters>(const Feet& obj)
   {
      return Meters(obj.GetValue() / 3.280839895);
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
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Length() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(1.0, std::tuple<Unit>{0.0}, std::tuple<>{}) {}

      Length(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(1.0, std::tuple<Unit>{val}, std::tuple<>{}) {}

      template<typename T>
      Length(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.GetVal<std::tuple<Unit>, std::tuple<>>()){}


      template<typename T>
      double GetLength()
      {
         return this->scalar * ConvertValue<Unit, T>(this->GetNumUnit()).GetValue();
      }
   };
}

#endif //STATIC_DIMENSION_LENGTH_H