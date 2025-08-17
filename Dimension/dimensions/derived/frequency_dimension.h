#ifndef STATIC_DIMENSION_FREQUENCY_H
#define STATIC_DIMENSION_FREQUENCY_H

#include "Dimension_Impl/DerivedDimensions/frequency_dimension_Impl.h"
#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{

   // This should only need to be an empty struct - WIP
   struct hertz
   {
      using units = std::tuple<
         unit_exponent<seconds, -1, 1>
      >;
   };

   template<> struct unit_traits<hertz> {
      //using dimension = energy_dimension;
      //static constexpr std::string_view name = "Joule";
      //static constexpr std::string_view abbr = "J";
      //static constexpr int id = 0;

      using units = std::tuple<
         unit_exponent<seconds, -1, 1>
      >;
  };

}


#endif // STATIC_DIMENSION_FREQUENCY_H