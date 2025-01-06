#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "Dimension_Impl/DerivedDimensions/SpeedDimension_Impl.h"

namespace Dimension
{

   struct Knots
   {
      using NumTuple = std::tuple<NauticalMiles>;
      using DenTuple = std::tuple<Seconds>;
   };

}

#endif //STATIC_DIMENSION_SPEED_H
