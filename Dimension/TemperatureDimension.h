#ifndef STATIC_DIMENSION_TEMPERATURE_H
#define STATIC_DIMENSION_TEMPERATURE_H

#include "Dimension_Impl/FundamentalDimensions/TemperatureDimension_Impl.h"

namespace Dimension
{

   struct Celsius : public TemperatureUnit<Celsius, "Celsius", "C"> { public: using TemperatureUnit::TemperatureUnit; };
   struct Fahrenheit : public TemperatureUnit<Fahrenheit, "Fahrenheit", "F"> { public: using TemperatureUnit::TemperatureUnit; };
   struct Kelvin : public TemperatureUnit<Kelvin, "Kelvin", "K"> { public: using TemperatureUnit::TemperatureUnit; };
   struct Rankine : public TemperatureUnit<Rankine, "Rankine", "R"> { public: using TemperatureUnit::TemperatureUnit; };


   template<> struct Conversion<Celsius, Fahrenheit>
   { 
      static constexpr PrecisionType slope = (9.0 / 5.0);
      static constexpr PrecisionType offset = 32;
   };

   template<> struct Conversion<Celsius, Kelvin>
   {
      static constexpr PrecisionType slope = 1.0;
      static constexpr PrecisionType offset = 273.15;
   };

   template<> struct Conversion<Celsius, Rankine>
   { 
      static constexpr PrecisionType slope = (9.0 / 5.0);
      static constexpr PrecisionType offset = 491.67;
   };

}

#endif //STATIC_DIMENSION_TEMPERATURE_H
