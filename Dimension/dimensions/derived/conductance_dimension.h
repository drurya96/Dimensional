#ifndef STATIC_DIMENSION_CONDUCTANCE_H
#define STATIC_DIMENSION_CONDUCTANCE_H

#include "Dimension_Impl/DerivedDimensions/conductance_dimension_Impl.h"

namespace dimension
{

   struct siemens
   {
      using units = std::tuple<
         unit_exponent<seconds, 1, 1>,
         unit_exponent<coulombs, 2, 1>,
         unit_exponent<kilo_grams, -1, 1>,
         unit_exponent<meters, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_CONDUCTANCE_H