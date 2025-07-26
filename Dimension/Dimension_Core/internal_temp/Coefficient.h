#ifndef DIMENSIONAL_COEFFICIENT_H
#define DIMENSIONAL_COEFFICIENT_H

#include <concepts>
#include <ratio>
#include <type_traits>

#include "base_dimension_signature.h"

namespace dimension
{

   template<typename... Rs> struct combine_ratios;

   template<typename R>
   struct combine_ratios<R>
   {   using type = R; };

   template<typename R1, typename R2, typename... Rest>
   struct combine_ratios<R1, R2, Rest...>
   {
      using type =
         typename combine_ratios<
               std::ratio_multiply<R1, R2>,
               Rest...>::type;
   };

   template<>
   struct combine_ratios<>
   {   using type = std::ratio<1>; };

   // ── primary template: default “prepend new symbol” ───────────────
   template<typename Tuple, typename Sym, typename Exp>
   struct add_symbol;

   template<typename Sym, typename Exp>
   struct add_symbol<std::tuple<>, Sym, Exp>
   {
      using type = std::conditional_t<
         std::ratio_equal_v<Exp, std::ratio<0>>,
         std::tuple<>,                                           // nothing to add
         std::tuple< symbol_exponent<Sym, Exp::num, Exp::den>>>;
   };

   // ------------- helper: merge new exponent into existing head -----------------
   template<typename Sym, int N0, int D0, typename Exp>
   struct _merge_symbol_head
   {
   private:
      using combined = std::ratio_add<std::ratio<N0, D0>, Exp>;

   public:
      using type = std::conditional_t<
         std::ratio_equal_v<combined, std::ratio<0>>,
         std::tuple<>,                                           // drop if zero
         std::tuple< symbol_exponent<Sym,
                        combined::num, combined::den>>>;
   };

   // ------------- 2)  head is symbol_exponent<Sym, N0, D0> ----------------------
   template<int N0, int D0, typename... Tail,
            typename Sym, typename Exp>
   struct add_symbol<std::tuple<symbol_exponent<Sym, N0, D0>, Tail...>,
                     Sym, Exp>
   {
      using type = decltype(
         std::tuple_cat(
               std::declval<typename _merge_symbol_head<Sym, N0, D0, Exp>::type>(),
               std::declval<std::tuple<Tail...>>() ) );
   };

   // ------------- 2') head is plain Sym -----------------------------------------
   template<typename Sym, typename... Tail, typename Exp>
      requires is_symbol<Sym>
   struct add_symbol<std::tuple<Sym, Tail...>, Sym, Exp>
   {
      using type = decltype(
         std::tuple_cat(
               std::declval<typename _merge_symbol_head<Sym, 1, 1, Exp>::type>(),
               std::declval<std::tuple<Tail...>>() ) );
   };

   // ------------- 3)  head does **not** match -----------------------------------
   template<typename Head, typename... Tail,
            typename Sym, typename Exp>
   struct add_symbol<std::tuple<Head, Tail...>, Sym, Exp>
   {
      using type = decltype(
         std::tuple_cat(
               std::declval<std::tuple<Head>>(),
               std::declval<typename add_symbol<std::tuple<Tail...>, Sym, Exp>::type>() ) );
   };

   // ───────────────────────────────────────────────────────────────
   // 5.  Build the final tuple<symbol_exponent<…>...>
   // ───────────────────────────────────────────────────────────────
   // primary: default “do nothing”
   template<typename Tuple, typename Coeff, typename = void>
   struct merge_symbol
   {
      using type = Tuple;                          // case #3  (std::ratio)
   };

   // case #1 — plain symbol
   template<typename Tuple, typename Sym>
      requires is_symbol<Sym>
   struct merge_symbol<Tuple, Sym>
   {
      using type = typename add_symbol<
                     Tuple,
                     std::remove_cvref_t<Sym>,
                     std::ratio<1>
                  >::type;
   };

   // case #2 — symbol_exponent<Sym, Num, Den>
   template<typename Tuple, typename SE>
      requires is_symbol_exponent<SE>
   struct merge_symbol<Tuple, SE>
   {
      using type = typename add_symbol<
                     Tuple,
                     typename std::remove_cvref_t<SE>::symbol,
                     typename std::remove_cvref_t<SE>::exponent
                  >::type;
   };
   
   template<typename Tuple, typename... Cs>
   struct build_symbols;                       // primary

   template<typename Tuple>
   struct build_symbols<Tuple>                 // base case: no coeffs left
   {
      using type = Tuple;
   };

   template<typename Tuple, typename First, typename... Rest>
   struct build_symbols<Tuple, First, Rest...>
   {
      using next_tuple =
         typename merge_symbol<Tuple, std::remove_cvref_t<First>>::type;

      using type =
         typename build_symbols<next_tuple, Rest...>::type;
   };

   // ───────────────────────────────────────────────────────────────
   // 6.  handle_coefficients<Ts…>
   // ───────────────────────────────────────────────────────────────
   template<is_coefficient... Ts>
   struct handle_coefficients
   {
   private:
      /*  split Ts...  */
      using ratio_list   = typename combine_ratios<
                              std::conditional_t< is_ratio<Ts>, Ts, std::ratio<1>>...
                           >::type;

      using symbol_tuple = typename build_symbols<std::tuple<>, Ts...>::type;

   public:
      using ratio   = ratio_list;         // aggregate ∏ ratios
      using symbols = symbol_tuple;       // merged symbol_exponent tuple
   };

   template<typename Tuple>
   struct handle_coefficients_tuple;                      // forward declaration

   template<typename... Ts>
   struct handle_coefficients_tuple<std::tuple<Ts...>>
      : handle_coefficients<Ts...> { };


   template<typename... Ts> struct partition_coeffs;

   template<>
   struct partition_coeffs<> {
      using units   = std::tuple<>;
      using coeffs  = std::tuple<>;
   };

   template<typename Head, typename... Tail>
   struct partition_coeffs<Head, Tail...> {
   private:
      using tail = partition_coeffs<Tail...>;
   public:
      using units  = std::conditional_t<are_unit_exponents<Head>,
                                       decltype(std::tuple_cat(std::declval<std::tuple<Head>>(),
                                                               std::declval<typename tail::units>())),
                                       typename tail::units>;

      using coeffs = std::conditional_t<is_coefficient<Head>,
                                       decltype(std::tuple_cat(std::declval<std::tuple<Head>>(),
                                                               std::declval<typename tail::coeffs>())),
                                       typename tail::coeffs>;
   };


   // ─────────────── eval-one trait  (specialisable) ───────────────
   template<typename T> struct eval_one_trait;               // primary (never used)

   // plain symbol  → exponent 1
   template<typename Sym>
      requires std::derived_from<Sym, symbol>
   struct eval_one_trait<Sym>
   {
      static constexpr long double value = Sym::value;
   };

   // symbol_exponent<Sym, Num, Den>  →  pow(Sym::value, Num/Den)
   template<typename Sym, int Num, int Den>
   struct eval_one_trait<symbol_exponent<Sym, Num, Den>>
   {
   private:
      static constexpr long double base =
         static_cast<long double>(Sym::value);
   public:
      static constexpr long double value = pow_impl(base, Num, Den);
   };

   // ─────────────── tuple front-end ───────────────
   template<typename Tuple> struct eval_symbol_tuple_impl;     // primary

   // specialisation that unpacks the tuple
   template<typename... Ts>
   struct eval_symbol_tuple_impl<std::tuple<Ts...>>
   {
      static constexpr long double product =
         (eval_one_trait<Ts>::value * ... * 1.0L);          // fold

      static constexpr double value =
         static_cast<double>(product);
   };

   // public helper
   template<typename Tuple>
   constexpr double eval_symbol_tuple()
   {
      return eval_symbol_tuple_impl<std::remove_cvref_t<Tuple>>::value;
   }

   // ============================================================================
   //  helper: negate a coefficient (for division)
   // ============================================================================
   template<typename T> struct negate_coeff;                           // primary

   // 1) plain symbol          → exponent -1
   template<is_symbol Sym>
   struct negate_coeff<Sym>
   {
      using type = symbol_exponent<Sym, -1, 1>;
   };

   // 2) symbol_exponent<Sym, N, D>  → exponent -N/D
   template<typename Sym, int Num, int Den>
   struct negate_coeff<symbol_exponent<Sym, Num, Den>>
   {
      using type = symbol_exponent<Sym, -Num, Den>;
   };

   // ============================================================================
   //  multiply_symbol_tuples<tuple<A...>, tuple<B...>>::type
   // ============================================================================
   template<typename... Ts>
   struct multiply_symbol_tuples;
   template<typename... T1s, typename... T2s>
   struct multiply_symbol_tuples<std::tuple<T1s...>, std::tuple<T2s...>>
   {
      using type = typename build_symbols<std::tuple<>,T1s..., T2s...>::type;
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
      using negated_pack = std::tuple<typename negate_coeff<T2s>::type...>;

   public:
      using type = typename multiply_symbol_tuples<std::tuple<T1s...>, negated_pack>::type;
   };

}

#endif //DIMENSIONAL_COEFFICIENT_H
