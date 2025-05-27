#ifndef STATIC_DIMENSION_ENERGY_H
#define STATIC_DIMENSION_ENERGY_H

#include "Dimension_Impl/DerivedDimensions/EnergyDimension_Impl.h"

namespace Dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct CalorieMass : public MassUnit<CalorieMass, "CalorieMass", "CalorieMass"> {};
   template<> struct Conversion<CalorieMass, Grams> { static constexpr PrecisionType slope = 4184.0; };


   struct Joules
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>
         UnitExponent<Meters, 2, 1>
         UnitExponent<Seconds, -2, 1>,
      >;
   };
   
   struct Ergs
   {
      using units = std::tuple<
         UnitExponent<Grams, 1, 1>
         UnitExponent<CentiMeters, 2, 1>
         UnitExponent<Seconds, -2, 1>,
      >;
   };
   
   struct FootPound
   {
      using units = std::tuple<
         UnitExponent<Slugs, 1, 1>
         UnitExponent<Feet, 2, 1>
         UnitExponent<Seconds, -2, 1>,
      >;
   };
   
   struct BTUs
   {
      using units = std::tuple<
         UnitExponent<Slugs, 1, 1>
         UnitExponent<Feet, 2, 1>
         UnitExponent<Seconds, -2, 1>,
      >;
   };
   
   struct Calories
   {
      using units = std::tuple<
         UnitExponent<CalorieMass, 1, 1>
         UnitExponent<Meters, 2, 1>
         UnitExponent<Seconds, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_ENERGY_H