#ifndef DIMENSIONAL_DIMENSION_TUPLE_H
#define DIMENSIONAL_DIMENSION_TUPLE_H

#include "utils.h"
#include "rep_type.h"

namespace dimension
{

   template<rep_type Rep, is_coefficient_or_unit... Ts>
   struct base_dimension_impl;

   // ============================================================
   // ================ base_dimension From Tuple ==================
   // ============================================================
   template<typename... T>
   struct base_dimension_from_tuple;

   template<typename Rep, typename Ratio, typename... Units, typename... Coeffs>
   struct base_dimension_from_tuple<Rep, Ratio, std::tuple<Units...>, std::tuple<Coeffs...>>
   {
      using dim = base_dimension_impl<Rep, Units..., Coeffs..., Ratio>;
   };

   template<typename Rep, typename... Units, typename... Coeffs>
   struct base_dimension_from_tuple<Rep, std::tuple<Units...>, std::tuple<Coeffs...>>
   {
      using dim = base_dimension_impl<Rep, Units..., Coeffs...>;
   };

   template<typename... Units>
   struct base_dimension_from_tuple<std::tuple<Units...>>
   {
      using dim = base_dimension_impl<double, Units...>;
   };

} // end Dimension

#endif // DIMENSIONAL_DIMENSION_TUPLE_H
