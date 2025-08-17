#ifndef STATIC_DIMENSION_CHARGE_H
#define STATIC_DIMENSION_CHARGE_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/charge_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Only really need empty struct - WIP
   struct coulombs : public chargeUnit<coulombs> {
      static constexpr std::string_view name_view = "Coulombs";
      static constexpr std::string_view abbr_view = "C";
      static constexpr std::string_view dimName_view = "charge";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<coulombs>{
      using dimension = chargeType; // Consider renaming to charge_dimension
      static constexpr std::string_view name = "Coulombs";
      static constexpr std::string_view abbr = "C";
      static constexpr std::string_view dimName = "charge";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct elementary_charges : public chargeUnit<elementary_charges> {
      static constexpr std::string_view name_view = "Elementary charges";
      static constexpr std::string_view abbr_view = "e";
      static constexpr std::string_view dimName_view = "charge";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<elementary_charges>{
      using dimension = chargeType; // Consider renaming to charge_dimension
      static constexpr std::string_view name = "Elementary charges";
      static constexpr std::string_view abbr = "e";
      static constexpr std::string_view dimName = "charge";
      static constexpr int id = 0;
   };


   template<> struct Conversion<coulombs, elementary_charges> { static constexpr double slope = (5e+27 / 801088317.0); };

   ALL_SI_PREFIXES(coulombs, "C", chargeUnit);

}




#endif //STATIC_DIMENSION_CHARGE_H