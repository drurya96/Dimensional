#ifndef STATIC_DIMENSION_ENERGY_H
#define STATIC_DIMENSION_ENERGY_H

#include "Dimension_Impl/DerivedDimensions/energy_dimension_Impl.h"
#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Helper fundamental units that exist only to compose derived units
   //   These don't map to typical physical units, but are necessary to
   //   produce some derived units
   struct calorie_mass : public massUnit<calorie_mass> {};
   template<> struct Conversion<calorie_mass, grams> { static constexpr double slope = 4184.0; };


   // This should only need to be an empty struct - WIP
   struct joules
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   template<> struct unit_traits<joules> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
  };

   struct foot_pounds
   {
      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   template<> struct unit_traits<foot_pounds> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
  };

   struct btus
   {
      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   template<> struct unit_traits<btus> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
  };

   struct calories
   {
      using units = std::tuple<
         unit_exponent<calorie_mass, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   template<> struct unit_traits<calories> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<calorie_mass, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -2, 1>
      >;
  };

}


#endif // STATIC_DIMENSION_ENERGY_H