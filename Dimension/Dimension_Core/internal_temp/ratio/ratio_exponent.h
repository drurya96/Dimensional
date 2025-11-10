#ifndef DIMENSIONAL_RATIO_EXPONENT_H
#define DIMENSIONAL_RATIO_EXPONENT_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction
#include <ratio>

#include "../TupleHandling.h"
#include "../exponentiation/exponentiation.h"
#include "ratio_utils.h"

namespace dimension
{
   template<typename R, int Num = 1, int Den = 1>
   struct ratio_exponent
   {
      using ratio = R;
      using exponent = std::ratio<Num, Den>;
   };

   template<typename> inline constexpr bool is_ratio_exponent_v = false;

   template<typename R, int Num, int Den>
   inline constexpr bool
   is_ratio_exponent_v<ratio_exponent<R, Num, Den>> = true;

   template<typename T>
   concept is_ratio_exponent = is_ratio_exponent_v<std::remove_cvref_t<T>>;

   template<is_ratio R, int Num, int Den>
   constexpr double ratio_exp_value()
   {
      return pow_rational(ratio_v<R>, Num, Den);
   }

   template<is_ratio_exponent R>
   constexpr double ratio_exponent_value()
   {
      return ratio_exp_value<typename R::ratio, R::exponent::num, R::exponent::den>();
   }

   template<is_ratio_exponent R>
   inline constexpr double ratio_exponent_value_v = ratio_exponent_value<R>();

   template<typename RE, typename P>
   struct raise_ratio_exponent; // primary

   // NOTE: Pn/Pd must be std::intmax_t to match std::ratio's parameters
   template<typename R, int Num, int Den, std::intmax_t Pn, std::intmax_t Pd>
   struct raise_ratio_exponent<ratio_exponent<R, Num, Den>, std::ratio<Pn, Pd>>
   {
      using cur  = std::ratio<Num, Den>;
      using next = std::ratio_multiply<cur, std::ratio<Pn, Pd>>;
      using type = ratio_exponent<R, next::num, next::den>;
   };


   namespace detail {
   namespace ratio_utils_impl {

      template<typename Tuple, typename P>
      struct raise_all_ratio_exponents;

      // fold state is a std::tuple<...>
      template<typename P>
      struct push_scaled_ratio_exp
      {
         template<typename Accum, typename RE>
         struct apply {
            using raised = typename raise_ratio_exponent<RE, P>::type;
            using type   = append_t<Accum, raised>;
         };
      };

      template<typename Tuple, typename P>
      struct raise_all_ratio_exponents
      {
         // Start from empty tuple and append transformed elements
         using type = fold_over_tuple_with_state_t<
            Tuple,
            std::tuple<>,
            push_scaled_ratio_exp<P>::template apply
         >;
      };

      
      template<class R>
      struct canonical_ratio  { using type = std::ratio<R::num, R::den>; };
      template<class R>
      using canonical_ratio_t = typename canonical_ratio<R>::type;

      // Normalize a single ratio_exponent's base type
      template<class RE> struct normalize_ratio_exponent;

      template<class R, int Num, int Den>
      struct normalize_ratio_exponent<ratio_exponent<R, Num, Den>> {
      using type = ratio_exponent< canonical_ratio_t<R>, Num, Den >;
      };

      // Map a tuple of ratio_exponent to canonical base types
      template<class Tuple> struct normalize_ratio_exponent_tuple;

      template<class... Es>
      struct normalize_ratio_exponent_tuple<std::tuple<Es...>> {
      using type = std::tuple< typename normalize_ratio_exponent<Es>::type... >;
      };






      // ---- 1) add_or_merge for ratio_exponent<Ratio, exp> (mirror of symbols) ----

      template<typename Tuple, typename RE>
      struct add_or_merge_ratio;

      template<typename RE>
      struct add_or_merge_ratio<std::tuple<>, RE> {
      using NE  = typename normalize_ratio_exponent<RE>::type;
      using exp = typename NE::exponent;
      using type = std::conditional_t<
         std::ratio_equal_v<exp, std::ratio<0>>,
         std::tuple<>,
         std::tuple<NE>
      >;
      };

      template<typename Head, typename... Tail, typename RE>
      struct add_or_merge_ratio<std::tuple<Head, Tail...>, RE> {
      private:
      using Hcanon  = canonical_ratio_t<typename Head::ratio>;
      using NE      = typename normalize_ratio_exponent<RE>::type;
      using Ecanon  = canonical_ratio_t<typename NE::ratio>;

      static constexpr bool match = std::is_same_v<Hcanon, Ecanon>;

      using merged_exponent = std::ratio_add<typename Head::exponent, typename NE::exponent>;
      using merged          = ratio_exponent<Ecanon, merged_exponent::num, merged_exponent::den>;

      using rest = std::tuple<Tail...>;
      using next = typename add_or_merge_ratio<rest, NE>::type;

      public:
      using type = std::conditional_t<
         match,
         std::conditional_t<
            std::ratio_equal_v<merged_exponent, std::ratio<0>>,
            rest,
            std::tuple<merged, Tail...>
         >,
         decltype(std::tuple_cat(std::tuple<Head>{}, next{}))
      >;
      };

      // ---- 2) collapse a tuple of ratio_exponent by summing like bases ----

      template<typename Tuple>
      struct collapse_ratio_exponents;

      template<>
      struct collapse_ratio_exponents<std::tuple<>> {
      using type = std::tuple<>;
      };

      template<typename Head, typename... Tail>
      struct collapse_ratio_exponents<std::tuple<Head, Tail...>> {
      private:
      using rest_collapsed = typename collapse_ratio_exponents<std::tuple<Tail...>>::type;
      public:
      using type = typename add_or_merge_ratio<rest_collapsed, Head>::type;
      };

      // ---- 3) tuple multiply/divide for ratio_exponent tuples ----

      template<typename... Ts>
      struct multiply_ratio_exponent_tuples;

      template<typename... A, typename... B>
      struct multiply_ratio_exponent_tuples<std::tuple<A...>, std::tuple<B...>> {
      using type = typename collapse_ratio_exponents<std::tuple<A..., B...>>::type;
      };

      template<typename... Ts>
      struct divide_ratio_exponent_tuples;

      template<typename... A, typename... B>
      struct divide_ratio_exponent_tuples<std::tuple<A...>, std::tuple<B...>> {
      private:
      // invert exponents of RHS
      template<typename T> struct invert_one;
      template<typename R, int Num, int Den>
      struct invert_one<ratio_exponent<R, Num, Den>> {
         using type = ratio_exponent<R, -Num, Den>;
      };
      using B_inv = std::tuple<typename invert_one<B>::type...>;
      public:
      using type = typename multiply_ratio_exponent_tuples<std::tuple<A...>, B_inv>::type;
      };




      template<class... Ts>
      constexpr double multiply_ratio_exponent_values_impl() {
         return (1.0 * ... * ratio_exponent_value_v<Ts>);
      }

      template<class Tuple>
      struct multiply_ratio_exponent_values;

      template<class... Ts>
      struct multiply_ratio_exponent_values<std::tuple<Ts...>> {
         static constexpr double value = multiply_ratio_exponent_values_impl<Ts...>();
      };




   }} // namespace detail::ratio_utils_impl

   template<typename Tuple1, typename Tuple2>
   using multiply_ratio_exponent_tuples_t = typename detail::ratio_utils_impl::multiply_ratio_exponent_tuples<Tuple1, Tuple2>::type;

   template<typename Tuple1, typename Tuple2>
   using divide_ratio_exponent_tuples_t = typename detail::ratio_utils_impl::divide_ratio_exponent_tuples<Tuple1, Tuple2>::type;

   template<typename Tuple>
   inline constexpr double multiply_ratio_exponent_values_v = detail::ratio_utils_impl::multiply_ratio_exponent_values<Tuple>::value;

   template<typename Tuple, typename P>
   using raise_all_ratio_exponents_t =
      typename detail::ratio_utils_impl::raise_all_ratio_exponents<Tuple, P>::type;

   template<typename Tuple>
   using collapse_ratio_exponents_t =
      typename detail::ratio_utils_impl::collapse_ratio_exponents<Tuple>::type;



   template<is_ratio_exponent T>
   struct invert_ratio_exponent
   {
   private:
      using exponent = inverse_ratio_t<typename T::exponent>;
   public:
      using type = ratio_exponent<typename T::ratio, exponent::num, exponent::den>;
   };


   template<is_ratio_exponent T>
   using invert_ratio_exponent_t = typename invert_ratio_exponent<T>::type;

   // Apply `invert_exponent_t` to each element of a std::tuple
   template<class Tuple>
   struct tuple_invert_ratio_exponents;

   template<class... SE>
   struct tuple_invert_ratio_exponents<std::tuple<SE...>> {
      using type = std::tuple< invert_ratio_exponent_t<SE>... >;
   };

   template<class Tuple>
   using tuple_invert_ratio_exponents_t = typename tuple_invert_ratio_exponents<Tuple>::type;




   // Negate a single ratio_exponent
   template<typename RE>
   struct negate_ratio_exponent;

   template<typename R, int Num, int Den>
   struct negate_ratio_exponent<ratio_exponent<R, Num, Den>>
   {
      using type = ratio_exponent<R, -Num, Den>;
   };

   // Helper alias
   template<typename RE>
   using negate_ratio_exponent_t = typename negate_ratio_exponent<RE>::type;

   // Apply to tuple
   template<typename Tuple>
   struct tuple_negate_ratio_exponents;

   template<typename... REs>
   struct tuple_negate_ratio_exponents<std::tuple<REs...>>
   {
      using type = std::tuple<negate_ratio_exponent_t<REs>...>;
   };

   template<typename Tuple>
   using tuple_negate_ratio_exponents_t = typename tuple_negate_ratio_exponents<Tuple>::type;











   // Collect only ratio_exponent types into a tuple
   template<typename... Ts>
   struct tuple_extract_ratio_exponents
   {
      using type = std::tuple<
         Ts... // filtered by "is_ratio_exponent_v"
         >;
   };

   // Specialization using pack expansion + std::conditional_t
   template<typename... Ts>
   struct tuple_extract_ratio_exponents<std::tuple<Ts...>>
   {
   private:
      template<typename T>
      using wrap_if_ratio_exp = std::conditional_t<
         is_ratio_exponent_v<T>,
         std::tuple<T>,
         std::tuple<>
      >;

   public:
      using type = decltype(std::tuple_cat(
         wrap_if_ratio_exp<Ts>()...
      ));
   };

   template<typename Tuple>
   using tuple_extract_ratio_exponents_t = typename tuple_extract_ratio_exponents<Tuple>::type;



}

#endif // DIMENSIONAL_RATIO_EXPONENT_H
