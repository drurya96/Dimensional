#ifndef STATIC_DIMENSION_FORCE_H
#define STATIC_DIMENSION_FORCE_H

#include "Dimension_Impl/DerivedDimensions/force_dimension_Impl.h"

namespace dimension
{

   struct newtons
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct pound_force
   {
      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_FORCE_H