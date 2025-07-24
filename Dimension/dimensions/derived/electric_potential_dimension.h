#ifndef STATIC_DIMENSION_ELECTRIC_POTENTIAL_H
#define STATIC_DIMENSION_ELECTRIC_POTENTIAL_H

#include "Dimension_Impl/DerivedDimensions/electric_potential_dimension_Impl.h"

namespace dimension
{

   struct volts
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -2, 1>,
         unit_exponent<coulombs, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_ELECTRIC_POTENTIAL_H