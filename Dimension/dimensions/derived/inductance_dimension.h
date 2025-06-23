#ifndef STATIC_DIMENSION_INDUCTANCE_H
#define STATIC_DIMENSION_INDUCTANCE_H

#include "Dimension_Impl/DerivedDimensions/inductance_dimension_Impl.h"

namespace dimension
{

   struct henrys
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<coulombs, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_INDUCTANCE_H