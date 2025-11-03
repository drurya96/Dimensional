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

      template<typename S, typename T, typename = void>
      struct get_factor_type;  // primary

      // 1) Identical units → 1:1
      template<typename S>
      struct get_factor_type<S, S, void> {
      using type = factor_t<std::ratio<1>>;
      };

      // 2) Forward conversion available
      template<typename S, typename T>
      requires HasConversion<S, T>
      struct get_factor_type<S, T, void> {
         using type = Conversion<S, T>::scale;
      };

      // 3) Only reverse conversion available
      template<typename S, typename T>
      requires (!HasConversion<S, T> && HasConversion<T, S>)
      struct get_factor_type<S, T, void> {
      private:
         using ratio = inverse_ratio_t<typename Conversion<T, S>::scale::ratio>;
         using coefficients = tuple_invert_exponents_t<typename Conversion<T, S>::scale::symbols>;
         // Notes to self as I go...
         //   coefficients may hold symbol_exponents or ratio_exponents
         //   I should generalize this to arbitrary coefficients
         //   Should I consider whether the ratio itself should be a coefficient? Such that the ratio is just a ratio_exponent<ratio, 1>?
         //   That might simplify things a bit... but that would be a very big change.
         //   It would probably be easier to just have two hard-coded paths: symbol_exponent and ratio_exponent for coefficients
         //   Once I have that working, I can create an issue to revisit that.

         // I have it using symbol_exponent, but ratio_exponent still needs some work
         // I think I need to have symbol_exponent and ratio_exponent (and maybe unit_exponent?) inherit from a common class

      public:
         using type = factor_t<ratio, coefficients>;
      };

      // 4) Neither direction available → go through primary
      template<typename S, typename T>
      requires (!HasConversion<S, T> && !HasConversion<T, S>)
      struct get_factor_type<S, T, void> {
      private:
         using ratio = std::ratio_multiply<
            typename get_factor_type<S, unit_primary_t<S>>::type::ratio,
            typename get_factor_type<unit_primary_t<S>, T>::type::ratio
         >;
         using coefficients = detail::multiply_symbol_tuples_t<
            typename get_factor_type<S, unit_primary_t<S>>::type::symbols,
            typename get_factor_type<unit_primary_t<S>, T>::type::symbols
         >;
      public:
         using type = factor_t<ratio, coefficients>;
      };
   }

   // TODO (see implementation comments above): This does not yet handle ratio_exponent as coefficients
   template<typename StartingUnit, typename TargetUnit>
   using get_factor_t = typename factor::get_factor_type<StartingUnit, TargetUnit>::type;


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