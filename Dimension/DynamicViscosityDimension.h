#ifndef STATIC_DIMENSION_DynamicViscosity_H
#define STATIC_DIMENSION_DynamicViscosity_H

#include "Dimension_Impl/DerivedDimensions/DynamicViscosityDimension_Impl.h" 

namespace Dimension
{

   struct Poises
   {
      using NumTuple = std::tuple<Grams>;
      using DenTuple = std::tuple<Seconds, CentiMeters>;
   };

}

#endif //STATIC_DIMENSION_DynamicViscosity_H
