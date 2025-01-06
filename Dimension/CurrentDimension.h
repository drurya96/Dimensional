#ifndef STATIC_DIMENSION_Current_H
#define STATIC_DIMENSION_Current_H

#include "Dimension_Impl/DerivedDimensions/CurrentDimension_Impl.h" 

namespace Dimension
{

   struct Amperes
   {
      using NumTuple = std::tuple<Coulombs>;
      using DenTuple = std::tuple<Seconds>;
   };

}

#endif //STATIC_DIMENSION_Current_H
