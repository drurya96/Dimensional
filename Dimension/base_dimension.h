#ifndef STATIC_DIMENSION_BASE_H
#define STATIC_DIMENSION_BASE_H

#include "base_dimension_impl.h"

namespace dimension
{

   // Forward declarations
   template<rep_type Rep, is_coefficient_or_unit... Ts>
   class base_dimension_impl;

   template<typename... Args>
   struct base_dimension_wrapper;

   // ────────────────────────────────────────────────────────────────
   //  base_dimension_wrapper specializations
   // ────────────────────────────────────────────────────────────────

   // 1.  Rep specified  +  at least ONE unit  + optional coeffs/units
   template<rep_type Rep,
            are_unit_exponents First,
            is_coefficient_or_unit... Rest>
   struct base_dimension_wrapper<Rep, First, Rest...>
   {
      using type = base_dimension_impl<Rep, First, Rest...>;
   };

   // 2.  Rep omitted (defaults to double)  +  at least ONE unit  + optional coeffs/units
   template<are_unit_exponents First,
            is_coefficient_or_unit... Rest>
   struct base_dimension_wrapper<First, Rest...>
   {
      using type = base_dimension_impl<double, First, Rest...>;
   };

   // 3.  Explicit Rep only      (unchanged)
   template<rep_type Rep>
   struct base_dimension_wrapper<Rep>
   {
      using type = base_dimension_impl<Rep>;
   };

   // 4.  No arguments            (unchanged)
   template<>
   struct base_dimension_wrapper<>
   {
      using type = base_dimension_impl<double>;
   };

   //--------------------------------------------------------------------------
   // 5.  Rep *and* at least one coefficient (no unit exponents)
   //--------------------------------------------------------------------------
   template<rep_type Rep,
            is_coefficient FirstCoeff,
            is_coefficient... RestCoeffs>
   struct base_dimension_wrapper<Rep, FirstCoeff, RestCoeffs...>
   {
      using type = base_dimension_impl<Rep, FirstCoeff, RestCoeffs...>;
   };

   //--------------------------------------------------------------------------
   // 6.  No Rep given   +   coefficient pack only  →  default Rep = double
   //--------------------------------------------------------------------------
   template<is_coefficient FirstCoeff,
            is_coefficient... RestCoeffs>
   struct base_dimension_wrapper<FirstCoeff, RestCoeffs...>
   {
      using type = base_dimension_impl<double, FirstCoeff, RestCoeffs...>;
   };

   template<typename... Args>
   using base_dimension = typename base_dimension_wrapper<Args...>::type;
}

#endif // STATIC_DIMENSION_BASE_H
