#ifndef STATIC_DIMENSION_MASS_H
#define STATIC_DIMENSION_MASS_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/mass_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Only really need empty struct - WIP
   struct grams : public massUnit<grams> {
      static constexpr std::string_view name_view = "Grams";
      static constexpr std::string_view abbr_view = "g";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<grams>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Grams";
      static constexpr std::string_view abbr = "g";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct pound_mass : public massUnit<pound_mass> {
      static constexpr std::string_view name_view = "Pound mass";
      static constexpr std::string_view abbr_view = "lb";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<pound_mass>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Pound mass";
      static constexpr std::string_view abbr = "lb";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct ounces : public massUnit<ounces> {
      static constexpr std::string_view name_view = "Ounces";
      static constexpr std::string_view abbr_view = "oz";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<ounces>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Ounces";
      static constexpr std::string_view abbr = "oz";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct slugs : public massUnit<slugs> {
      static constexpr std::string_view name_view = "Slugs";
      static constexpr std::string_view abbr_view = "slug";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<slugs>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Slugs";
      static constexpr std::string_view abbr = "slug";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct grains : public massUnit<grains> {
      static constexpr std::string_view name_view = "Grains";
      static constexpr std::string_view abbr_view = "gr";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<grains>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Grains";
      static constexpr std::string_view abbr = "gr";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct stone : public massUnit<stone> {
      static constexpr std::string_view name_view = "Stone";
      static constexpr std::string_view abbr_view = "st";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<stone>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Stone";
      static constexpr std::string_view abbr = "st";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct short_ton : public massUnit<short_ton> {
      static constexpr std::string_view name_view = "Short Ton";
      static constexpr std::string_view abbr_view = "ton";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<short_ton>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Short Ton";
      static constexpr std::string_view abbr = "ton";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct long_ton : public massUnit<long_ton> {
      static constexpr std::string_view name_view = "Long Ton";
      static constexpr std::string_view abbr_view = "LT";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<long_ton>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Long Ton";
      static constexpr std::string_view abbr = "LT";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct tonne : public massUnit<tonne> {
      static constexpr std::string_view name_view = "Tonne";
      static constexpr std::string_view abbr_view = "t";
      static constexpr std::string_view dimName_view = "mass";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<tonne>{
      using dimension = massType; // Consider renaming to mass_dimension
      static constexpr std::string_view name = "Tonne";
      static constexpr std::string_view abbr = "t";
      static constexpr std::string_view dimName = "mass";
      static constexpr int id = 0;
   };


   template<> struct Conversion<grams, pound_mass> { static constexpr double slope = (100000.0 / 45359237.0); };
   template<> struct Conversion<grams, ounces> { static constexpr double slope = (1600000.0 / 45359237.0); };
   template<> struct Conversion<grams, slugs> { static constexpr double slope = (609600000.0 / 8896443230521.0); };
   template<> struct Conversion<grams, grains> { static constexpr double slope = (100000000.0 / 6479891.0); };
   template<> struct Conversion<grams, stone> { static constexpr double slope = (50000.0 / 317514659.0); };
   template<> struct Conversion<grams, short_ton> { static constexpr double slope = (50.0 / 45359237.0); };
   template<> struct Conversion<grams, long_ton> { static constexpr double slope = (625.0 / 635029318.0); };
   template<> struct Conversion<grams, tonne> { static constexpr double slope = 1e-06; };

   ALL_SI_PREFIXES(grams, "g", massUnit);

}




#endif //STATIC_DIMENSION_MASS_H