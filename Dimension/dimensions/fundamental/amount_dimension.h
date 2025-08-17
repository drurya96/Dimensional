#ifndef STATIC_DIMENSION_AMOUNT_H
#define STATIC_DIMENSION_AMOUNT_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/amount_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"

namespace dimension
{
   // Only really need empty struct - WIP
   struct moles : public amountUnit<moles> {
      static constexpr std::string_view name_view = "Moles";
      static constexpr std::string_view abbr_view = "mol";
      static constexpr std::string_view dimName_view = "amount";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<moles>{
      using dimension = amountType; // Consider renaming to amount_dimension
      static constexpr std::string_view name = "Moles";
      static constexpr std::string_view abbr = "mol";
      static constexpr std::string_view dimName = "amount";
      static constexpr int id = 0;
   };

   // Only really need empty struct - WIP
   struct pound_moles : public amountUnit<pound_moles> {
      static constexpr std::string_view name_view = "Pound Moles";
      static constexpr std::string_view abbr_view = "lbmol";
      static constexpr std::string_view dimName_view = "amount";
   };

   // Should only need this part - WIP
   template<> struct unit_traits<pound_moles>{
      using dimension = amountType; // Consider renaming to amount_dimension
      static constexpr std::string_view name = "Pound Moles";
      static constexpr std::string_view abbr = "lbmol";
      static constexpr std::string_view dimName = "amount";
      static constexpr int id = 0;
   };


   template<> struct Conversion<moles, pound_moles> { static constexpr double slope = (100000.0 / 45359237.0); };

   ALL_SI_PREFIXES(moles, "mol", amountUnit);

}




#endif //STATIC_DIMENSION_AMOUNT_H