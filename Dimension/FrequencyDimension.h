#ifndef STATIC_DIMENSION_Frequency_H
#define STATIC_DIMENSION_Frequency_H

#include "Dimension_Impl/DerivedDimensions/FrequencyDimension_Impl.h" 

namespace Dimension
{

   struct Hertz
   {
      using NumTuple = std::tuple<>;
      using DenTuple = std::tuple<Seconds>;
   };

}

#endif //STATIC_DIMENSION_Frequency_H
