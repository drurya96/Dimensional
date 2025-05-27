#ifndef STATIC_DIMENSION_PRESSURE_H
#define STATIC_DIMENSION_PRESSURE_H

#include "Dimension_Impl/DerivedDimensions/PressureDimension_Impl.h"

namespace Dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct AtmosphereMass : public MassUnit<AtmosphereMass, "AtmosphereMass", "AtmosphereMass"> {};
   template<> struct Conversion<AtmosphereMass, Grams> { static constexpr PrecisionType slope = 101325000.0; };

   struct BarMass : public MassUnit<BarMass, "BarMass", "BarMass"> {};
   template<> struct Conversion<BarMass, Grams> { static constexpr PrecisionType slope = 100000000.0; };

   struct TorrMass : public MassUnit<TorrMass, "TorrMass", "TorrMass"> {};
   template<> struct Conversion<TorrMass, Grams> { static constexpr PrecisionType slope = 133322.31202220617; };

   struct inHgMass : public MassUnit<inHgMass, "inHgMass", "inHgMass"> {};
   template<> struct Conversion<inHgMass, Grams> { static constexpr PrecisionType slope = 3386386.725364; };


   struct Pascals
   {
      using units = std::tuple<
         UnitExponent<KiloGrams, 1, 1>,
         UnitExponent<Meters, -1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

   struct Atmospheres
   {
      using units = std::tuple<
         UnitExponent<AtmosphereMass, 1, 1>,
         UnitExponent<Meters, -1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

   struct Bar
   {
      using units = std::tuple<
         UnitExponent<BarMass, 1, 1>,
         UnitExponent<Meters, -1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

   struct Torr
   {
      using units = std::tuple<
         UnitExponent<TorrMass, 1, 1>,
         UnitExponent<Meters, -1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

   struct inHg
   {
      using units = std::tuple<
         UnitExponent<inHgMass, 1, 1>,
         UnitExponent<Meters, -1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

   struct mmHg
   {
      using units = std::tuple<
         UnitExponent<TorrMass, 1, 1>,
         UnitExponent<Meters, -1, 1>,
         UnitExponent<Seconds, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_PRESSURE_H