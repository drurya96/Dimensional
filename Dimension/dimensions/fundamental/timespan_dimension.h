#ifndef STATIC_DIMENSION_TIMESPAN_H
#define STATIC_DIMENSION_TIMESPAN_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/timespan_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Only really need empty struct - WIP
   struct seconds : public timespanUnit<seconds> {
      static constexpr std::string_view name_view = "seconds";
      static constexpr std::string_view abbr_view = "s";
      static constexpr std::string_view dimName_view = "timespan";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<seconds>{
      using dimension = timespanType; // Consider renaming to timespan_dimension
      static constexpr std::string_view name = "seconds";
      static constexpr std::string_view abbr = "s";
      static constexpr std::string_view dimName = "timespan";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct minutes : public timespanUnit<minutes> {
      static constexpr std::string_view name_view = "minutes";
      static constexpr std::string_view abbr_view = "min";
      static constexpr std::string_view dimName_view = "timespan";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<minutes>{
      using dimension = timespanType; // Consider renaming to timespan_dimension
      static constexpr std::string_view name = "minutes";
      static constexpr std::string_view abbr = "min";
      static constexpr std::string_view dimName = "timespan";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct hours : public timespanUnit<hours> {
      static constexpr std::string_view name_view = "Hours";
      static constexpr std::string_view abbr_view = "h";
      static constexpr std::string_view dimName_view = "timespan";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<hours>{
      using dimension = timespanType; // Consider renaming to timespan_dimension
      static constexpr std::string_view name = "Hours";
      static constexpr std::string_view abbr = "h";
      static constexpr std::string_view dimName = "timespan";
      static constexpr int id = 0;
   };


   template<> struct Conversion<seconds, minutes> { static constexpr double slope = (1.0 / 60.0); };
   template<> struct Conversion<seconds, hours> { static constexpr double slope = (1.0 / 3600.0); };

   ALL_SI_PREFIXES(seconds, "s", timespanUnit);

}




#endif //STATIC_DIMENSION_TIMESPAN_H