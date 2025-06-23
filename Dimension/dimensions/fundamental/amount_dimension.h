#ifndef STATIC_DIMENSION_AMOUNT_H
#define STATIC_DIMENSION_AMOUNT_H

#include "../Dimension_Impl/FundamentalDimensions/amount_dimension_Impl.h"

namespace dimension
{
   struct moles : public amountUnit<moles, "Moles", "mol"> {};
   struct pound_moles : public amountUnit<pound_moles, "Pound Moles", "lbmol"> {};

   template<> struct Conversion<moles, pound_moles> { static constexpr PrecisionType slope = (100000.0 / 45359237.0); };

   ALL_SI_PREFIXES(moles, "mol", amountUnit);

}




#endif //STATIC_DIMENSION_AMOUNT_H