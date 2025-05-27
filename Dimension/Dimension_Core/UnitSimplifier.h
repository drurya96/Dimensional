#ifndef DIMENSION_UNIT_SIMPLIFIER_H
#define DIMENSION_UNIT_SIMPLIFIER_H

#include "TupleHandling.h"
#include "Conversion.h"
#include "FundamentalUnitExtractor.h"
#include "UnitValidation.h"
#include "TemplateUtils/RatioUtils.h"
#include "TemplateUtils/GenericUtils.h"
#include "Conversion.h"

namespace Dimension
{


   template<are_unit_exponents... Units>
   class BaseDimension;


   // TODO: Maybe not needed? Keep until verified
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

      using added = UnitExponent<typename Unit::unit, std::ratio_add<typename Unit::exponent, typename IncomingUnit::exponent>::num, std::ratio_add<typename Unit::exponent, typename IncomingUnit::exponent>::den>;

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

   // ============================================================
   // ===================== Combine Power ========================
   // ============================================================

   template<typename IncomingUnit, typename... Units>
   struct CombinePower;

   template<typename IncomingUnit, typename... Units>
   struct CombinePower<IncomingUnit, std::tuple<Units...>>
   {
   private:
      // Fold over matching unit exponents
      using combined_exponent = ratio_add_fold_t<
        std::conditional_t<
            std::is_same_v<IncomingUnit, typename Units::unit>,
            typename Units::exponent,
            std::ratio<0>
        >...
    >;

   public:
      using unit = UnitExponent<IncomingUnit, combined_exponent::num, combined_exponent::den>;
   };

   // ============================================================
   // ====================== Unit Present ========================
   // ============================================================

   template<typename IncomingUnit, typename Tuple>
   struct UnitPresent;

   template<typename IncomingUnit, typename... Units>
   struct UnitPresent<IncomingUnit, std::tuple<Units...>>
      : std::bool_constant<(... || std::is_same_v<IncomingUnit, typename Units::unit>)> {};

   // ============================================================
   // ===================== Remove Zeroes ========================
   // ============================================================

   template<typename E>
   struct is_nonzero_unit : std::bool_constant<E::exponent::num != 0> {};

   template<typename Tuple>
   struct RemoveZeros
   {
       using units = typename filter_tuple<Tuple, is_nonzero_unit>::type;
   };

   // ============================================================
   // ================== Initial Simplifier ======================
   // ============================================================

   template<typename InputTuple>
   struct InitialSimplifier;
   
   template<typename... Units>
   struct InitialSimplifier<std::tuple<Units...>>
   {
   private:
       using flattened = typename FundamentalUnitExtractor<std::tuple<Units...>>::units;
      
      template<typename FullTuple>
      struct CombineUniqueFrom {
          template<typename Accum, typename Unit>
          struct apply {      
              using type = std::conditional_t<
                  UnitPresent<typename Unit::unit, Accum>::value,
                  Accum,
                  tuple_cat_t<
                      Accum,
                      std::tuple<typename CombinePower<typename Unit::unit, FullTuple>::unit>
                  >
              >;
          };
      };

       using combined = fold_over_tuple_with_state<flattened,std::tuple<>, CombineUniqueFrom<flattened>::template apply>::type;
      
   public:
       using units = typename RemoveZeros<combined>::units;
   };

   





























   // ============================================================
   // =================== Conversion Stuff =======================
   // ============================================================

   namespace Math
   {
      template<int Power>
      constexpr double PowInt(double value)
      {
         if constexpr (Power % 2 == 0)
         {
            double half = PowInt<Power / 2>(value);
            return half * half;
         }
         else
         {
            return value * PowInt<Power - 1>(value);
         }
      }

      template<int Power>
      requires (Power < 0)
      constexpr double PowInt(double value)
      {
         return 1.0 / PowInt<-Power>(value);
      }

      template<>
      constexpr double PowInt<0>(double)
      {
         return 1.0;
      }

      template<>
      constexpr double PowInt<1>(double value)
      {
         return value;
      }

      template<>
      constexpr double PowInt<2>(double value)
      {
         return value * value;
      }

      template<>
      constexpr double PowInt<3>(double value)
      {
         return value * value * value;
      }

      template<>
      constexpr double PowInt<4>(double value)
      {
         return value * value * value * value;
      }

      template<>
      constexpr double PowInt<5>(double value)
      {
         return value * value * value * value * value;
      }



      template<int Root>
      constexpr double RootInt(double value)
      {
         static_assert(Root != 0, "RootInt<0> is undefined.");

         return std::pow(value, 1.0 / static_cast<double>(Root));
      }

      template<>
      inline double RootInt<1>(double value)
      {
         return value;
      }

      template<>
      inline double RootInt<2>(double value)
      {
         return std::sqrt(value);
      }

      template<>
      inline double RootInt<3>(double value)
      {
         return std::cbrt(value);
      }

      template<>
      inline double RootInt<4>(double value)
      {
         return std::sqrt(std::sqrt(value));
      }

   }








   template<typename StartingUnit, typename TargetUnit>
   constexpr double GetSlope()
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
         return GetSlope<StartingUnit, Primary>() * GetSlope<Primary, TargetUnit>();
         //return Conversion<StartingUnit, Primary>::slope * Conversion<Primary, TargetUnit>::slope;
      }
   }

   template<typename TargetUnit, typename Unit>
   constexpr double DoConversion(double value)
   {
      // Assumptions for now:
      // Only called on deltas (deal with this once Quantities are working)
      constexpr double scale = GetSlope<typename Unit::unit, TargetUnit>();

      if constexpr (Unit::exponent::den == 1)
      {
         return value * Math::PowInt<Unit::exponent::num>(scale);
      }
      else
      {
         return value * Math::RootInt<Unit::exponent::den>(Math::PowInt<Unit::exponent::num>(scale));
      }
   }



































   // ============================================================
   // =================== Convert All Units ======================
   // ============================================================

   template<typename TargetUnit, typename Tuple>
   struct Convert_All_Units;
   
   template<typename TargetUnit>
   struct Convert_All_Units<TargetUnit, std::tuple<>> {
       using units = std::tuple<>;
   
       static constexpr double Convert(double val) {
           return val;
       }
   };
   
   template<typename TargetUnit, typename Unit, typename... Rest>
   struct Convert_All_Units<TargetUnit, std::tuple<Unit, Rest...>> {
   private:
       static constexpr bool same_dim = is_same_dim<typename Unit::unit, TargetUnit>::value;
   
       using converted_unit = std::conditional_t<
           same_dim,
           UnitExponent<TargetUnit, Unit::exponent::num, Unit::exponent::den>,
           Unit
       >;
   
       using tail_result = Convert_All_Units<TargetUnit, std::tuple<Rest...>>;
   
   public:
       using units = tuple_cat_t<
           std::tuple<converted_unit>,
           typename tail_result::units
       >;
   
       static constexpr double Convert(double val) {
           if constexpr (same_dim) {
               val = DoConversion<TargetUnit, Unit>(val);
           }
           return tail_result::Convert(val);
       }
   };

   // ============================================================
   // ==================== Convert All Dims ======================
   // ============================================================

   template<typename Tuple>
   struct Convert_All_Dims;
   
   template<>
   struct Convert_All_Dims<std::tuple<>> {
       using units = std::tuple<>;
   
       static constexpr double Convert(double val) {
           return val;
       }
   };
   
   template<typename Unit, typename... Units>
   struct Convert_All_Dims<std::tuple<Unit, Units...>> {
   private:
       using tail = std::tuple<Units...>;
   
       // Convert all subsequent units to match Unit's unit
       using converted = Convert_All_Units<typename Unit::unit, tail>;
   
       // Recurse on the updated tail
       using next = Convert_All_Dims<typename converted::units>;
   
   public:
       using units = tuple_cat_t<
           std::tuple<Unit>,
           typename next::units
       >;
   
       static constexpr double Convert(double val) {
           double after_conversion = converted::Convert(val);
           return next::Convert(after_conversion);
       }
   };
   
   // ============================================================
   // ================ BaseDimension From Tuple ==================
   // ============================================================
   template<typename... T>
   struct BaseDimensionFromTuple;

   template<typename... T>
   struct BaseDimensionFromTuple<std::tuple<T...>>
   {
      using dim = typename BaseDimension<T...>;
   };



   // ============================================================
   // ==================== Full Simplifier =======================
   // ============================================================

   template<typename... Units>
   struct FullSimplifyType;

   template<typename... Units>
   struct FullSimplifyType<std::tuple<Units...>>
   {
      using initial_simplified = typename InitialSimplifier<std::tuple<Units...>>::units;
      using after_conversion = Convert_All_Dims<initial_simplified>;
      using final_units = typename InitialSimplifier<typename after_conversion::units>::units;
      using dimType = typename BaseDimensionFromTuple<final_units>::dim;
   };

   template<typename Dim>
   constexpr auto FullSimplify(Dim input) {
      using simplify_type = FullSimplifyType<typename Dim::units>;
      return simplify_type::dimType(simplify_type::after_conversion::Convert(input.GetRaw()));
   }













   // ============================================================
   // ============== Find Matching Unit by Dimension =============
   // ============================================================

   // THIS IS NAMED VERY WRONG AND IS CONFUSING
   // THIS IS REALLY FINDING MATCHING UNITS, NOT DIMENSIONS

   template<typename Target, typename Tuple>
   struct find_unit_by_dimension;

   // Empty case
   template<typename Target>
   struct find_unit_by_dimension<Target, std::tuple<>> {
      static constexpr bool found = false;

      struct Dummy
      {
         using exponent = std::ratio<0>;
      };
      using type = Dummy;
   };

   // Recursive case
   template<typename Target, typename Head, typename... Tail>
   struct find_unit_by_dimension<Target, std::tuple<Head, Tail...>> {
   private:
      static constexpr bool is_match = std::is_same_v<typename Target::unit, typename Head::unit>;

   public:
      static constexpr bool found = is_match || find_unit_by_dimension<Target, std::tuple<Tail...>>::found;
      using type = std::conditional_t<is_match, Head, typename find_unit_by_dimension<Target, std::tuple<Tail...>>::type>;
   };






   // ============================================================
   // ==================== Subtract Tuples =======================
   // ============================================================

   template<typename TupleA, typename TupleB>
   struct SubtractUnitExponents;

   // Empty base case
   template<>
   struct SubtractUnitExponents<std::tuple<>, std::tuple<>> {
      using type = std::tuple<>;
   };

   // General case: A and B are std::tuple<UnitExponent<...>...>
   template<typename... UnitsA, typename... UnitsB>
   struct SubtractUnitExponents<std::tuple<UnitsA...>, std::tuple<UnitsB...>> {
   private:
      template<typename UnitA>
      struct subtract_one {
         using matching = find_unit_by_dimension<UnitA, std::tuple<UnitsB...>>;
         
         using result = std::conditional_t<
               matching::found,
               UnitExponent<
                  typename UnitA::unit,
                  std::ratio_subtract<
                     typename UnitA::exponent,
                     typename matching::type::exponent
                  >::num,
                  std::ratio_subtract<
                     typename UnitA::exponent,
                     typename matching::type::exponent
                  >::den
               >,
               UnitA
         >;
      };

   public:
      using type = tuple_cat_t<
         std::tuple<typename subtract_one<UnitsA>::result>...
      >;
   };












   template<typename From, typename ToTuple>
   struct MatchUnit;
   

   template<typename From>
   struct MatchUnit<From, std::tuple<>>
   {
      using type = void; // Should never occur
   };

   template<typename From, typename Head, typename... Tail>
   struct MatchUnit<From, std::tuple<Head, Tail...>>
   {
       //using unit_type = typename From::unit;
   
       static constexpr bool match = is_same_dim<typename From::unit, typename Head::unit>::value;

       using type = std::conditional_t<
           match,
           Head,
           typename MatchUnit<From, std::tuple<Tail...>>::type
       >;
   };









/*
   // Conversion assuming FromTuple and ToTuple are fully simplified
   template<typename... Units>
   struct ConvertSimplified;

   template<>
   struct ConvertSimplified<std::tuple<>, std::tuple<>>
   {
      static constexpr double scalar = 1.0;
   };

   template<typename FromUnit, typename ToUnit>
   struct ConvertSimplified<std::tuple<FromUnit>, std::tuple<ToUnit>>
   {
      // Figure out how to make constexpr
      static const inline double scalar = DoConversion<ToUnit::unit, FromUnit>(1.0);
   };

   template<typename FromUnit, typename... FromUnits, typename ToUnit, typename... ToUnits>
   struct ConvertSimplified<std::tuple<FromUnit, FromUnits...>, std::tuple<ToUnit, ToUnits...>>
   {
      // Figure out how to make constexpr
      static const inline double scalar = ConvertSimplified<std::tuple<FromUnit>, std::tuple<ToUnit>>::scalar * ConvertSimplified<std::tuple<FromUnits...>, std::tuple<ToUnits...>>::scalar;
   };
*/

   template<typename... Units>
   struct ConvertSimplified;

   template<typename... ToUnits>
   struct ConvertSimplified<std::tuple<>, std::tuple<ToUnits...>>
   {
       static constexpr double scalar = 1.0;
   };

   struct Coulombs;
   struct Meters;

   template<typename FromUnit, typename... FromRest, typename... ToUnits>
   struct ConvertSimplified<std::tuple<FromUnit, FromRest...>, std::tuple<ToUnits...>>
   {
      using ToMatch = typename MatchUnit<FromUnit, std::tuple<ToUnits...>>::type;

      static constexpr double scalar =
         DoConversion<typename ToMatch::unit, FromUnit>(1.0) *
         ConvertSimplified<std::tuple<FromRest...>, std::tuple<ToUnits...>>::scalar;
   };

   // ============================================================
   // ======================= ConvertDim =========================
   // ============================================================

   template<typename FromTuple, typename ToTuple>
   struct ConvertDim
   {

       using SimplifiedFrom = typename InitialSimplifier<FromTuple>::units;
       using SimplifiedTo = typename InitialSimplifier<ToTuple>::units;
   
       using FromRemainingRaw = typename SubtractUnitExponents<SimplifiedFrom, SimplifiedTo>::type;
       using ToRemainingRaw = typename SubtractUnitExponents<SimplifiedTo, SimplifiedFrom>::type;
   
       using FromRemaining = typename RemoveZeros<FromRemainingRaw>::units;
       using ToRemaining = typename RemoveZeros<ToRemainingRaw>::units;

      static constexpr double Convert(double value) 
      {

         using FromFullySimplified = decltype(FullSimplify(typename BaseDimensionFromTuple<FromTuple>::dim(1.0)));
         using ToFullySimplified = decltype(FullSimplify(typename BaseDimensionFromTuple<ToTuple>::dim(1.0)));

         FromFullySimplified fullSimplified = FullSimplify(typename BaseDimensionFromTuple<FromTuple>::dim(value));
         double inverse_scalar = 1.0 / (FullSimplify(typename BaseDimensionFromTuple<ToTuple>::dim(1.0)).GetRaw());

         using converter = ConvertSimplified<typename FromFullySimplified::units, typename ToFullySimplified::units>;

       return fullSimplified.GetRaw() * inverse_scalar * converter::scalar;
   }
   };
   
   






   template<typename... Units>
   struct FlipExponents;
   
   template<>
   struct FlipExponents<std::tuple<>> {
       using units = std::tuple<>;
   };
   
   template<typename Unit, typename... Rest>
   struct FlipExponents<std::tuple<Unit, Rest...>> {
       using units = tuple_cat_t<
           std::tuple<UnitExponent<typename Unit::unit, -Unit::exponent::num, Unit::exponent::den>>,
           typename FlipExponents<std::tuple<Rest...>>::units
       >;
   };







   //template<typename T>
   //concept base_dimension = requires {
   //    typename T::units; // Must have a 'units' type
   //};
   class base_dimension_marker;

   template<typename T>
   concept base_dimension = std::is_base_of_v<base_dimension_marker, T>;












   template<is_unit_exponent T, is_unit_exponent U>
   struct ConvertibleUnitExponent
   {
      static constexpr bool value =
         is_same_dim<typename T::unit, typename U::unit>::value &&
         std::ratio_equal<typename T::exponent, typename U::exponent>::value;
   };

   template<typename Unit, typename Tuple>
   struct ContainsEquivalentUnit;
   
   template<typename Unit, typename First, typename... Rest>
   struct ContainsEquivalentUnit<Unit, std::tuple<First, Rest...>> {
       static constexpr bool value =
         ConvertibleUnitExponent<Unit, First>::value ||
         ContainsEquivalentUnit<Unit, std::tuple<Rest...>>::value;
   };
   
   template<typename Unit>
   struct ContainsEquivalentUnit<Unit, std::tuple<>> {
       static constexpr bool value = false;
   };
   
   template<typename Tuple1, typename Tuple2>
   struct AreUnitTuplesEquivalent;
   
   template<typename... Units1, typename Tuple2>
   struct AreUnitTuplesEquivalent<std::tuple<Units1...>, Tuple2> {
       static constexpr bool value =
           (ContainsEquivalentUnit<Units1, Tuple2>::value && ...) &&
           (sizeof...(Units1) == std::tuple_size_v<Tuple2>);
   };



















   template<typename T, typename U>
   concept matching_dimensions =
       AreUnitTuplesEquivalent<
           typename decltype(FullSimplify(T{}))::units,
           typename decltype(FullSimplify(U{}))::units
       >::value;




























/*


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
   template<are_unit_exponents... Units>
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

*/

} // end Dimension

#endif // DIMENSION_UNIT_SIMPLIFIER_H