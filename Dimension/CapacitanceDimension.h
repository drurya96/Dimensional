#ifndef STATIC_DIMENSION_Capacitance_H
#define STATIC_DIMENSION_Capacitance_H

#include "Dimension_Impl/DerivedDimensions/CapacitanceDimension_Impl.h" 

namespace Dimension
{

   struct Farads
   {
      using NumTuple = std::tuple<Coulombs, Coulombs,Seconds, Seconds>;
      using DenTuple = std::tuple<KiloGrams, Meters, Meters>;
   };

}

#endif //STATIC_DIMENSION_Capacitance_H
