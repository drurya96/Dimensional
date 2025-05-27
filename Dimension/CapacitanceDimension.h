#ifndef STATIC_DIMENSION_CAPACITANCE_H
#define STATIC_DIMENSION_CAPACITANCE_H

#include "Dimension_Impl/DerivedDimensions/CapacitanceDimension_Impl.h"

namespace Dimension
{

   struct Farads
   {
      using units = std::tuple<
         UnitExponent<Coulombs, 2, 1>,
         UnitExponent<Seconds, 2, 1>,
         UnitExponent<KiloGrams, -1, 1>,
         UnitExponent<Meters, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_CAPACITANCE_H