#ifndef DIMENSIONAL_UNIT_EXPONENT_H
#define DIMENSIONAL_UNIT_EXPONENT_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction
#include <ratio>

#include "unit_dim.h"
#include "../TupleHandling.h"
#include "../ratio/ratio_utils.h"

namespace dimension
{
   template<typename Unit, int Num = 1, int Den = 1, class Label = void>
   struct unit_exponent
   {
      using unit = Unit;
      using exponent = std::ratio<Num, Den>;
      using label = Label;
   };

   // ───────────────────────────── helper: map one unit_exponent ─────────────────────────────
   template<typename UE> struct to_primary_ue;                 // primary template

   template<typename U, int Num, int Den>                     // specialization
   struct to_primary_ue<unit_exponent<U, Num, Den>>
   {
      using type = unit_exponent<unit_primary_t<U>, Num, Den>;
   };

   template<typename UE>
   using to_primary_ue_t = typename to_primary_ue<UE>::type;

   // ───────────────────────────── helper: map a whole std::tuple ────────────────────────────
   template<typename Tuple> struct to_primary_tuple;           // primary template

   template<typename... UE>
   struct to_primary_tuple<std::tuple<UE...>>
   {
      using type = std::tuple<to_primary_ue_t<UE>...>;
   };

   template<typename Tuple>
   using to_primary_tuple_t = typename to_primary_tuple<Tuple>::type;


   template<typename T>
   concept is_unit_exponent = requires {
         typename T::unit;
         typename T::exponent;
   };

   template<typename... Ts>
   concept are_unit_exponents = (is_unit_exponent<Ts> && ...);

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

   template<typename TupleA, typename TupleB>
   struct unit_exponent_sets_equal;

   template<typename... A, typename... B>
   struct unit_exponent_sets_equal<std::tuple<A...>, std::tuple<B...>>
      : std::bool_constant<
            (contains_unit_exponent<A, std::tuple<B...>>::value && ...) &&
            (contains_unit_exponent<B, std::tuple<A...>>::value && ...)
      > {};


   template<typename Tuple, typename Label>
   struct label_all_unit_exponents;

   template<typename... Ts, typename Label>
   struct label_all_unit_exponents<std::tuple<Ts...>, Label> {
   using type = std::tuple<
      unit_exponent<typename Ts::unit,
                     Ts::exponent::num,
                     Ts::exponent::den,
                     Label>...>;
   };

   template<typename Tuple, typename Label>
   using label_all_unit_exponents_t =
   typename label_all_unit_exponents<Tuple, Label>::type;

   // TODO: Consider a generic "Raise" and "Raise All" to apply to each _exponent type

   template<is_unit_exponent T, is_ratio R>
   struct raise_unit_exponent
   {
   private:
      using new_exponent = std::ratio_multiply<typename T::exponent, R>;
   public:
      using type = unit_exponent<typename T::unit, new_exponent::num, new_exponent::den, typename T::label>;
   };

   template<typename Tuple, is_ratio R>
   struct raise_all_unit_exponents;  // primary

   template<typename... Ts, is_ratio R>
   struct raise_all_unit_exponents<std::tuple<Ts...>, R>
   {
       using type = std::tuple<typename raise_unit_exponent<Ts, R>::type...>;
   };

   template<typename Tuple, is_ratio R>
   using raise_all_unit_exponents_t = typename raise_all_unit_exponents<Tuple, R>::type;

}

#endif // DIMENSIONAL_UNIT_EXPONENT_H
