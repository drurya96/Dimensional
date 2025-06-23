#ifndef STATIC_DIMENSION_ANGLE_H
#define STATIC_DIMENSION_ANGLE_H

#include "../Dimension_Impl/FundamentalDimensions/angle_dimension_Impl.h"

namespace dimension
{
   struct radians : public angleUnit<radians, "radians", "rad"> {};
   struct degrees : public angleUnit<degrees, "Degrees", "deg"> {};

   template<> struct Conversion<radians, degrees> { static constexpr PrecisionType slope = 57.2958; };


}



#include "extensions/angleExtras.h"

#endif //STATIC_DIMENSION_ANGLE_H