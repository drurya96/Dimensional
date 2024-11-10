#ifndef STATIC_DIMENSION_TIME_H
#define STATIC_DIMENSION_TIME_H

#include "Dimension_Impl/FundamentalDimensions/TimeDimension_Impl.h"

namespace Dimension
{

   struct Seconds : public TimeUnit<Seconds, "Seconds", "s"> {};
   struct Minutes : public TimeUnit<Minutes, "Minutes", "min"> {};
   struct Hours : public TimeUnit<Hours, "Hours", "h"> {};


   template<> struct Conversion<Seconds, Minutes> { static constexpr PrecisionType slope = (1.0 / 60.0); };
   template<> struct Conversion<Seconds, Hours> { static constexpr PrecisionType slope = (1.0 / 3600.0); };

   ALL_SI_PREFIXES(Seconds, "s", TimeUnit);

}

#endif //STATIC_DIMENSION_TIME_H
