#ifndef DIMENSION_UNIT_SIMPLIFIER_H
#define DIMENSION_UNIT_SIMPLIFIER_H

#include <concepts>

#include "TupleHandling.h"
#include "Conversion.h"
#include "FundamentalUnitExtractor.h"
#include "UnitValidation.h"
#include "base_dimension_signature.h"

namespace dimension
{



   // TODO: This should live somewhere else
   // Make power a ratio type
   // Make Units a BaseUnit maybe?
   template<typename IncomingUnit, typename... Units>
   struct AddPower;

   template<typename IncomingUnit>
   struct AddPower<IncomingUnit, std::tuple<>>
   {
      static constexpr bool success = false;
      using units = std::tuple<>;
   };

   template<typename IncomingUnit, typename Unit>
   struct AddPower<IncomingUnit, std::tuple<Unit>>
   {
      // Use a conditional to set `units` to either the added value or empty tuple
      static constexpr bool success = std::is_same_v<typename IncomingUnit::unit, typename Unit::unit>;

      using added = unit_exponent<typename Unit::unit, std::ratio_add<typename Unit::exponent, typename IncomingUnit::exponent>::num, std::ratio_add<typename Unit::exponent, typename IncomingUnit::exponent>::den>;

      using units = std::conditional_t<success,
         std::tuple<added>,
         std::tuple<Unit>>;
   };

   template<typename IncomingUnit, typename Unit, typename... Units>
   struct AddPower<IncomingUnit, std::tuple<Unit, Units...>>
   {

      // Use conditional to check if AddPower was a real value or empty
      // If real, concatenate the rest and halt
      // If empty, continue
      using current = AddPower<IncomingUnit, std::tuple<Unit>>;

      using units = std::conditional_t<current::success,
         tuple_cat_t<typename current::units, std::tuple<Units...>>,
         tuple_cat_t<typename current::units, typename AddPower<IncomingUnit, std::tuple<Units...>>::units>>;

      using before = std::conditional_t<current::success,
         std::tuple<>,
         tuple_cat_t<std::tuple<Unit>>>;

      using added = std::conditional_t<current::success, typename current::added, void>;

      static constexpr bool success = current::success || AddPower<IncomingUnit, std::tuple<Units...>>::success;

   };



   









   template<typename IncomingUnit, typename... Units>
   struct CombinePower_Impl;

   template<typename IncomingUnit>
   struct CombinePower_Impl<IncomingUnit, std::tuple<>>
   {
      using exponent = std::ratio<0>;
   };

   template<typename IncomingUnit, typename Unit>
   struct CombinePower_Impl<IncomingUnit, std::tuple<Unit>>
   {
      using exponent = std::conditional_t<std::is_same_v<IncomingUnit, typename Unit::unit>,
         typename Unit::exponent,
         std::ratio<0>>;
   };

   template<typename IncomingUnit, typename Unit, typename... Units>
   struct CombinePower_Impl<IncomingUnit, std::tuple<Unit, Units...>>
   {
      using exponent = std::ratio_add<
         typename CombinePower_Impl<IncomingUnit, std::tuple<Unit>>::exponent,
         typename CombinePower_Impl<IncomingUnit, std::tuple<Units...>>::exponent
      >;
   };

   template<typename IncomingUnit, typename... Units>
   struct CombinePower;

   template<typename IncomingUnit, typename... Units>
   struct CombinePower<IncomingUnit, std::tuple<Units...>>
   {
      using exponent = typename CombinePower_Impl<IncomingUnit, std::tuple<Units...>>::exponent;
      //using exponent = std::ratio<2>;
      using unit = unit_exponent<IncomingUnit, exponent::num, exponent::den>;
   };










   template<typename IncomingUnit, typename... Units>
   struct UnitPresent_Impl;

   template<typename IncomingUnit>
   struct UnitPresent_Impl<IncomingUnit, std::tuple<>>
   {
      static constexpr bool found = false;
   };

   template<typename IncomingUnit, typename Unit>
   struct UnitPresent_Impl<IncomingUnit, std::tuple<Unit>>
   {
      static constexpr bool found = std::is_same_v<IncomingUnit, typename Unit::unit>;
   };

   template<typename IncomingUnit, typename Unit, typename... Units>
   struct UnitPresent_Impl<IncomingUnit, std::tuple<Unit, Units...>>
   {
      static constexpr bool found = 
         UnitPresent_Impl<IncomingUnit, std::tuple<Unit>>::found ||
         UnitPresent_Impl<IncomingUnit, std::tuple<Units...>>::found;
   };


   template<typename IncomingUnit, typename... Units>
   struct UnitPresent;

   template<typename IncomingUnit, typename... Units>
   struct UnitPresent<IncomingUnit, std::tuple<Units...>>
   {
      static constexpr bool found = UnitPresent_Impl<IncomingUnit, std::tuple<Units...>>::found;
   };

   template<typename R>
   struct is_zero_ratio : std::bool_constant<R::num == 0> {};

   template<typename... Units>
   struct RemoveZeros;

   template<>
   struct RemoveZeros<std::tuple<>>
   {
      using units = std::tuple<>;
   };

   template<typename Unit>
   struct RemoveZeros<std::tuple<Unit>>
   {
      using units = std::conditional_t<!is_zero_ratio<typename Unit::exponent>::value,
         std::tuple<Unit>,
         std::tuple<>>;
   };

   template<typename Unit, typename... Units>
   struct RemoveZeros<std::tuple<Unit, Units...>>
   {
      using units = std::conditional_t<!is_zero_ratio<typename Unit::exponent>::value,
         tuple_cat_t<std::tuple<Unit>, typename RemoveZeros<std::tuple<Units...>>::units>,
         tuple_cat_t<std::tuple<>, typename RemoveZeros<std::tuple<Units...>>::units>>;
   };










// extract fundamental
// for each unit
  // CombinePower for the unit with the remaining
  // Add combined unit to growing tuple
  // If unit is already in tuple, skip it
    // Check via UnitPresent




   /// @brief 
   /// @details 
   /// @tparam 
   template<typename IncomingTuple, typename... Units>
   struct InitialSimplifier_Impl;

   template<typename IncomingTuple>
   struct InitialSimplifier_Impl<IncomingTuple, std::tuple<>>
   {
      using units = IncomingTuple;
   };

   template<typename IncomingTuple, typename Unit>
   struct InitialSimplifier_Impl<IncomingTuple, std::tuple<Unit>>
   {
      //static constexpr bool already_used = UnitPresent<typename Unit::unit, IncomingTuple>::found;
      using units = std::conditional_t<UnitPresent<typename Unit::unit, IncomingTuple>::found,
         IncomingTuple,
         tuple_cat_t<IncomingTuple, std::tuple<Unit>>>;
   };

   template<typename IncomingTuple, typename Unit, typename... Units>
   struct InitialSimplifier_Impl<IncomingTuple, std::tuple<Unit, Units...>>
   {

      
      // Only used if needed...
      using newUnit = typename CombinePower<typename Unit::unit, std::tuple<Unit, Units...>>::unit;
      using newTuple = tuple_cat_t<IncomingTuple, std::tuple<newUnit>>;



      using units = std::conditional_t<UnitPresent<typename Unit::unit, IncomingTuple>::found,
         typename InitialSimplifier_Impl<IncomingTuple, std::tuple<Units...>>::units,
         typename InitialSimplifier_Impl<newTuple, std::tuple<Units...>>::units>;

      /*
      using units = std::conditional_t<UnitPresent<typename Unit::unit, IncomingTuple>::found,
         typename InitialSimplifier_Impl<IncomingTuple, std::tuple<Units...>>::units,
         typename InitialSimplifier_Impl<tuple_cat_t<IncomingTuple, std::tuple<typename CombinePower<typename Unit::unit, std::tuple<Units...>>::unit>>, std::tuple<Units...>>::units>;
      */

      /*
      static constexpr bool already_used = UnitPresent<typename Unit::unit, IncomingTuple>::found;

      // if already used
      using units = InitialSimplifier_Impl<IncomingTuple, std::tuple<Units...>>;

      // if not yet used
      using newUnit = typename CombinePower<typename Unit::unit, std::tuple<Units...>>::unit;
      using newTuple = std::tuple_cat_t<IncomingTuple, std::tuple<newUnit>>;
      using units = InitialSimplifier_Impl<newTuple, std::tuple<Units...>>;
      */
   };

   template<typename... Units>
   struct InitialSimplifier;

   template<typename... Units>
   struct InitialSimplifier<std::tuple<Units...>>
   {
      using units = typename RemoveZeros<typename InitialSimplifier_Impl<std::tuple<>, typename FundamentalUnitExtractor<std::tuple<Units...>>::units>::units>::units;
   };






























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
   /// @typedef dimType A base_dimension templated on the simplified types
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

      using dimType = base_dimension<newNum, newDen>;

      constexpr static bool isDelta = !((std::tuple_size_v<newNum> == 1) && (std::tuple_size_v<newDen> == 0));
      constexpr static bool isScalar = (std::tuple_size_v<newNum> == 0) && (std::tuple_size_v<newDen> == 0);
   };

   /// @brief Unit cancellation implementation
   /// @details recursive base-case
   template<bool inverse = false, int index = 0, typename RealTupType, typename IncomingTupType>
   requires (index == std::tuple_size_v<IncomingTupType>)
   constexpr PrecisionType CancelUnitsImpl(PrecisionType value)
   {
      return value;
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
   constexpr PrecisionType CancelUnitsImpl(PrecisionType value)
   {
      using currentType = std::tuple_element_t<index, IncomingTupType>;
      if constexpr (has_same_dim<currentType, RealTupType>::value)
      {
         const PrecisionType valueConverted = Convert<currentType, typename get_first_match<is_same_dim, currentType, RealTupType>::type, inverse>(value);
         return CancelUnitsImpl<inverse, index + 1, typename RemoveOneInstance<is_same_dim, currentType, RealTupType>::type, IncomingTupType>(valueConverted);
      }
      else
      {
         using primary = type_from_quantity_or_delta_t<currentType>::Primary;
         using realPrimary = std::conditional_t<is_quantity_v<currentType>, Quantity<primary>, primary>;

         const PrecisionType valueConverted = Convert<currentType, realPrimary, inverse>(value);
         return CancelUnitsImpl<inverse, index + 1, RealTupType, IncomingTupType>(valueConverted);
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
   constexpr PrecisionType CancelUnits(PrecisionType value)
   {
      const PrecisionType valueConverted1 = CancelUnitsImpl<false, 0, RealNumTupType, NumTupType>(value);
      const PrecisionType valueConverted2 = CancelUnitsImpl<true, 0, RealDenTupType, DenTupType>(valueConverted1);

      return valueConverted2;
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