#ifndef DIMENSIONAL_FACTOR_H
#define DIMENSIONAL_FACTOR_H

#include "convertible.h"
#include "../exponentiation/exponentiation.h"
#include "../symbol/symbol_utils.h"
#include "../ratio/ratio_exponent.h"

namespace dimension {

   template<class T>
   struct is_symbol_exponent_pred
   : std::bool_constant< is_symbol_exponent_v<std::remove_cvref_t<T>> > {};

   template<class T>
   struct is_ratio_exponent_pred
   : std::bool_constant< is_ratio_exponent_v<std::remove_cvref_t<T>> > {};


   // TODO: This is not robust.. I need to formalize whether coefficients are passed variadic or as tuples, then be consistent.
   // Variadic would be much easier here, but at the call sites that might get complicated...
   // This becomes more straight forward if I collapse ratio into ratios, then I can make ratios a required tuple parameter, and the symbols
   //    as an optional secondary tuple, default to empty.
   //    But that only really makes sense if I collapse the leading ratio into a ratio_exponent...
   //    Long term that's probably best, but I'm in an unstable state right now.


   template<
      class Ratio,                        // must be std::ratio<...>
      class RatioExponentsTuple,          // must be std::tuple<ratio_exponent<...>, ...>
      class SymbolExponentsTuple = std::tuple<> // optional std::tuple<symbol_exponent<...>, ...>
   >
   struct factor_t {
      using ratio   = Ratio;
      using ratios  = RatioExponentsTuple;
      using symbols = SymbolExponentsTuple;
   };

   namespace factor
   {
      template<class Factor, typename Rep = double>
      constexpr Rep eval_factor()
      {
         // Ratio part
         constexpr long double num = static_cast<long double>(Factor::ratio::num);
         constexpr long double den = static_cast<long double>(Factor::ratio::den);
         constexpr long double base_ratio = num / den;

         // Symbol exponents part (multiply all symbols’ values^exponent)
         constexpr long double symbol_val = multiply_symbol_exponent_values_v<typename Factor::symbols>;

         // Ratio exponents part (multiply all ratio_exponents)
         constexpr long double ratio_exp_val = multiply_ratio_exponent_values_v<typename Factor::ratios>;

         return static_cast<Rep>(base_ratio * symbol_val * ratio_exp_val);
      }
   }

   template<typename Factor, is_ratio P>
   struct raise_factor;

   // Specialization for factor_t< BaseRatio, RatioExponentsTuple, SymbolExponentsTuple >
   template<class BaseRatio, class RatioTuple, class SymbolTuple, is_ratio P>
   struct raise_factor<factor_t<BaseRatio, RatioTuple, SymbolTuple>, P>
   {
   static_assert(P::den != 0, "Exponent denominator cannot be zero");

   using power      = P;
   constexpr static bool kInteger = (power::den == 1);

   // Pull grouped tuples
   using rate_tuple = RatioTuple;   // tuple<ratio_exponent<...>, ...>
   using sym_tuple  = SymbolTuple;  // tuple<symbol_exponent<...>, ...>

   // Scale existing exponents
   using rate_scaled = raise_all_ratio_exponents_t<rate_tuple, power>; // This seems to be a tuple
   using sym_scaled  = detail::raise_all_symbol_exponents_t<sym_tuple, power>;

   // Compute new base ratio
   using raised_base_ratio =
      std::conditional_t<
         kInteger,
         raise_ratio_t<BaseRatio, power::num>,
         std::ratio<1>
      >;

   // If non-integer exponent, carry base ratio into ratio-exponents
   using rate_with_base =
      std::conditional_t<
         kInteger,
         rate_scaled,
         append_t<rate_scaled, ratio_exponent<BaseRatio, power::num, power::den>>
      >;

   // Result
   using type = factor_t<raised_base_ratio, rate_with_base, sym_scaled>; // rate_with_base should be a tuple, but suspect its just a ratio_exponent. Maybe same with sym_scaled. Need to check
   };




   template<class F, class P>
   using raise_factor_t = typename raise_factor<F, P>::type;

   template<typename F1, typename F2>
   struct multiply_factors;

   // factor_t< Ratio, RatioExponentsTuple, SymbolExponentsTuple >
   template<class R1, class RE1, class SE1, class R2, class RE2, class SE2>
   struct multiply_factors<factor_t<R1, RE1, SE1>, factor_t<R2, RE2, SE2>> {
   private:
   using F1 = factor_t<R1, RE1, SE1>;
   using F2 = factor_t<R2, RE2, SE2>;

   // grouped tuples from factors
   using s1 = typename F1::symbols;
   using s2 = typename F2::symbols;
   using r1 = typename F1::ratios;
   using r2 = typename F2::ratios;

   // combine tuples
   using sC = typename detail::symbol_utils_impl::multiply_symbol_tuples<s1, s2>::type;
   using rC = typename detail::ratio_utils_impl::multiply_ratio_exponent_tuples<r1, r2>::type; // if this is a template<...><...>::type, keep as such

   // base ratio product
   using Rprod = std::ratio_multiply<R1, R2>;

   public:
   using type = factor_t<Rprod, rC, sC>;
   };




   template<typename F1, typename F2>
   using multiply_factors_t = typename multiply_factors<F1, F2>::type;

   template<typename F1, typename F2>
   struct divide_factors;

   template<typename R1, typename... SE1, typename R2, typename... SE2>
   struct divide_factors<factor_t<R1, SE1...>, factor_t<R2, SE2...>> {
   private:
   using F1 = factor_t<R1, SE1...>;
   using F2 = factor_t<R2, SE2...>;

   using s1 = typename F1::symbols;
   using s2 = typename F2::symbols;
   using r1 = typename F1::ratios;
   using r2 = typename F2::ratios;

   using sC = typename detail::symbol_utils_impl::divide_symbol_tuples<s1, s2>::type;
   using rC = typename detail::ratio_utils_impl::divide_ratio_exponent_tuples<r1, r2>::type;

   using Rquot = std::ratio_divide<R1, R2>;

   using merged = tuple_cat_t<sC, rC>;

   struct builder {
      template<class... Es>
      auto operator()() -> factor_t<Rquot, Es...>;
   };

   public:
   using type = decltype(call_unpack<merged>(builder{}));
   };

   template<typename F1, typename F2>
   using divide_factors_t = typename divide_factors<F1, F2>::type;


   template<typename Factor>
   struct reduce_factor;

   // factor_t< Ratio, RatioExponentsTuple, SymbolExponentsTuple >
   template<class R, class RatioTuple, class SymbolTuple>
   struct reduce_factor<factor_t<R, RatioTuple, SymbolTuple>>
   {
      using F = factor_t<R, RatioTuple, SymbolTuple>;

      using sym_collapsed  = detail::collapse_symbol_exponents_t<typename F::symbols>; // -> std::tuple<symbol_exponent<...>, ...>
      using rate_collapsed = collapse_ratio_exponents_t<typename F::ratios>;           // -> std::tuple<ratio_exponent<...>,  ...>

      using type = factor_t<R, rate_collapsed, sym_collapsed>;
   };

   // Variadic multiply: fold (left-assoc) over multiply_factors_t
   template<class... Fs>
   struct multiply_factors_n;

   template<>
   struct multiply_factors_n<> {
   using type = factor_t<std::ratio<1,1>, std::tuple<>>; // identity
   };

   template<class F>
   struct multiply_factors_n<F> {
   using type = F;
   };

   template<class F1, class F2, class... Rest>
   struct multiply_factors_n<F1, F2, Rest...> {
   private:
   using partial = multiply_factors_t<F1, F2>;
   public:
   using type = typename multiply_factors_n<partial, Rest...>::type;
   };

   template<class... Fs>
   using multiply_factors_n_t = typename multiply_factors_n<Fs...>::type;

   // Optional: multiply all then reduce once
   template<class... Fs>
   using multiply_and_reduce_n_t =
   typename reduce_factor<multiply_factors_n_t<Fs...>>::type;




   // Identity factor
   using factor_identity = factor_t<std::ratio<1,1>, std::tuple<>>;

   // Fold adapter: State × NextFactor -> multiply_factors_t<State, NextFactor>
   template<typename State, typename Next>
   struct tuple_multiply_step {
   using type = multiply_factors_t<State, Next>;
   };

   // Multiply all factors in a std::tuple<...>
   template<class Tuple>
   struct multiply_factors_tuple;

   template<class... Fs>
   struct multiply_factors_tuple<std::tuple<Fs...>> {
   using type = fold_over_tuple_with_state_t<
         std::tuple<Fs...>,            // input tuple
         factor_identity,              // initial state
         tuple_multiply_step           // step (defined above)
      >;
   };

   template<class Tuple>
   using multiply_factors_tuple_t = typename multiply_factors_tuple<Tuple>::type;

   // Multiply all factors in a tuple, then reduce once
   template<class Tuple>
   struct multiply_and_reduce_tuple {
      using mult = multiply_factors_tuple_t<Tuple>;
      using type = typename reduce_factor<mult>::type;
   };

   template<class Tuple>
   using multiply_and_reduce_tuple_t = typename multiply_and_reduce_tuple<Tuple>::type;
}

#endif // DIMENSIONAL_FACTOR_H