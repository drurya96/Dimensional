#ifndef STATIC_DIMENSION_TEMPERATURE_H
#define STATIC_DIMENSION_TEMPERATURE_H

#include "StaticBaseDimension.h"

namespace Dimension
{
   struct TemperatureType {};
   struct Celsius;

   template<typename Unit>
   struct TemperatureUnit : public BaseUnit
   {
   public:
      using BaseUnit::BaseUnit;

      using Dim = TemperatureType;
      using Primary = Celsius;
   };

   struct Celsius : public TemperatureUnit<Celsius> { public: using TemperatureUnit::TemperatureUnit; };
   struct Fahrenheit : public TemperatureUnit<Fahrenheit> { public: using TemperatureUnit::TemperatureUnit; };
   struct Kelvin : public TemperatureUnit<Kelvin> { public: using TemperatureUnit::TemperatureUnit; };

   template<typename Unit>
   class Temperature : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<typename Unit::Dim, typename Celsius::Dim>, "Unit provided does not derive from TemperatureUnit");
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Temperature() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      Temperature(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      template<typename T>
      Temperature(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<typename T>
      double GetTemperature() const
      {
         return this->template GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   template<typename T>
   Temperature(T) -> Temperature<T>;

   template<typename TemperatureUnit>
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

}

#endif //STATIC_DIMENSION_TEMPERATURE_H
