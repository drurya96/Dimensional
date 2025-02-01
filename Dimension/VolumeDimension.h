#ifndef STATIC_DIMENSION_VOLUME_H
#define STATIC_DIMENSION_VOLUME_H

#include "Dimension_Impl/DerivedDimensions/VolumeDimension_Impl.h"

namespace Dimension
{

   struct Liters
   {
      using NumTuple = std::tuple<DeciMeters, DeciMeters, DeciMeters>;
      using DenTuple = std::tuple<>;
   };

   struct MilliLiters
   {
      using NumTuple = std::tuple<CentiMeters, CentiMeters, CentiMeters>;
      using DenTuple = std::tuple<>;
   };



   struct FluidOunceLength : public LengthUnit<FluidOunceLength, "FluidOunceLength", "FluidOunceLength"> {};
   template<> struct Conversion<FluidOunceLength, Meters> { static constexpr PrecisionType slope = 0.03092440948; };

   struct CupLength : public LengthUnit<CupLength, "CupLength", "CupLength"> {};
   template<> struct Conversion<CupLength, Meters> { static constexpr PrecisionType slope = 0.061848767433875; };

   struct PintLength : public LengthUnit<PintLength, "PintLength", "PintLength"> {};
   template<> struct Conversion<PintLength, Meters> { static constexpr PrecisionType slope = 0.077924564; };

   struct QuartLength : public LengthUnit<QuartLength, "QuartLength", "QuartLength"> {};
   template<> struct Conversion<QuartLength, Meters> { static constexpr PrecisionType slope = 0.09817886355045987; };

   struct GallonLength : public LengthUnit<GallonLength, "GallonLength", "GallonLength"> {};
   template<> struct Conversion<GallonLength, Meters> { static constexpr PrecisionType slope = 0.155849128; };

   struct TeaspoonLength : public LengthUnit<TeaspoonLength, "TeaspoonLength", "TeaspoonLength"> {};
   template<> struct Conversion<TeaspoonLength, Meters> { static constexpr PrecisionType slope = 0.01701834420916624526055025475059; };

   struct TablespoonLength : public LengthUnit<TablespoonLength, "TablespoonLength", "TablespoonLength"> {};
   template<> struct Conversion<TablespoonLength, Meters> { static constexpr PrecisionType slope = 0.02454469962137835; };

   struct FluidOunces
   {
      using NumTuple = std::tuple<FluidOunceLength, FluidOunceLength, FluidOunceLength>;
      using DenTuple = std::tuple<>;
   };

   struct Cups
   {
      using NumTuple = std::tuple<CupLength, CupLength, CupLength>;
      using DenTuple = std::tuple<>;
   };

   struct Pints
   {
      using NumTuple = std::tuple<PintLength, PintLength, PintLength>;
      using DenTuple = std::tuple<>;
   };

   struct Quarts
   {
      using NumTuple = std::tuple<QuartLength, QuartLength, QuartLength>;
      using DenTuple = std::tuple<>;
   };

   struct Gallons
   {
      using NumTuple = std::tuple<GallonLength, GallonLength, GallonLength>;
      using DenTuple = std::tuple<>;
   };

   struct Teaspoons
   {
      using NumTuple = std::tuple<TeaspoonLength, TeaspoonLength, TeaspoonLength>;
      using DenTuple = std::tuple<>;
   };

   struct Tablespoons
   {
      using NumTuple = std::tuple<TablespoonLength, TablespoonLength, TablespoonLength>;
      using DenTuple = std::tuple<>;
   };

}

#endif //STATIC_DIMENSION_VOLUME_H
