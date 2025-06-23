#ifndef STATIC_DIMENSION_DYNAMICVISCOSITY_H
#define STATIC_DIMENSION_DYNAMICVISCOSITY_H

#include "Dimension_Impl/DerivedDimensions/DynamicViscosityDimension_Impl.h"

namespace dimension
{

   struct Poises
   {
      using units = std::tuple<
         unit_exponent<Grams, 1, 1>
         unit_exponent<seconds, -1, 1>
         unit_exponent<Centimeters, -1, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_DYNAMICVISCOSITY_H