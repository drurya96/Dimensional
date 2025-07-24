#ifndef STATIC_DIMENSION_RESISTANCE_H
#define STATIC_DIMENSION_RESISTANCE_H

#include "Dimension_Impl/DerivedDimensions/resistance_dimension_Impl.h"

namespace dimension
{

   struct ohms
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -1, 1>,
         unit_exponent<coulombs, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_RESISTANCE_H