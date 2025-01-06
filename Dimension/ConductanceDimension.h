#ifndef STATIC_DIMENSION_Conductance_H
#define STATIC_DIMENSION_Conductance_H

#include "Dimension_Impl/DerivedDimensions/ConductanceDimension_Impl.h" 

namespace Dimension
{

   struct Siemens
   {
      using NumTuple = std::tuple<Seconds, Coulombs, Coulombs>;
      using DenTuple = std::tuple<KiloGrams, Meters, Meters>;
   };

}

#endif //STATIC_DIMENSION_Conductance_H
