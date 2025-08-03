#ifndef DIMENSIONAL_GET_FACTOR_H
#define DIMENSIONAL_GET_FACTOR_H

#include "convertible.h"
#include "exponent_math.h"

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
            using Primary = typename StartingUnit::Primary;
            return get_factor<StartingUnit, Primary>() * get_factor<Primary, TargetUnit>();
         }
      }
   }

   template<typename TargetUnit, typename Unit>
   constexpr double do_conversion(double value)
   {
      // Assumptions for now:
      // Only called on deltas (deal with this once Quantities are working)
      constexpr double scale = factor::get_factor<typename Unit::unit, TargetUnit>();

      if constexpr (Unit::exponent::den == 1)
      {
         return value * math::PowInt<Unit::exponent::num>(scale);
      }
      else
      {
         return value * math::RootInt<Unit::exponent::den>(math::PowInt<Unit::exponent::num>(scale));
      }
   }


}

#endif // DIMENSIONAL_GET_FACTOR_H