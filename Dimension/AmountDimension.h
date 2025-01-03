#ifndef STATIC_DIMENSION_AMOUNT_H
#define STATIC_DIMENSION_AMOUNT_H

#include "Dimension_Impl/FundamentalDimensions/AmountDimension_Impl.h"

namespace Dimension
{
   struct Moles : public AmountUnit<Moles, "Moles", "mol"> {};
   struct PoundMoles : public AmountUnit<PoundMoles, "PoundMoles", "lbmol"> {};

   template<> struct Conversion<Moles, PoundMoles> { static constexpr PrecisionType slope = (1.0 / 453.59237); };

}

#endif //STATIC_DIMENSION_AMOUNT_H
