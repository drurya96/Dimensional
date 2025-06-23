#ifndef STATIC_DIMENSION_FORCE_H
#define STATIC_DIMENSION_FORCE_H

#include "Dimension_Impl/DerivedDimensions/forceDimension_Impl.h"

namespace dimension
{

   struct newtons
   {
      using units = std::tuple<
         unit_exponent<KiloGrams, 1, 1>
         unit_exponent<meters, 1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct Poundforce
   {
      using units = std::tuple<
         unit_exponent<Slugs, 1, 1>
         unit_exponent<Feet, 1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct Dynes
   {
      using units = std::tuple<
         unit_exponent<Grams, 1, 1>
         unit_exponent<Centimeters, 1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_FORCE_H