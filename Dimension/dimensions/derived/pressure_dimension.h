#ifndef STATIC_DIMENSION_PRESSURE_H
#define STATIC_DIMENSION_PRESSURE_H

#include "Dimension_Impl/DerivedDimensions/pressure_dimension_Impl.h"

namespace dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct atmosphere_mass : public massUnit<atmosphere_mass, "Atmospheremass", "Atmospheremass"> {};
   template<> struct Conversion<atmosphere_mass, grams> { static constexpr PrecisionType slope = 101325000.0; };

   struct bar_mass : public massUnit<bar_mass, "Barmass", "Barmass"> {};
   template<> struct Conversion<bar_mass, grams> { static constexpr PrecisionType slope = 100000000.0; };

   struct torr_mass : public massUnit<torr_mass, "Torrmass", "Torrmass"> {};
   template<> struct Conversion<torr_mass, grams> { static constexpr PrecisionType slope = 133322.31202220617; };

   struct in_hg_mass : public massUnit<in_hg_mass, "inHgmass", "inHgmass"> {};
   template<> struct Conversion<in_hg_mass, grams> { static constexpr PrecisionType slope = 3386386.725364; };


   struct pascals
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, -1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct atmospheres
   {
      using units = std::tuple<
         unit_exponent<atmosphere_mass, 1, 1>,
         unit_exponent<meters, -1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct bar
   {
      using units = std::tuple<
         unit_exponent<bar_mass, 1, 1>,
         unit_exponent<meters, -1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct torr
   {
      using units = std::tuple<
         unit_exponent<torr_mass, 1, 1>,
         unit_exponent<meters, -1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct in_hg
   {
      using units = std::tuple<
         unit_exponent<in_hg_mass, 1, 1>,
         unit_exponent<meters, -1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   struct mm_hg
   {
      using units = std::tuple<
         unit_exponent<torr_mass, 1, 1>,
         unit_exponent<meters, -1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

}


#endif // STATIC_DIMENSION_PRESSURE_H