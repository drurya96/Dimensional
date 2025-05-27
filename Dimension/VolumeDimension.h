#ifndef STATIC_DIMENSION_VOLUME_H
#define STATIC_DIMENSION_VOLUME_H

#include "Dimension_Impl/DerivedDimensions/VolumeDimension_Impl.h"

namespace Dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct FluidOunceLength : public LengthUnit<FluidOunceLength, "FluidOunceLength", "FluidOunceLength"> {};
   template<> struct Conversion<FluidOunceLength, Meters> { static constexpr PrecisionType slope = 0.03092440948; };

   struct CupLength : public LengthUnit<CupLength, "CupLength", "CupLength"> {};
   template<> struct Conversion<CupLength, Meters> { static constexpr PrecisionType slope = 0.061848767433875; };

   struct PintLength : public LengthUnit<PintLength, "PintLength", "PintLength"> {};
   template<> struct Conversion<PintLength, Meters> { static constexpr PrecisionType slope = 0.077924564; };

   struct QuartLength : public LengthUnit<QuartLength, "QuartLength", "QuartLength"> {};
   template<> struct Conversion<QuartLength, Meters> { static constexpr PrecisionType slope = 0.09817886355045986; };

   struct GallonLength : public LengthUnit<GallonLength, "GallonLength", "GallonLength"> {};
   template<> struct Conversion<GallonLength, Meters> { static constexpr PrecisionType slope = 0.155849128; };

   struct TeaspoonLength : public LengthUnit<TeaspoonLength, "TeaspoonLength", "TeaspoonLength"> {};
   template<> struct Conversion<TeaspoonLength, Meters> { static constexpr PrecisionType slope = 0.017018344209166245; };

   struct TablespoonLength : public LengthUnit<TablespoonLength, "TablespoonLength", "TablespoonLength"> {};
   template<> struct Conversion<TablespoonLength, Meters> { static constexpr PrecisionType slope = 0.02454469962137835; };


   struct Liters
   {
      using units = std::tuple<
         UnitExponent<DeciMeters, 3, 1>
      >;
   };

   struct MilliLiters
   {
      using units = std::tuple<
         UnitExponent<CentiMeters, 3, 1>
      >;
   };

   struct FluidOunces
   {
      using units = std::tuple<
         UnitExponent<FluidOunceLength, 3, 1>
      >;
   };

   struct Cups
   {
      using units = std::tuple<
         UnitExponent<CupLength, 3, 1>
      >;
   };

   struct Pints
   {
      using units = std::tuple<
         UnitExponent<PintLength, 3, 1>
      >;
   };

   struct Quarts
   {
      using units = std::tuple<
         UnitExponent<QuartLength, 3, 1>
      >;
   };

   struct Gallons
   {
      using units = std::tuple<
         UnitExponent<GallonLength, 3, 1>
      >;
   };

   struct Teaspoons
   {
      using units = std::tuple<
         UnitExponent<TeaspoonLength, 3, 1>
      >;
   };

   struct Tablespoons
   {
      using units = std::tuple<
         UnitExponent<TablespoonLength, 3, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_VOLUME_H