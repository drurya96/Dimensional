#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "Dimension_Impl/DerivedDimensions/SpeedDimension_Impl.h"

namespace Dimension
{

   struct Knots
   {
      using units = std::tuple<
         UnitExponent<NauticalMiles, 1, 1>,
         UnitExponent<Hours, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_SPEED_H