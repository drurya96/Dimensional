#ifndef STATIC_DIMENSION_CONDUCTANCE_H
#define STATIC_DIMENSION_CONDUCTANCE_H

#include "Dimension_Impl/DerivedDimensions/ConductanceDimension_Impl.h"

namespace Dimension
{

   struct Siemens
   {
      using units = std::tuple<
         UnitExponent<Seconds, 1, 1>
         UnitExponent<Coulombs, 2, 1>
         UnitExponent<KiloGrams, -1, 1>
         UnitExponent<Meters, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_CONDUCTANCE_H