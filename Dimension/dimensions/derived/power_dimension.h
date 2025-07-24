#ifndef STATIC_DIMENSION_POWER_H
#define STATIC_DIMENSION_POWER_H

#include "Dimension_Impl/DerivedDimensions/power_dimension_Impl.h"

namespace dimension
{

   struct Watts
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -3, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_POWER_H