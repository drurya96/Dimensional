#ifndef STATIC_DIMENSION_CHARGE_H
#define STATIC_DIMENSION_CHARGE_H

#include "Dimension_Impl/FundamentalDimensions/charge_dimension_Impl.h"

namespace dimension
{
   struct coulombs : public chargeUnit<coulombs, "Coulombs", "C"> {};
   struct elementary_charges : public chargeUnit<elementary_charges, "Elementary charges", "e"> {};

   template<> struct Conversion<coulombs, elementary_charges> { static constexpr PrecisionType slope = (5e+27 / 801088317.0); };

   ALL_SI_PREFIXES(coulombs, "C", chargeUnit);

}




#endif //STATIC_DIMENSION_CHARGE_H