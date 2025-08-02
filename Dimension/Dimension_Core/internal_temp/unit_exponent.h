#ifndef DIMENSION_UNIT_EXPONENT_H
#define DIMENSION_UNIT_EXPONENT_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction

#include "Conversion.h" // TODO: Remove this by moving PrimaryConvertible and friends to a separate header

namespace dimension
{
   template<typename Unit, int Num = 1, int Den = 1>
   struct unit_exponent
   {
      using unit = Unit;
      using exponent = std::ratio<Num, Den>;

      static constexpr auto exponentString = ratio_literal<Num, Den>();
   };

   // ───────────────────────────── helper: map one unit_exponent ─────────────────────────────
   template<typename UE> struct to_primary_ue;                 // primary template

   template<typename U, int Num, int Den>                     // specialization
   struct to_primary_ue<unit_exponent<U, Num, Den>>
   {
      using type = unit_exponent<typename U::Primary, Num, Den>;
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
}

#endif // DIMENSION_UNIT_EXPONENT_H
