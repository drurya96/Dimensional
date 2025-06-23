#ifndef STATIC_DIMENSION_TIMESPAN_H
#define STATIC_DIMENSION_TIMESPAN_H

#include "../Dimension_Impl/FundamentalDimensions/timespan_dimension_Impl.h"

namespace dimension
{
   struct seconds : public timespanUnit<seconds, "seconds", "s"> {};
   struct minutes : public timespanUnit<minutes, "minutes", "min"> {};
   struct hours : public timespanUnit<hours, "Hours", "h"> {};

   template<> struct Conversion<seconds, minutes> { static constexpr PrecisionType slope = (1.0 / 60.0); };
   template<> struct Conversion<seconds, hours> { static constexpr PrecisionType slope = (1.0 / 3600.0); };

   ALL_SI_PREFIXES(seconds, "s", timespanUnit);

}




#endif //STATIC_DIMENSION_TIMESPAN_H