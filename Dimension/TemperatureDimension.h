#ifndef STATIC_DIMENSION_TEMPERATURE_H
#define STATIC_DIMENSION_TEMPERATURE_H

#include "Dimension_Impl/FundamentalDimensions/TemperatureDimension_Impl.h"

namespace Dimension
{

   struct Kelvin : public TemperatureUnit<Kelvin, "Kelvin", "K"> {};
   struct Rankine : public TemperatureUnit<Rankine, "Rankine", "R"> {};

   template<> struct Conversion<Kelvin, Rankine> { static constexpr PrecisionType slope = 1.8; };

   struct Celsius : public reference_frame<Kelvin>{ static constexpr double offset = 273.15; };
   struct Fahrenheit : public reference_frame<Rankine>{ static constexpr double offset = 459.67; };

}

#endif //STATIC_DIMENSION_TEMPERATURE_H
