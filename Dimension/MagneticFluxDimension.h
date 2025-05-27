#ifndef STATIC_DIMENSION_MAGNETICFLUX_H
#define STATIC_DIMENSION_MAGNETICFLUX_H

#include "Dimension_Impl/DerivedDimensions/MagneticFluxDimension_Impl.h"

namespace Dimension
{

   struct Webers
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>,
         UnitExponent<Meters, 2, 1>,
         UnitExponent<Seconds, -1, 1>,
         UnitExponent<Coulombs, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_MAGNETICFLUX_H