#ifndef STATIC_DIMENSION_TEMPERATURE_IMPL_H
#define STATIC_DIMENSION_TEMPERATURE_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   struct TemperatureType {};
   struct Kelvin;

   template<typename T>
   concept IsTemperatureUnit = IsNonQuantityUnitDimension<T, TemperatureType> || IsQuantityUnitDimension<T, TemperatureType>;

   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct TemperatureUnit : public BaseUnit<Unit, Name, Abbreviation, "Temperature">
   {
   public:
      using Dim = TemperatureType;
      using Primary = Kelvin;
   };

   template<typename T>
   struct is_temperature : std::false_type {};

   template<typename T>
   struct is_temperature<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsTemperatureUnit<T>> {};

   template<typename T>
   constexpr bool is_temperature_v = is_temperature<T>::value;

   template<typename T>
   concept temperature_type = is_temperature_v<T>;

   template<IsTemperatureUnit T>
   constexpr PrecisionType getTemperature(temperature_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   template<IsTemperatureUnit Unit>
   class Temperature : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      constexpr Temperature() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      constexpr Temperature(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      template<IsTemperatureUnit T>
      constexpr Temperature(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<IsTemperatureUnit T>
      [[deprecated("Use the free function getTemperature() instead.")]]
      double GetTemperature() const
      {
         return getTemperature<T>(*this);
      }
   };

   template<IsTemperatureUnit T>
   Temperature(T) -> Temperature<T>;

   template<IsTemperatureUnit TemperatureUnit>
   Temperature(BaseDimension<std::tuple<TemperatureUnit>, std::tuple<>>) -> Temperature<TemperatureUnit>;

   template<typename T>
   struct is_temperature<Temperature<T>> : std::bool_constant<IsTemperatureUnit<T>> {};

}

#endif //STATIC_DIMENSION_TEMPERATURE_IMPL_H
