#ifndef STATIC_DIMENSION_VOLUME_H
#define STATIC_DIMENSION_VOLUME_H

#include "Dimension_Impl/DerivedDimensions/VolumeDimension_Impl.h"

namespace Dimension
{

   struct Liter
   {
      using NumTuple = std::tuple<DeciMeters, DeciMeters, DeciMeters>;
      using DenTuple = std::tuple<>;
   };

   struct MilliLiter
   {
      using NumTuple = std::tuple<CentiMeters, CentiMeters, CentiMeters>;
      using DenTuple = std::tuple<>;
   };

   struct GallonLength : public LengthUnit<GallonLength> { public: using LengthUnit::LengthUnit; };

   template<> struct Conversion<Meters, GallonLength> { static constexpr PrecisionType slope = (1.0 / 0.15562); };
   template<> struct Conversion<GallonLength, Meters> { static constexpr PrecisionType slope = 0.15562; };

   struct Gallon
   {
      using NumTuple = std::tuple<GallonLength, GallonLength, GallonLength>;
      using DenTuple = std::tuple<>;
   };

}

#endif //STATIC_DIMENSION_VOLUME_H
