#ifndef STATIC_DIMENSION_PRESSURE_H
#define STATIC_DIMENSION_PRESSURE_H

#include "Dimension_Impl/DerivedDimensions/PressureDimension_Impl.h" 

namespace Dimension
{

   struct Pascals
   {
      using NumTuple = std::tuple<KiloGrams>;
      using DenTuple = std::tuple<Meters, Seconds, Seconds>;
   };
   
}

#endif //STATIC_DIMENSION_PRESSURE_H
