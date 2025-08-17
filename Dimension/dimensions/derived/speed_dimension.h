#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "Dimension_Impl/DerivedDimensions/speed_dimension_Impl.h"
#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{

   // This should only need to be an empty struct - WIP
   struct knots
   {
      using units = std::tuple<
         unit_exponent<nautical_miles, 1, 1>,
         unit_exponent<hours, -1, 1>
      >;
   };

   template<> struct unit_traits<knots> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<nautical_miles, 1, 1>,
         unit_exponent<hours, -1, 1>
      >;
  };

}


#endif // STATIC_DIMENSION_SPEED_H