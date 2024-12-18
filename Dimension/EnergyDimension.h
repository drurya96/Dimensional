#ifndef STATIC_DIMENSION_ENERGY_H
#define STATIC_DIMENSION_ENERGY_H

#include "Dimension_Impl/DerivedDimensions/EnergyDimension_Impl.h" 

namespace Dimension
{

   struct Joules
   {
      using NumTuple = std::tuple<KiloGrams, Meters, Meters>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

   struct Ergs
   {
      using NumTuple = std::tuple<Grams, CentiMeters, CentiMeters>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

   struct FootPound
   {
      using NumTuple = std::tuple<Slugs, Feet, Feet>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

   struct Calories
   {
      using NumTuple = std::tuple<Grams, CentiMeters, CentiMeters>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

   struct BTUs
   {
      using NumTuple = std::tuple<Slugs, Feet, Feet>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

}

#endif //STATIC_DIMENSION_ENERGY_H
