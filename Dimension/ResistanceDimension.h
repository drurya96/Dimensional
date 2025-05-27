#ifndef STATIC_DIMENSION_RESISTANCE_H
#define STATIC_DIMENSION_RESISTANCE_H

#include "Dimension_Impl/DerivedDimensions/ResistanceDimension_Impl.h"

namespace Dimension
{

   struct Ohms
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>,
         UnitExponent<Meters, 2, 1>,
         UnitExponent<Seconds, -1, 1>,
         UnitExponent<Coulombs, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_RESISTANCE_H