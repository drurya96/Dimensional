#ifndef STATIC_DIMENSION_MAGNETICFIELD_H
#define STATIC_DIMENSION_MAGNETICFIELD_H

#include "Dimension_Impl/DerivedDimensions/MagneticFieldDimension_Impl.h"

namespace Dimension
{

   struct Teslas
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>
         UnitExponent<Seconds, -1, 1>
         UnitExponent<Coulombs, -1, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_MAGNETICFIELD_H