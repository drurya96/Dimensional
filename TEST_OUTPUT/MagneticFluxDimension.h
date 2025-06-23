#ifndef STATIC_DIMENSION_MAGNETICFLUX_H
#define STATIC_DIMENSION_MAGNETICFLUX_H

#include "Dimension_Impl/DerivedDimensions/MagneticFluxDimension_Impl.h"

namespace dimension
{

   struct Webers
   {
      using units = std::tuple<
         unit_exponent<KiloGrams, 1, 1>
         unit_exponent<meters, 2, 1>
         unit_exponent<seconds, -1, 1>
         unit_exponent<Coulombs, -1, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_MAGNETICFLUX_H