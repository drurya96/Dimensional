#ifndef DIMENSIONAL_GET_FACTOR_H
#define DIMENSIONAL_GET_FACTOR_H

#include "convertible.h"
#include "exponentiation/exponentiation.h"

namespace dimension::details {

   namespace factor
   {
      template<typename StartingUnit, typename TargetUnit>
      constexpr double get_factor()
      {
         if constexpr (std::is_same_v<StartingUnit, TargetUnit>)
         {
            return 1.0;
         }
         else if constexpr (HasConversion<StartingUnit, TargetUnit>)
         {
            return Conversion<StartingUnit, TargetUnit>::slope;
         }
         else if constexpr (HasConversion<TargetUnit, StartingUnit>)
         {
            return 1 / Conversion<TargetUnit, StartingUnit>::slope;
         }
         else
         {
            return get_factor<StartingUnit, unit_primary_t<StartingUnit>>() * get_factor<unit_primary_t<StartingUnit>, TargetUnit>();
         }
      }
   }

   template<typename TargetUnit, typename Unit>
   constexpr double do_conversion(double value)
   {
      return value * pow_rational(
         factor::get_factor<typename Unit::unit, TargetUnit>(),
         typename Unit::exponent{}
      );
   }


}

#endif // DIMENSIONAL_GET_FACTOR_H