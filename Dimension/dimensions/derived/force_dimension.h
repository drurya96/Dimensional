#ifndef STATIC_DIMENSION_FORCE_H
#define STATIC_DIMENSION_FORCE_H

#include "Dimension_Impl/DerivedDimensions/force_dimension_Impl.h"
#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{

   // This should only need to be an empty struct - WIP
   struct newtons
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   template<> struct unit_traits<newtons> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
  };

   struct pound_force
   {
      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
   };

   template<> struct unit_traits<pound_force> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<slugs, 1, 1>,
         unit_exponent<feet, 1, 1>,
         unit_exponent<seconds, -2, 1>
      >;
  };

}


#endif // STATIC_DIMENSION_FORCE_H