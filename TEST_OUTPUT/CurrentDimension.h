#ifndef STATIC_DIMENSION_CURRENT_H
#define STATIC_DIMENSION_CURRENT_H

#include "Dimension_Impl/DerivedDimensions/CurrentDimension_Impl.h"

namespace dimension
{

   struct Amperes
   {
      using units = std::tuple<
         unit_exponent<Coulombs, 1, 1>
         unit_exponent<seconds, -1, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_CURRENT_H