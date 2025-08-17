#ifndef STATIC_DIMENSION_POWER_H
#define STATIC_DIMENSION_POWER_H

#include "Dimension_Impl/DerivedDimensions/power_dimension_Impl.h"
#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{

   // This should only need to be an empty struct - WIP
   struct Watts
   {
      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -3, 1>
      >;
   };

   template<> struct unit_traits<Watts> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<kilo_grams, 1, 1>,
         unit_exponent<meters, 2, 1>,
         unit_exponent<seconds, -3, 1>
      >;
  };

}


#endif // STATIC_DIMENSION_POWER_H