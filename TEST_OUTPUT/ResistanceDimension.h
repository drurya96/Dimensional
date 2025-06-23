#ifndef STATIC_DIMENSION_RESISTANCE_H
#define STATIC_DIMENSION_RESISTANCE_H

#include "Dimension_Impl/DerivedDimensions/ResistanceDimension_Impl.h"

namespace dimension
{

   struct Ohms
   {
      using units = std::tuple<
         unit_exponent<KiloGrams, 1, 1>
         unit_exponent<meters, 2, 1>
         unit_exponent<seconds, -1, 1>
         unit_exponent<Coulombs, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_RESISTANCE_H