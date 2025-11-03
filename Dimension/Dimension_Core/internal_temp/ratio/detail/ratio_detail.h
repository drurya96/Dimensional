#ifndef DIMENSIONAL_RATIO_DETAIL_H
#define DIMENSIONAL_RATIO_DETAIL_H

#include <ratio>
#include <tuple>
#include <type_traits>
#include <limits>
#include <cmath>

#include "../../TemplateUtils/GenericUtils.h" // filter_pack_tuple_t

namespace dimension::detail::ratio_impl {

   // ---------- Traits / concepts ----------
   template<class T> struct is_ratio_type : std::false_type {};
   template<std::intmax_t Num, std::intmax_t Den>
   struct is_ratio_type<std::ratio<Num, Den>> : std::true_type {};

   template<class T>
   inline constexpr bool is_ratio_v = is_ratio_type<std::remove_cvref_t<T>>::value;

   template<class T>
   struct is_ratio_trait : std::bool_constant<is_ratio_v<T>> {};

   // ---------- Variadic ratio folds ----------
   template<class... Rs> struct ratio_add_fold { using type = std::ratio<0>; };
   template<class R1, class... Rs>
   struct ratio_add_fold<R1, Rs...> {
      using type = typename std::ratio_add<R1, typename ratio_add_fold<Rs...>::type>::type;
   };

   template<class... Rs> struct ratio_mul_fold { using type = std::ratio<1>; };
   template<class R1, class... Rs>
   struct ratio_mul_fold<R1, Rs...> {
      using type = typename std::ratio_multiply<R1, typename ratio_mul_fold<Rs...>::type>::type;
   };
   template<class... Rs>
   using ratio_mul_t = typename ratio_mul_fold<Rs...>::type;

   // Tuple adapter
   template<class Tuple> struct ratio_multiply_tuple;
   template<class... Rs>
   struct ratio_multiply_tuple<std::tuple<Rs...>> { using type = ratio_mul_t<Rs...>; };
   template<class Tuple>
   using ratio_multiply_tuple_t = typename ratio_multiply_tuple<Tuple>::type;

   template<typename R>
   requires is_ratio_v<R>
   constexpr double ratio_value() {
      return static_cast<double>(R::num) / static_cast<double>(R::den);
   }

   //template<class R, int N>
   //struct raise_ratio;

   template<typename R, int N>
   requires is_ratio_v<R>
   struct raise_ratio_pos {
      static_assert(N >= 0, "N must be non-negative here");

      using RR = std::ratio_multiply<R, R>; // no ::type needed in C++17+

      using type = std::conditional_t<
            (N % 2 == 0),
            typename raise_ratio_pos<RR, (N / 2)>::type,
            std::ratio_multiply<R, typename raise_ratio_pos<RR, ((N - 1) / 2)>::type>
      >;
   };

   // N == 0: base case
   template<typename R>
   requires is_ratio_v<R>
   struct raise_ratio_pos<R, 0> {
      using type = std::ratio<1, 1>;
   };

   // (Optional) N == 1 fast path
   template<typename R>
   requires is_ratio_v<R>
   struct raise_ratio_pos<R, 1> {
      using type = R;
   };

   template<typename R, int N>
   requires is_ratio_v<R>
   struct raise_ratio {
   // Negative powers → invert the ratio
   using type = std::conditional_t<
      (N < 0),
      raise_ratio_pos<std::ratio<R::den, R::num>, -N>,
      raise_ratio_pos<R, N>
   >::type;
   };

   template<class... Ts>
   using extract_ratios_t =
         filter_pack_tuple_t<is_ratio_trait, Ts...>;


} // namespace dimension::detail::ratio_impl

#endif // DIMENSIONAL_RATIO_DETAIL_H
