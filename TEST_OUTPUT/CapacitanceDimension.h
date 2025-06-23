#ifndef STATIC_DIMENSION_CAPACITANCE_H
#define STATIC_DIMENSION_CAPACITANCE_H

#include "Dimension_Impl/DerivedDimensions/CapacitanceDimension_Impl.h"

namespace dimension
{

   struct Farads
   {
      using units = std::tuple<
         unit_exponent<Coulombs, 2, 1>
         unit_exponent<seconds, 2, 1>
         unit_exponent<KiloGrams, -1, 1>
         unit_exponent<meters, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_CAPACITANCE_H