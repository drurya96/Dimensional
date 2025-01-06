#ifndef STATIC_DIMENSION_Resistance_H
#define STATIC_DIMENSION_Resistance_H

#include "Dimension_Impl/DerivedDimensions/ResistanceDimension_Impl.h" 

namespace Dimension
{

   struct Ohms
   {
      using NumTuple = std::tuple<KiloGrams, Meters, Meters>;
      using DenTuple = std::tuple<Seconds, Coulombs, Coulombs>;
   };

}

#endif //STATIC_DIMENSION_Resistance_H
