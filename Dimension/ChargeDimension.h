#ifndef STATIC_DIMENSION_CHARGE_H
#define STATIC_DIMENSION_CHARGE_H

#include "Dimension_Impl/FundamentalDimensions/ChargeDimension_Impl.h"

namespace Dimension
{

   struct Coulombs         : public ChargeUnit<Coulombs, "Coulombs", "C"> {};
   struct StatCoulombs     : public ChargeUnit<StatCoulombs, "StatCoulombs", "esu"> {}; // Same as Franklins (fr)
   struct ElementaryCharges : public ChargeUnit<ElementaryCharges, "ElementaryCharge", "e"> {};

   template<> struct Conversion<Coulombs, StatCoulombs> { static constexpr PrecisionType slope = 2.9979245799996e9; };
   template<> struct Conversion<Coulombs, ElementaryCharges> { static constexpr PrecisionType slope = 6.241506363094e18; };

   ALL_SI_PREFIXES(Coulombs, "C", ChargeUnit);

}

#endif //STATIC_DIMENSION_CHARGE_H
