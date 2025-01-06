#ifndef STATIC_DIMENSION_Power_H
#define STATIC_DIMENSION_Power_H

#include "Dimension_Impl/DerivedDimensions/PowerDimension_Impl.h" 

namespace Dimension
{

   struct Watts
   {
      using NumTuple = std::tuple<KiloGrams, Meters, Meters>;
      using DenTuple = std::tuple<Seconds, Seconds, Seconds>;
   };

}

#endif //STATIC_DIMENSION_Power_H
