#ifndef STATIC_DIMENSION_Inductance_H
#define STATIC_DIMENSION_Inductance_H

#include "Dimension_Impl/DerivedDimensions/InductanceDimension_Impl.h" 

namespace Dimension
{

   struct Henrys
   {
      using NumTuple = std::tuple<KiloGrams, Meters, Meters>;
      using DenTuple = std::tuple<Coulombs, Coulombs>;
   };

}

#endif //STATIC_DIMENSION_Inductance_H
