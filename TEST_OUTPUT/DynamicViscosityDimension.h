#ifndef STATIC_DIMENSION_DYNAMICVISCOSITY_H
#define STATIC_DIMENSION_DYNAMICVISCOSITY_H

#include "Dimension_Impl/DerivedDimensions/DynamicViscosityDimension_Impl.h"

namespace Dimension
{

   struct Poises
   {
      using units = std::tuple<
         UnitExponent<Grams, 1, 1>
         UnitExponent<Seconds, -1, 1>
         UnitExponent<CentiMeters, -1, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_DYNAMICVISCOSITY_H