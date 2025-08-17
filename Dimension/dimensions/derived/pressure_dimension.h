#ifndef STATIC_DIMENSION_PRESSURE_H
#define STATIC_DIMENSION_PRESSURE_H

#include "Dimension_Impl/DerivedDimensions/pressure_dimension_Impl.h"
#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct atmosphere_mass : public massUnit<atmosphere_mass> {};
   template<> struct Conversion<atmosphere_mass, grams> { static constexpr double slope = 101325000.0; };

   struct bar_mass : public massUnit<bar_mass> {};
   template<> struct Conversion<bar_mass, grams> { static constexpr double slope = 100000000.0; };

   struct torr_mass : public massUnit<torr_mass> {};
   template<> struct Conversion<torr_mass, grams> { static constexpr double slope = 133322.31202220617; };

   struct in_hg_mass : public massUnit<in_hg_mass> {};
   template<> struct Conversion<in_hg_mass, grams> { static constexpr double slope = 3386386.725364; };


   // This should only need to be an empty struct - WIP
   struct pascals
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, -1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   template<> struct unit_traits<pascals> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

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

   template<> struct unit_traits<atmospheres> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

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

   template<> struct unit_traits<bar> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

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

   template<> struct unit_traits<torr> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

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

   template<> struct unit_traits<in_hg> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

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

   template<> struct unit_traits<mm_hg> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<torr_mass, 1, 1>,
         unit_exponent<meters, -1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
  };

}


#endif // STATIC_DIMENSION_PRESSURE_H