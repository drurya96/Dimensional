#ifndef STATIC_DIMENSION_LENGTH_H
#define STATIC_DIMENSION_LENGTH_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/length_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Only really need empty struct - WIP
   struct meters : public lengthUnit<meters> {
      static constexpr std::string_view name_view = "meters";
      static constexpr std::string_view abbr_view = "m";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<meters>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "meters";
      static constexpr std::string_view abbr = "m";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct feet : public lengthUnit<feet> {
      static constexpr std::string_view name_view = "Feet";
      static constexpr std::string_view abbr_view = "ft";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<feet>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Feet";
      static constexpr std::string_view abbr = "ft";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct inches : public lengthUnit<inches> {
      static constexpr std::string_view name_view = "Inches";
      static constexpr std::string_view abbr_view = "in";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<inches>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Inches";
      static constexpr std::string_view abbr = "in";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct astronomical_units : public lengthUnit<astronomical_units> {
      static constexpr std::string_view name_view = "Astronomical Units";
      static constexpr std::string_view abbr_view = "au";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<astronomical_units>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Astronomical Units";
      static constexpr std::string_view abbr = "au";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct data_miles : public lengthUnit<data_miles> {
      static constexpr std::string_view name_view = "Data Miles";
      static constexpr std::string_view abbr_view = "DataMiles";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<data_miles>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Data Miles";
      static constexpr std::string_view abbr = "DataMiles";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct nautical_miles : public lengthUnit<nautical_miles> {
      static constexpr std::string_view name_view = "Nautical Miles";
      static constexpr std::string_view abbr_view = "nmi";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<nautical_miles>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Nautical Miles";
      static constexpr std::string_view abbr = "nmi";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct miles : public lengthUnit<miles> {
      static constexpr std::string_view name_view = "Miles";
      static constexpr std::string_view abbr_view = "mi";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<miles>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Miles";
      static constexpr std::string_view abbr = "mi";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct fathoms : public lengthUnit<fathoms> {
      static constexpr std::string_view name_view = "Fathoms";
      static constexpr std::string_view abbr_view = "ftm";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<fathoms>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Fathoms";
      static constexpr std::string_view abbr = "ftm";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct furlong : public lengthUnit<furlong> {
      static constexpr std::string_view name_view = "Furlong";
      static constexpr std::string_view abbr_view = "fur";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<furlong>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Furlong";
      static constexpr std::string_view abbr = "fur";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct yards : public lengthUnit<yards> {
      static constexpr std::string_view name_view = "Yards";
      static constexpr std::string_view abbr_view = "yd";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<yards>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "Yards";
      static constexpr std::string_view abbr = "yd";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct us_survey_feet : public lengthUnit<us_survey_feet> {
      static constexpr std::string_view name_view = "US Survey Feet";
      static constexpr std::string_view abbr_view = "ftUS";
      static constexpr std::string_view dimName_view = "length";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<us_survey_feet>{
      using dimension = lengthType; // Consider renaming to length_dimension
      static constexpr std::string_view name = "US Survey Feet";
      static constexpr std::string_view abbr = "ftUS";
      static constexpr std::string_view dimName = "length";
      static constexpr int id = 0;
   };


   template<> struct Conversion<meters, feet> { static constexpr double slope = (1250.0 / 381.0); };
   template<> struct Conversion<meters, inches> { static constexpr double slope = (15000.0 / 381.0); };
   template<> struct Conversion<meters, astronomical_units> { static constexpr double slope = (1.0 / 149597870700.0); };
   template<> struct Conversion<meters, data_miles> { static constexpr double slope = (5.0 / 9144.0); };
   template<> struct Conversion<meters, nautical_miles> { static constexpr double slope = (1.0 / 1852.0); };
   template<> struct Conversion<meters, miles> { static constexpr double slope = (125.0 / 201168.0); };
   template<> struct Conversion<meters, fathoms> { static constexpr double slope = (3937.0 / 7200.0); };
   template<> struct Conversion<meters, furlong> { static constexpr double slope = (3937.0 / 792000.0); };
   template<> struct Conversion<meters, yards> { static constexpr double slope = (1250.0 / 1143.0); };
   template<> struct Conversion<meters, us_survey_feet> { static constexpr double slope = (3937.0 / 1200.0); };

   ALL_SI_PREFIXES(meters, "m", lengthUnit);

}




#endif //STATIC_DIMENSION_LENGTH_H