#ifndef STATIC_DIMENSION_CURRENT_H
#define STATIC_DIMENSION_CURRENT_H

#include "Dimension_Impl/DerivedDimensions/current_dimension_Impl.h"

namespace dimension
{

   struct amperes
   {
      using units = std::tuple<
         unit_exponent<coulombs, 1, 1>,
         unit_exponent<seconds, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_CURRENT_H