#ifndef DIMENSION_CANCEL_UNITS_H
#define DIMENSION_CANCEL_UNITS_H

#include "GenericTypeTraits.h"
#include "Conversion.h"

namespace Dimension
{
   /// @brief Unit cancellation implementation
   /// @details recursive base-case
   template<bool inverse = false, int index = 0, typename RealTupType, typename IncomingTupType, bool isDelta = false>
   requires (index == std::tuple_size_v<IncomingTupType>)
   void CancelUnitsImpl(PrecisionType&)
   {
      return;
   }

   /// @brief Unit cancellation implementation
   /// @details Updates the given value when units are cancelled,
   ///    and updates the given unit tuple when a unit is not cancelled.
   /// @tparam inverse bool indicating whether the item is in numerator (false) or denominator (true)
   /// @tparam index The tuple index to attempt cancelling
   /// @tparam IncomingTupType The types to attempt cancelling
   /// @tparam RealTupType The types to update as needed
   /// @tparam isDelata Bool indicating whether this conversion is of a single unit in the normator (false)
   ///    or not (true).
   /// @param[in,out] value The value to update when cancelling units
   template<bool inverse = false, int index = 0, typename RealTupType, typename IncomingTupType, bool isDelta = false>
   requires (index < std::tuple_size_v<IncomingTupType>)
   void CancelUnitsImpl(PrecisionType& value)
   {
      using currentType = std::tuple_element_t<index, IncomingTupType>;
      if constexpr (has_same_dim<currentType, RealTupType>::value)
      {
         value = Convert<currentType, typename get_first_match<is_same_dim, currentType, RealTupType>::type, isDelta, inverse>(value);
         CancelUnitsImpl<inverse, index + 1, typename RemoveOneInstance<is_same_dim, currentType, RealTupType>::type, IncomingTupType, isDelta>(value);
      }
      else
      {
         value = Convert<currentType, typename currentType::Primary, isDelta, inverse>(value);
         CancelUnitsImpl<inverse, index + 1, RealTupType, IncomingTupType, isDelta>(value);
      }
   }

   /// @brief Perform unit cancellation
   /// @tparam NumTupType Incoming numerator types
   /// @tparam DenTupType Incoming denominator types
   /// @tparam RealNumTupType Numerator types to update
   /// @tparam RealDenTupType Denominator types to udpate
   /// @tparam isDelata Bool indicating whether this conversion is of a single unit in the normator (false)
   ///    or not (true).
   /// @param[in,out] value Value to update
   template<typename NumTupType, typename DenTupType, typename RealNumTupType, typename RealDenTupType, bool isDelta = false>
   void CancelUnits(PrecisionType& value)
   {
      CancelUnitsImpl<false, 0, RealNumTupType, NumTupType, isDelta>(value);
      CancelUnitsImpl<true, 0, RealDenTupType, DenTupType, isDelta>(value);
   }

} // end Dimension

#endif // DIMENSION_CANCEL_UNITS_H