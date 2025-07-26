#ifndef DIMENSION_UNIT_SIMPLIFIER_H
#define DIMENSION_UNIT_SIMPLIFIER_H

#include <concepts>

#include "TemplateUtils/RatioUtils.h"
#include "TemplateUtils/GenericUtils.h"

#include "base_dimension_signature.h"
#include "TupleHandling.h"

#include "FundamentalUnitExtractor.h"

namespace dimension
{

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
      using unit = unit_exponent<IncomingUnit, combined_exponent::num, combined_exponent::den>;
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
           unit_exponent<TargetUnit, Unit::exponent::num, Unit::exponent::den>,
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
   // ================ base_dimension From Tuple ==================
   // ============================================================
   template<typename... T>
   struct base_dimensionFromTuple;

   template<typename Rep, typename Ratio, typename... Units, typename... Coeffs>
   struct base_dimensionFromTuple<Rep, Ratio, std::tuple<Units...>, std::tuple<Coeffs...>>
   {
      using dim = base_dimension<Rep, Units..., Coeffs..., Ratio>;
   };

   template<typename Rep, typename... Units, typename... Coeffs>
   struct base_dimensionFromTuple<Rep, std::tuple<Units...>, std::tuple<Coeffs...>>
   {
      using dim = base_dimension<Rep, Units..., Coeffs...>;
   };

   template<typename... Units>
   struct base_dimensionFromTuple<std::tuple<Units...>>
   {
      using dim = base_dimension<Units...>;
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
      using dimType = typename base_dimensionFromTuple<final_units>::dim;
   };

   template<typename Dim>
   constexpr auto FullSimplify(Dim input) {
      using simplify_type = FullSimplifyType<typename Dim::units>;
      using result_type   = typename simplify_type::dimType;

      return result_type{simplify_type::after_conversion::Convert(input.template get_tuple_scalar<typename Dim::units>())};
   }

   // ============================================================
   // ==================== Equivalent Units ======================
   // ============================================================

   template<typename A, typename B>
   constexpr bool unit_exponent_equiv_v =
      std::is_same_v<typename A::unit, typename B::unit> &&
      std::ratio_equal_v<typename A::exponent, typename B::exponent>;

   template<typename TargetUnit, typename Tuple>
   struct contains_unit_exponent;

   template<typename TargetUnit>
   struct contains_unit_exponent<TargetUnit, std::tuple<>> : std::false_type {};

   template<typename TargetUnit, typename Head, typename... Tail>
   struct contains_unit_exponent<TargetUnit, std::tuple<Head, Tail...>> : std::conditional_t<
      unit_exponent_equiv_v<TargetUnit, Head>,
      std::true_type,
      contains_unit_exponent<TargetUnit, std::tuple<Tail...>>
   > {};

   template<typename A, typename B>
   struct all_in;

   template<typename... As, typename B>
   struct all_in<std::tuple<As...>, B> : std::conjunction<
      contains_unit_exponent<As, B>...
   > {};

   template<typename A, typename B>
   struct same_units_trait
   : std::conjunction<
         all_in<A, B>,
         all_in<B, A>
      > {};

   template<typename T1, typename T2>
   concept same_units = same_units_trait<
      typename InitialSimplifier<T1>::units,
      typename InitialSimplifier<T2>::units
   >::value;

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
   struct Subtractunit_exponents;

   // Empty base case
   template<>
   struct Subtractunit_exponents<std::tuple<>, std::tuple<>> {
      using type = std::tuple<>;
   };

   // General case: A and B are std::tuple<unit_exponent<...>...>
   template<typename... UnitsA, typename... UnitsB>
   struct Subtractunit_exponents<std::tuple<UnitsA...>, std::tuple<UnitsB...>> {
   private:
      template<typename UnitA>
      struct subtract_one {
         using matching = find_unit_by_dimension<UnitA, std::tuple<UnitsB...>>;
         
         using result = std::conditional_t<
               matching::found,
               unit_exponent<
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

   template<typename... Units>
   struct ConvertSimplified;

   template<typename... ToUnits>
   struct ConvertSimplified<std::tuple<>, std::tuple<ToUnits...>>
   {
       static constexpr double scalar = 1.0;
   };

   struct Coulombs;
   struct meters;

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

       using RawFrom = typename base_dimensionFromTuple<FromTuple>::dim;
       using RawTo = typename base_dimensionFromTuple<ToTuple>::dim;

       using SimplifiedFrom = typename InitialSimplifier<FromTuple>::units;
       using SimplifiedTo = typename InitialSimplifier<ToTuple>::units;
   
       using FromRemainingRaw = typename Subtractunit_exponents<SimplifiedFrom, SimplifiedTo>::type;
       using ToRemainingRaw = typename Subtractunit_exponents<SimplifiedTo, SimplifiedFrom>::type;
   
       using FromRemaining = typename RemoveZeros<FromRemainingRaw>::units;
       using ToRemaining = typename RemoveZeros<ToRemainingRaw>::units;

      static constexpr double Convert(double value) 
      {

         using FromFullySimplified = decltype(FullSimplify(RawFrom(1.0)));
         using ToFullySimplified = decltype(FullSimplify(RawTo(1.0)));

         FromFullySimplified fullSimplified = FullSimplify(RawFrom(value));
         constexpr double inverse_scalar = 1.0 / (FullSimplify(RawTo(1.0)).template get_tuple_scalar<typename ToFullySimplified::units>());

         using converter = ConvertSimplified<typename FromFullySimplified::units, typename ToFullySimplified::units>;

         return fullSimplified.template get_tuple_scalar<typename FromFullySimplified::units>() * inverse_scalar * converter::scalar;
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
           std::tuple<unit_exponent<typename Unit::unit, -Unit::exponent::num, Unit::exponent::den>>,
           typename FlipExponents<std::tuple<Rest...>>::units
       >;
   };

   template<is_unit_exponent T, is_unit_exponent U>
   struct Convertibleunit_exponent
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
         Convertibleunit_exponent<Unit, First>::value ||
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

} // end Dimension

#endif // DIMENSION_UNIT_SIMPLIFIER_H