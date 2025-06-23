#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "Dimension_Impl/DerivedDimensions/speedDimension_Impl.h"

namespace dimension
{

   struct Knots
   {
      using units = std::tuple<
         unit_exponent<NauticalMiles, 1, 1>
         unit_exponent<Hours, -1, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_SPEED_H