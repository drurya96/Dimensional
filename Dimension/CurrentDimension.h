#ifndef STATIC_DIMENSION_CURRENT_H
#define STATIC_DIMENSION_CURRENT_H

#include "Dimension_Impl/DerivedDimensions/CurrentDimension_Impl.h"

namespace Dimension
{

   struct Amperes
   {
      using units = std::tuple<
         UnitExponent<Coulombs, 1, 1>,
         UnitExponent<Seconds, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_CURRENT_H