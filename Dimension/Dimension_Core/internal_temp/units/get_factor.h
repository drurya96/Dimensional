#ifndef DIMENSIONAL_GET_FACTOR_H
#define DIMENSIONAL_GET_FACTOR_H

#include "convertible.h"
#include "../exponentiation/exponentiation.h"
#include "factor.h"

namespace dimension::details {

   namespace factor
   {

      template<typename StartingUnit, typename TargetUnit>
      requires HasConversion<StartingUnit, TargetUnit>
      constexpr double get_raw_factor()
      {
         return dimension::factor::eval_factor<typename Conversion<StartingUnit, TargetUnit>::scale, double>();
      }

      template<typename StartingUnit, typename TargetUnit>
      constexpr double get_factor()
      {
         if constexpr (std::is_same_v<StartingUnit, TargetUnit>)
         {
            return 1.0;
         }
         else if constexpr (HasConversion<StartingUnit, TargetUnit>)
         {
            return get_raw_factor<StartingUnit, TargetUnit>();
         }
         else if constexpr (HasConversion<TargetUnit, StartingUnit>)
         {
            return 1 / get_raw_factor<TargetUnit, StartingUnit>();
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