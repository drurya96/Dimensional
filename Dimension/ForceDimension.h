#ifndef STATIC_DIMENSION_FORCE_H
#define STATIC_DIMENSION_FORCE_H

#include "Dimension_Impl/DerivedDimensions/ForceDimension_Impl.h" 

namespace Dimension
{

   struct Newtons
   {
      using NumTuple = std::tuple<KiloGrams, Meters>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

   struct PoundForce
   {
      using NumTuple = std::tuple<Slugs, Feet>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

   struct Dynes
   {
      using NumTuple = std::tuple<Grams, CentiMeters>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

}

#endif //STATIC_DIMENSION_FORCE_H
