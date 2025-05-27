#ifndef STATIC_DIMENSION_ELECTRICPOTENTIAL_H
#define STATIC_DIMENSION_ELECTRICPOTENTIAL_H

#include "Dimension_Impl/DerivedDimensions/ElectricPotentialDimension_Impl.h"

namespace Dimension
{

   struct Volts
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>,
         UnitExponent<Meters, 2, 1>,
         UnitExponent<Seconds, -2, 1>,
         UnitExponent<Coulombs, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_ELECTRICPOTENTIAL_H