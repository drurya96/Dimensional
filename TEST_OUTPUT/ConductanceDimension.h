#ifndef STATIC_DIMENSION_CONDUCTANCE_H
#define STATIC_DIMENSION_CONDUCTANCE_H

#include "Dimension_Impl/DerivedDimensions/ConductanceDimension_Impl.h"

namespace dimension
{

   struct Siemens
   {
      using units = std::tuple<
         unit_exponent<seconds, 1, 1>
         unit_exponent<Coulombs, 2, 1>
         unit_exponent<KiloGrams, -1, 1>
         unit_exponent<meters, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_CONDUCTANCE_H