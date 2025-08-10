#ifndef DIMENSIONAL_DETAIL_SYMBOL_H
#define DIMENSIONAL_DETAIL_SYMBOL_H

#include <ratio>
#include <tuple>
#include <type_traits>

#include "../../ratio/ratio_utils.h"
#include "../../exponentiation/exponentiation.h"
#include "symbol_core.h"

namespace dimension::detail::symbol_utils_impl
{
   template<is_symbol T, int Num, int Den>
   constexpr double symbol_value()
   {
      return pow_rational/*<Num, Den>*/(T::value, Num, Den);
   }

   template<is_symbol_exponent T>
   constexpr double symbol_exponent_value()
   {
      return symbol_value<typename T::symbol, T::exponent::num, T::exponent::den>();
   }

   template<typename... Ts>
   constexpr double multiply_symbol_exponent_values_impl(std::tuple<Ts...>)
   {
      return (1.0 * ... * symbol_exponent_value<Ts>());
   }

   template<typename Tuple>
   struct multiply_symbol_exponent_values
   {
      static constexpr double value = multiply_symbol_exponent_values_impl(Tuple{});
   };

   template<typename Tuple, typename SE>
   struct add_or_merge_symbol;

   template<typename SE>
   struct add_or_merge_symbol<std::tuple<>, SE> {
      using exp = typename SE::exponent;
      using type = std::conditional_t<
         std::ratio_equal_v<exp, std::ratio<0>>,
         std::tuple<>,
         std::tuple<SE>>;
   };

   template<typename Head, typename... Tail, typename SE>
   struct add_or_merge_symbol<std::tuple<Head, Tail...>, SE> {
   private:
      static constexpr bool match = std::is_same_v<typename Head::symbol, typename SE::symbol>;

      using merged_exponent = typename std::ratio_add<typename Head::exponent, typename SE::exponent>::type;
      using merged = symbol_exponent<typename SE::symbol, merged_exponent::num, merged_exponent::den>;

      using rest = std::tuple<Tail...>;

      using next = typename add_or_merge_symbol<rest, SE>::type;

   public:
      using type = std::conditional_t<
         match,
         std::conditional_t<
               std::ratio_equal_v<merged_exponent, std::ratio<0>>,
               rest,  // omit entirely
               std::tuple<merged, Tail...>  // replace head
         >,
         decltype(std::tuple_cat(std::tuple<Head>{}, next{}))
      >;
   };

   template<is_symbol_exponent T>
   struct inverse_symbol_exponent
   {
      using type = symbol_exponent<typename T::symbol, -T::exponent::num, T::exponent::den>;
   };

   template<typename Tuple>
   struct collapse_symbol_exponents;

   template<>
   struct collapse_symbol_exponents<std::tuple<>> {
      using type = std::tuple<>;
   };

   template<typename Head, typename... Tail>
   struct collapse_symbol_exponents<std::tuple<Head, Tail...>> {
   private:
      using rest_collapsed = typename collapse_symbol_exponents<std::tuple<Tail...>>::type;
   public:
      using type = typename add_or_merge_symbol<rest_collapsed, Head>::type;
   };

   // ── detail: normalize any input to either a symbol_exponent or void ──
   template<typename T>
   struct to_symbol_exponent {
      using type = void;
   };

   template<is_symbol_exponent T>
   struct to_symbol_exponent<T> {
      using type = T;
   };

   template<is_symbol T>
   struct to_symbol_exponent<T> {
      using type = symbol_exponent<T, 1, 1>;
   };

   // ── tuple filter/map helper ──────────────────────────────────────────
   template<typename... Ts>
   struct collect_symbol_exponents {
   public:
      using type = decltype(std::tuple_cat(
         std::conditional_t<std::is_void_v<typename to_symbol_exponent<Ts>::type>,
                              std::tuple<>, // skip
                              std::tuple<typename to_symbol_exponent<Ts>::type>>{}...
      ));
   };

   // ============================================================================
   //  multiply_symbol_tuples<tuple<A...>, tuple<B...>>::type
   // ============================================================================
   template<typename... Ts>
   struct multiply_symbol_tuples;

   template<typename... T1s, typename... T2s>
   struct multiply_symbol_tuples<std::tuple<T1s...>, std::tuple<T2s...>>
   {
      using type = typename collapse_symbol_exponents<std::tuple<T1s..., T2s...>>::type;
   };

   // ============================================================================
   //  divide_symbol_tuples<tuple<A...>, tuple<B...>>::type
   //     (equivalent to multiply A by B⁻¹)
   // ============================================================================
   template<typename... Ts>
   struct divide_symbol_tuples;

   template<typename... T1s, typename... T2s>
   struct divide_symbol_tuples<std::tuple<T1s...>, std::tuple<T2s...>>
   {
   private:
      using negated_pack = std::tuple<typename inverse_symbol_exponent<T2s>::type...>;

   public:
      using type = typename multiply_symbol_tuples<std::tuple<T1s...>, negated_pack>::type;
   };


   template<is_symbol_exponent T, is_ratio R>
   struct raise_symbol_exponent
   {
   private:
      using new_exponent = std::ratio_multiply<typename T::exponent, R>;
   public:
      using type = symbol_exponent<typename T::symbol, new_exponent::num, new_exponent::den>;
   };

   template<typename Tuple, is_ratio R>
   struct raise_all_symbol_exponents;  // primary

   template<typename... Ts, is_ratio R>
   struct raise_all_symbol_exponents<std::tuple<Ts...>, R>
   {
       using type = std::tuple<typename raise_symbol_exponent<Ts, R>::type...>;
   };




}

#endif // DIMENSIONAL_DETAIL_SYMBOL_H