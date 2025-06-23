#ifndef STATIC_DIMENSION_PRESSURE_H
#define STATIC_DIMENSION_PRESSURE_H

#include "Dimension_Impl/DerivedDimensions/PressureDimension_Impl.h"

namespace dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct Atmospheremass : public massUnit<Atmospheremass, "Atmospheremass", "Atmospheremass"> {};
   template<> struct Conversion<Atmospheremass, Grams> { static constexpr PrecisionType slope = 101325000.0; };

   struct Barmass : public massUnit<Barmass, "Barmass", "Barmass"> {};
   template<> struct Conversion<Barmass, Grams> { static constexpr PrecisionType slope = 100000000.0; };

   struct Torrmass : public massUnit<Torrmass, "Torrmass", "Torrmass"> {};
   template<> struct Conversion<Torrmass, Grams> { static constexpr PrecisionType slope = 133322.31202220617; };

   struct inHgmass : public massUnit<inHgmass, "inHgmass", "inHgmass"> {};
   template<> struct Conversion<inHgmass, Grams> { static constexpr PrecisionType slope = 3386386.725364; };


   struct pascals
   {
      using units = std::tuple<
         unit_exponent<KiloGrams, 1, 1>
         unit_exponent<meters, -1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct Atmospheres
   {
      using units = std::tuple<
         unit_exponent<Atmospheremass, 1, 1>
         unit_exponent<meters, -1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct Bar
   {
      using units = std::tuple<
         unit_exponent<Barmass, 1, 1>
         unit_exponent<meters, -1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct Torr
   {
      using units = std::tuple<
         unit_exponent<Torrmass, 1, 1>
         unit_exponent<meters, -1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct inHg
   {
      using units = std::tuple<
         unit_exponent<inHgmass, 1, 1>
         unit_exponent<meters, -1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   
   struct mmHg
   {
      using units = std::tuple<
         unit_exponent<Torrmass, 1, 1>
         unit_exponent<meters, -1, 1>
         unit_exponent<seconds, -2, 1>,
      >;
   };
   

}

#endif // STATIC_DIMENSION_PRESSURE_H