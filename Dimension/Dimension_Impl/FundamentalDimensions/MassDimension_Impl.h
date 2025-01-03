#ifndef STATIC_DIMENSION_MASS_IMPL_H
#define STATIC_DIMENSION_MASS_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{

   struct MassType {};
   struct Grams;

   template<typename T>
   concept IsMassUnit = IsNonQuantityUnitDimension<T, MassType> || IsQuantityUnitDimension<T, MassType>;

   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct MassUnit : public BaseUnit<Unit, Name, Abbreviation, "Mass">
   { 
   public: 
      using Dim = MassType;
      using Primary = Grams;
   };

   template<typename T>
   struct is_mass : std::false_type {};

   template<typename T>
   struct is_mass<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsMassUnit<T>> {};

   template<typename T>
   constexpr bool is_mass_v = is_mass<T>::value;

   template<typename T>
   concept mass_type = is_mass_v<T>;

   template<IsMassUnit T>
   constexpr PrecisionType getMass(mass_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   template<IsMassUnit Unit>
   class Mass : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      constexpr Mass() : BaseDimension<std::tuple<Unit>, std::tuple<>>(0.0) {}

      constexpr Mass(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>(val) {}

      template<IsMassUnit T>
      constexpr Mass(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<IsMassUnit T>
      [[deprecated("Use the free function getMass() instead.")]]
      double GetMass() const
      {
         return getMass<T>(*this);
      }
   };

   template<IsMassUnit T>
   Mass(T) -> Mass<T>;

   template<IsMassUnit MassUnit>
   Mass(BaseDimension<std::tuple<MassUnit>, std::tuple<>>) -> Mass<MassUnit>;

   template<typename T>
   struct is_mass<Mass<T>> : std::bool_constant<IsMassUnit<T>> {};

}

#endif //STATIC_DIMENSION_MASS_IMPL_H
