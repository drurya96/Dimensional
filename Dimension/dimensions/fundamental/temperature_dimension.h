#ifndef STATIC_DIMENSION_TEMPERATURE_H
#define STATIC_DIMENSION_TEMPERATURE_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/temperature_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Only really need empty struct - WIP
   struct kelvin : public temperatureUnit<kelvin> {
      static constexpr std::string_view name_view = "Kelvin";
      static constexpr std::string_view abbr_view = "K";
      static constexpr std::string_view dimName_view = "temperature";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<kelvin>{
      using dimension = temperatureType; // Consider renaming to temperature_dimension
      static constexpr std::string_view name = "Kelvin";
      static constexpr std::string_view abbr = "K";
      static constexpr std::string_view dimName = "temperature";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct rankine : public temperatureUnit<rankine> {
      static constexpr std::string_view name_view = "Rankine";
      static constexpr std::string_view abbr_view = "R";
      static constexpr std::string_view dimName_view = "temperature";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<rankine>{
      using dimension = temperatureType; // Consider renaming to temperature_dimension
      static constexpr std::string_view name = "Rankine";
      static constexpr std::string_view abbr = "R";
      static constexpr std::string_view dimName = "temperature";
      static constexpr int id = 0;
   };


   template<> struct Conversion<kelvin, rankine> { static constexpr double slope = (9.0 / 5.0); };


}



#include "extensions/temperatureExtras.h"

#endif //STATIC_DIMENSION_TEMPERATURE_H