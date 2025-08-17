#ifndef STATIC_DIMENSION_ANGLE_H
#define STATIC_DIMENSION_ANGLE_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/angle_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Only really need empty struct - WIP
   struct radians : public angleUnit<radians> {
      static constexpr std::string_view name_view = "radians";
      static constexpr std::string_view abbr_view = "rad";
      static constexpr std::string_view dimName_view = "angle";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<radians>{
      using dimension = angleType; // Consider renaming to angle_dimension
      static constexpr std::string_view name = "radians";
      static constexpr std::string_view abbr = "rad";
      static constexpr std::string_view dimName = "angle";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct degrees : public angleUnit<degrees> {
      static constexpr std::string_view name_view = "Degrees";
      static constexpr std::string_view abbr_view = "deg";
      static constexpr std::string_view dimName_view = "angle";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<degrees>{
      using dimension = angleType; // Consider renaming to angle_dimension
      static constexpr std::string_view name = "Degrees";
      static constexpr std::string_view abbr = "deg";
      static constexpr std::string_view dimName = "angle";
      static constexpr int id = 0;
   };


   template<> struct Conversion<radians, degrees> { static constexpr double slope = 57.2958; };


}



#include "extensions/angleExtras.h"

#endif //STATIC_DIMENSION_ANGLE_H