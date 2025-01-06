#ifndef STATIC_DIMENSION_MagneticField_H
#define STATIC_DIMENSION_MagneticField_H

#include "Dimension_Impl/DerivedDimensions/MagneticFieldDimension_Impl.h" 

namespace Dimension
{

   struct Teslas
   {
      using NumTuple = std::tuple<KiloGrams>;
      using DenTuple = std::tuple<Seconds, Coulombs>;
   };

}

#endif //STATIC_DIMENSION_MagneticField_H
