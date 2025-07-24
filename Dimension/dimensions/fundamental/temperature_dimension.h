#ifndef STATIC_DIMENSION_TEMPERATURE_H
#define STATIC_DIMENSION_TEMPERATURE_H

#include "Dimension_Impl/FundamentalDimensions/temperature_dimension_Impl.h"

namespace dimension
{
   struct kelvin : public temperatureUnit<kelvin, "Kelvin", "K"> {};
   struct rankine : public temperatureUnit<rankine, "Rankine", "R"> {};

   template<> struct Conversion<kelvin, rankine> { static constexpr PrecisionType slope = (9.0 / 5.0); };


}



#include "extensions/temperatureExtras.h"

#endif //STATIC_DIMENSION_TEMPERATURE_H