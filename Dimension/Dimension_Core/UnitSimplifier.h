#ifndef DIMENSION_UNIT_SIMPLIFIER_H
#define DIMENSION_UNIT_SIMPLIFIER_H

#include "TupleHandling.h"
#include "Conversion.h"
#include "FundamentalUnitExtractor.h"
#include "UnitValidation.h"

namespace Dimension
{

   template<typename NumTuple, typename DenTuple, int Index = 0>
   struct has_matching_quantity;

   template<typename NumTuple, typename DenTuple, int Index>
   requires (Index == std::tuple_size_v<NumTuple>)
   struct has_matching_quantity<NumTuple, DenTuple, Index>
   {
      static constexpr bool value = false;
   };

   template<typename NumTuple, typename DenTuple, int Index>
   requires (Index < std::tuple_size_v<NumTuple>)
   struct has_matching_quantity<NumTuple, DenTuple, Index>
   {
      static constexpr bool value = has_same_quantity_dim<std::tuple_element_t<Index, NumTuple>, DenTuple>::value ||
         has_matching_quantity<NumTuple, DenTuple, Index + 1>::value;
   };

   template<typename NumTuple, typename DenTuple>
   concept NoNonAbsoluteQuantitiesInCompoundDimension = 
                  (!has_non_absolute_quantity_v<NumTuple> && !has_non_absolute_quantity_v<DenTuple>) ||
                  (std::tuple_size_v<NumTuple> == 1 && std::tuple_size_v<DenTuple> == 0);

   template<typename NumTuple, typename DenTuple>
   concept IsUnitTuplePair = IsUnitTuple<typename FundamentalUnitExtractor<NumTuple, DenTuple>::Num> &&
                             IsUnitTuple<typename FundamentalUnitExtractor<NumTuple, DenTuple>::Den> &&
                             NoNonAbsoluteQuantitiesInCompoundDimension<NumTuple, DenTuple> &&
                             !has_matching_quantity<NumTuple, DenTuple>::value;

   // Forward declarations
   template<typename NumTuple, typename DenTuple>
   requires IsUnitTuplePair<NumTuple, DenTuple>
   class BaseDimension;



   /// @brief Struct to simplify units by cancelling out as necessary
   template<typename NumTypes1, typename NumTypes2, typename DenTypes1, typename DenTypes2>
   struct UnitSimplifier;

   /// @brief Struct to simplify units by cancelling out as necessary
   /// @tparam NumTypes1... The types in the first group of numerator types
   /// @tparam NumTypes2... The types in the second group of numerator types
   /// @tparam DenTypes1... The types in the first group of denominator types
   /// @tparam DenTypes2... The types in the second group of denominator types
   /// @typedef newNum The simplified numerator types
   /// @typedef newDen The simplified denominator types
   /// @typedef dimType A BaseDimension templated on the simplified types
   template<typename ... NumTypes1, typename ... NumTypes2, typename ... DenTypes1, typename ... DenTypes2>
   struct UnitSimplifier<std::tuple<NumTypes1...>, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>, std::tuple<DenTypes2...>>
   {
   private:
      using num1AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<NumTypes1...>, std::tuple<DenTypes2...>>;
      using num2AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>>;

      using den1AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<DenTypes1...>, std::tuple<NumTypes2...>>;
      using den2AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<DenTypes2...>, std::tuple<NumTypes1...>>;

      using remainingNum1 = tuple_diff<has_same_dim, typename num1AfterSimpleCancel::type, typename den2AfterSimpleCancel::type>;
      using remainingNum2 = tuple_diff<has_same_dim, typename num2AfterSimpleCancel::type, typename den1AfterSimpleCancel::type>;

      using remainingDen1 = tuple_diff<has_same_dim, typename den1AfterSimpleCancel::type, typename num2AfterSimpleCancel::type>;
      using remainingDen2 = tuple_diff<has_same_dim, typename den2AfterSimpleCancel::type, typename num1AfterSimpleCancel::type>;
      

   public:
      using newNum = tuple_cat_t<typename remainingNum1::type, typename remainingNum2::type>;
      using newDen = tuple_cat_t<typename remainingDen1::type, typename remainingDen2::type>;

      using numSimple = tuple_cat_t<typename num1AfterSimpleCancel::type, typename num2AfterSimpleCancel::type>;
      using denSimple = tuple_cat_t<typename den1AfterSimpleCancel::type, typename den2AfterSimpleCancel::type>;

      using dimType = BaseDimension<newNum, newDen>;

      constexpr static bool isDelta = !((std::tuple_size_v<newNum> == 1) && (std::tuple_size_v<newDen> == 0));
      constexpr static bool isScalar = (std::tuple_size_v<newNum> == 0) && (std::tuple_size_v<newDen> == 0);
   };

   /// @brief Unit cancellation implementation
   /// @details recursive base-case
   template<bool inverse = false, int index = 0, typename RealTupType, typename IncomingTupType>
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
   template<bool inverse = false, int index = 0, typename RealTupType, typename IncomingTupType>
   requires (index < std::tuple_size_v<IncomingTupType>)
   void CancelUnitsImpl(PrecisionType& value)
   {
      using currentType = std::tuple_element_t<index, IncomingTupType>;
      if constexpr (has_same_dim<currentType, RealTupType>::value)
      {
         value = Convert<currentType, typename get_first_match<is_same_dim, currentType, RealTupType>::type, inverse>(value);
         CancelUnitsImpl<inverse, index + 1, typename RemoveOneInstance<is_same_dim, currentType, RealTupType>::type, IncomingTupType>(value);
      }
      else
      {

         using primary = type_from_quantity_or_delta_t<currentType>::Primary;
         using realPrimary = std::conditional_t<is_quantity_v<currentType>, Quantity<primary>, primary>;

         value = Convert<currentType, realPrimary, inverse>(value);
         CancelUnitsImpl<inverse, index + 1, RealTupType, IncomingTupType>(value);
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
      CancelUnitsImpl<false, 0, RealNumTupType, NumTupType>(value);
      CancelUnitsImpl<true, 0, RealDenTupType, DenTupType>(value);
   }

   template<typename Dim1, typename Dim2>
   struct is_matching_dimension
   {
      using simplified = UnitSimplifier<typename Dim1::simplifiedNumTuple, typename Dim2::simplifiedDenTuple, typename Dim1::simplifiedDenTuple, typename Dim2::simplifiedNumTuple>;

      static constexpr bool value = simplified::isScalar;
   };

   template<typename Dim1, typename Dim2>
   concept MatchingDimensionsNew = is_matching_dimension<Dim1, Dim2>::value;

} // end Dimension

#endif // DIMENSION_UNIT_SIMPLIFIER_H