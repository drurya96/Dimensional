#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "Dimension_Impl/DerivedDimensions/speed_dimension_Impl.h"

namespace dimension
{

   struct knots
   {
      using units = std::tuple<
         unit_exponent<nautical_miles, 1, 1>,
         unit_exponent<hours, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_SPEED_H