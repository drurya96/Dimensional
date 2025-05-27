#ifndef STATIC_DIMENSION_POWER_H
#define STATIC_DIMENSION_POWER_H

#include "Dimension_Impl/DerivedDimensions/PowerDimension_Impl.h"

namespace Dimension
{

   struct Watts
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>
         UnitExponent<Meters, 2, 1>
         UnitExponent<Seconds, -3, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_POWER_H