#ifndef STATIC_DIMENSION_MagneticFlux_H
#define STATIC_DIMENSION_MagneticFlux_H

#include "Dimension_Impl/DerivedDimensions/MagneticFluxDimension_Impl.h" 

namespace Dimension
{

   struct Webers
   {
      using NumTuple = std::tuple<KiloGrams, Meters, Meters>;
      using DenTuple = std::tuple<Seconds, Coulombs>;
   };

}

#endif //STATIC_DIMENSION_MagneticFlux_H
