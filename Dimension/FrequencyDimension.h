#ifndef STATIC_DIMENSION_FREQUENCY_H
#define STATIC_DIMENSION_FREQUENCY_H

#include "Dimension_Impl/DerivedDimensions/FrequencyDimension_Impl.h"

namespace Dimension
{

   struct Hertz
   {
      using units = std::tuple<
         UnitExponent<Seconds, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_FREQUENCY_H