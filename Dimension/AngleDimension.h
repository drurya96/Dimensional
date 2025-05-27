#ifndef STATIC_DIMENSION_ANGLE_H
#define STATIC_DIMENSION_ANGLE_H

#include "Dimension_Impl/FundamentalDimensions/AngleDimension_Impl.h"

namespace Dimension
{
   struct Radians : public AngleUnit<Radians, "Radians", "rad"> {};
   struct Degrees : public AngleUnit<Degrees, "Degrees", "deg"> {};

   template<> struct Conversion<Radians, Degrees> { static constexpr PrecisionType slope = 180 / std::numbers::pi; };

}

#include "Dimension_Impl/FundamentalDimensions/AngleExtras.h"

#endif //STATIC_DIMENSION_ANGLE_H
