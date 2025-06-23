#ifndef STATIC_DIMENSION_MAGNETICFIELD_H
#define STATIC_DIMENSION_MAGNETICFIELD_H

#include "Dimension_Impl/DerivedDimensions/MagneticFieldDimension_Impl.h"

namespace dimension
{

   struct Teslas
   {
      using units = std::tuple<
         unit_exponent<KiloGrams, 1, 1>
         unit_exponent<seconds, -1, 1>
         unit_exponent<Coulombs, -1, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_MAGNETICFIELD_H