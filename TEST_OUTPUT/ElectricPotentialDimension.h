#ifndef STATIC_DIMENSION_ELECTRICPOTENTIAL_H
#define STATIC_DIMENSION_ELECTRICPOTENTIAL_H

#include "Dimension_Impl/DerivedDimensions/ElectricPotentialDimension_Impl.h"

namespace dimension
{

   struct Volts
   {
      using units = std::tuple<
         unit_exponent<KiloGrams, 1, 1>
         unit_exponent<meters, 2, 1>
         unit_exponent<seconds, -2, 1>
         unit_exponent<Coulombs, -1, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_ELECTRICPOTENTIAL_H