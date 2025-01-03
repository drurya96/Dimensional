#ifndef STATIC_DIMENSION_TEMPERATURE_H
#define STATIC_DIMENSION_TEMPERATURE_H

#include "Dimension_Impl/FundamentalDimensions/TemperatureDimension_Impl.h"

namespace Dimension
{

   struct Celsius : public TemperatureUnit<Celsius, "Celsius", "C"> {};
   struct Fahrenheit : public TemperatureUnit<Fahrenheit, "Fahrenheit", "F"> {};
   struct Kelvin : public TemperatureUnit<Kelvin, "Kelvin", "K"> {};
   struct Rankine : public TemperatureUnit<Rankine, "Rankine", "R"> {};

   template<> struct is_absolute<Celsius> : std::integral_constant<bool, false> {};
   template<> struct is_absolute<Fahrenheit> : std::integral_constant<bool, false> {};


   template<> struct Conversion<Celsius, Fahrenheit>
   { 
      static constexpr PrecisionType slope = 1.8;
      static constexpr PrecisionType offset = 32;
   };

   template<> struct Conversion<Celsius, Kelvin>
   {
      static constexpr PrecisionType slope = 1.0;
      static constexpr PrecisionType offset = 273.15;
   };

   template<> struct Conversion<Celsius, Rankine>
   { 
      static constexpr PrecisionType slope = 1.8;
      static constexpr PrecisionType offset = 491.67;
   };

   template<> struct Conversion<Kelvin, Rankine> { static constexpr PrecisionType slope = 1.8; };

   template<> struct Conversion<Kelvin, Fahrenheit>
   { 
      static constexpr PrecisionType slope = 1.8;
      static constexpr PrecisionType offset = -459.67;
   };

}

#endif //STATIC_DIMENSION_TEMPERATURE_H
