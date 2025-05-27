#ifndef STATIC_DIMENSION_FORCE_H
#define STATIC_DIMENSION_FORCE_H

#include "Dimension_Impl/DerivedDimensions/ForceDimension_Impl.h"

namespace Dimension
{

   struct Newtons
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>,
         UnitExponent<Meters, 1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

   struct PoundForce
   {
      using units = std::tuple<
         UnitExponent<Slugs, 1, 1>,
         UnitExponent<Feet, 1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

   struct Dynes
   {
      using units = std::tuple<
         UnitExponent<Grams, 1, 1>,
         UnitExponent<CentiMeters, 1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_FORCE_H