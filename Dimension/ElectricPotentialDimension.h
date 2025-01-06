#ifndef STATIC_DIMENSION_ElectricPotential_H
#define STATIC_DIMENSION_ElectricPotential_H

#include "Dimension_Impl/DerivedDimensions/ElectricPotentialDimension_Impl.h" 

namespace Dimension
{

   struct Volts
   {
      using NumTuple = std::tuple<KiloGrams, Meters, Meters>;
      using DenTuple = std::tuple<Seconds, Seconds, Coulombs>;
   };

}

#endif //STATIC_DIMENSION_ElectricPotential_H
