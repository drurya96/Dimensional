#ifndef STATIC_DIMENSION_TEMPERATURE_H
#define STATIC_DIMENSION_TEMPERATURE_H

#include "BaseDimension.h"

namespace Dimension
{
   struct TemperatureType {};
   struct Celsius;

   template<typename TemperatureUnit>
   concept IsTemperatureUnit = std::is_same_v<typename TemperatureUnit::Dim, TemperatureType>;

   template<typename Unit>
   struct TemperatureUnit : public BaseUnit<Unit>
   {
   public:
      using BaseUnit<Unit>::BaseUnit;

      using Dim = TemperatureType;
      using Primary = Celsius;
   };

   struct Celsius : public TemperatureUnit<Celsius> { public: using TemperatureUnit::TemperatureUnit; };
   struct Fahrenheit : public TemperatureUnit<Fahrenheit> { public: using TemperatureUnit::TemperatureUnit; };
   struct Kelvin : public TemperatureUnit<Kelvin> { public: using TemperatureUnit::TemperatureUnit; };

   template<IsTemperatureUnit Unit>
   class Temperature : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Temperature() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      Temperature(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      template<IsTemperatureUnit T>
      Temperature(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<IsTemperatureUnit T>
      double GetTemperature() const
      {
         return this->template GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   template<IsTemperatureUnit T>
   Temperature(T) -> Temperature<T>;

   template<IsTemperatureUnit TemperatureUnit>
   Temperature(BaseDimension<std::tuple<TemperatureUnit>, std::tuple<>>) -> Temperature<TemperatureUnit>;

   template<> struct Conversion<Celsius, Fahrenheit>
   { 
      static constexpr PrecisionType slope = (9.0 / 5.0);
      static constexpr PrecisionType offset = 32;
   };

   template<> struct Conversion<Fahrenheit, Celsius>
   {
      static constexpr PrecisionType slope = (5.0 / 9.0);
      static constexpr PrecisionType offset = (-160.0 / 9.0);
   };

   template<> struct Conversion<Celsius, Kelvin>
   {
      static constexpr PrecisionType slope = 1.0;
      static constexpr PrecisionType offset = 273.15;
   };

   template<> struct Conversion<Kelvin, Celsius>
   {
      static constexpr PrecisionType slope = 1.0;
      static constexpr PrecisionType offset = -273.15;
   };

   template<typename T>
   struct is_temperature : std::is_convertible<T, Temperature<Celsius>> {};

   template<typename T>
   constexpr bool is_temperature_v = is_temperature<T>::value;

   template<typename T>
   concept temperature_type = is_temperature_v<T>;

}

#endif //STATIC_DIMENSION_TEMPERATURE_H
