#ifndef STATIC_DIMENSION_INDUCTANCE_H
#define STATIC_DIMENSION_INDUCTANCE_H

#include "Dimension_Impl/DerivedDimensions/InductanceDimension_Impl.h"

namespace dimension
{

   struct Henrys
   {
      using units = std::tuple<
         unit_exponent<KiloGrams, 1, 1>
         unit_exponent<meters, 2, 1>
         unit_exponent<Coulombs, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_INDUCTANCE_H