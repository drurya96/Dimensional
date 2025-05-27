#ifndef STATIC_DIMENSION_INDUCTANCE_H
#define STATIC_DIMENSION_INDUCTANCE_H

#include "Dimension_Impl/DerivedDimensions/InductanceDimension_Impl.h"

namespace Dimension
{

   struct Henrys
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>,
         UnitExponent<Meters, 2, 1>,
         UnitExponent<Coulombs, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_INDUCTANCE_H