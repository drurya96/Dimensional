#ifndef STATIC_DIMENSION_FREQUENCY_H
#define STATIC_DIMENSION_FREQUENCY_H

#include "Dimension_Impl/DerivedDimensions/frequency_dimension_Impl.h"

namespace dimension
{

   struct hertz
   {
      using units = std::tuple<
         unit_exponent<seconds, -1, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_FREQUENCY_H