#ifndef STATIC_DIMENSION_MASS_H
#define STATIC_DIMENSION_MASS_H

#include "StaticBaseDimension.h"

namespace StaticDimension
{

   struct MassType {};
   class Grams;

   template<typename Unit>
   class MassUnit : public BaseUnit<MassUnit<Unit>> 
   { 
   public: 
      using BaseUnit<MassUnit<Unit>>::BaseUnit;

      using Dim = MassType;
      using Primary = Grams;

      Grams GetPrimary() const;

   };

   class Grams : public MassUnit<Grams> { public: using MassUnit::MassUnit; };
   class Pounds : public MassUnit<Pounds> { public: using MassUnit::MassUnit; };
   class Ounces : public MassUnit<Ounces> { public: using MassUnit::MassUnit; };

   template<typename Unit>
   inline Grams MassUnit<Unit>::GetPrimary() const
   {
      return GetPrimaryImpl<Unit>(*(static_cast<const Unit*>(this)));
   }





   template<>
   inline Pounds ConvertValue<Grams, Pounds>(const Grams& obj)
   {
      return Pounds(obj.GetValue() * 0.0022046226);
   }
   
   template<>
   inline Grams ConvertValue<Pounds, Grams>(const Pounds& obj)
   {
      return Grams(obj.GetValue() / 0.0022046226);
   }
   
   template<>
   inline Ounces ConvertValue<Grams, Ounces>(const Grams& obj)
   {
      return Ounces(obj.GetValue() * 0.0352739619);
   }

   template<>
   inline Grams ConvertValue<Ounces, Grams>(const Ounces& obj)
   {
      return Grams(obj.GetValue() / 0.0352739619);
   }
   



   template<typename Unit = Grams>
   class Mass : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<Unit::Dim, Grams::Dim>, "Unit provided does not derive from MassUnit");
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Mass() : BaseDimension<std::tuple<Unit>, std::tuple<>>(1.0, std::tuple<Unit>{0.0}, std::tuple<>{}) {}

      Mass(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>(1.0, std::tuple<Unit>{val}, std::tuple<>{}) {}

      template<typename T>
      Mass(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>(base.GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<typename T>
      double GetMass()
      {
         return this->scalar * ConvertValue<Unit, T>(this->GetNumUnit()).GetValue();
      }
   };
}

#endif //STATIC_DIMENSION_MASS_H