#ifndef DIMENSIONAL_SYMBOL_UTILS_H
#define DIMENSIONAL_SYMBOL_UTILS_H

#include <ratio>
#include <tuple>

#include "detail/symbol_core.h"
#include "detail/symbol_detail.h"

namespace dimension
{
   template<is_symbol sym, int Num, int Den>
   struct symbol_exponent
   {
      static_assert(Den != 0, "Denominator cannot be zero");
      using symbol = sym;
      using exponent = std::ratio<Num, Den>;
   };

   namespace detail{
      template<typename Tuple>
      inline constexpr double multiply_symbol_exponent_values_v = detail::symbol_utils_impl::multiply_symbol_exponent_values<Tuple>::value;

      template<typename Tuple>
      using collapse_symbol_exponents_t = typename detail::symbol_utils_impl::collapse_symbol_exponents<Tuple>::type;

      template<typename... Ts>
      using collect_symbol_exponents_t = typename detail::symbol_utils_impl::collect_symbol_exponents<Ts...>::type;

      template<typename Tuple1, typename Tuple2>
      using multiply_symbol_tuples_t = typename detail::symbol_utils_impl::multiply_symbol_tuples<Tuple1, Tuple2>::type;

      template<typename Tuple1, typename Tuple2>
      using divide_symbol_tuples_t = typename detail::symbol_utils_impl::divide_symbol_tuples<Tuple1, Tuple2>::type;

      template<typename Tuple, is_ratio R>
      using raise_all_symbol_exponents_t = typename detail::symbol_utils_impl::raise_all_symbol_exponents<Tuple, R>::type;
   }

   template<typename... Ts>
   using extract_symbols_t = detail::collapse_symbol_exponents_t<detail::collect_symbol_exponents_t<Ts...>>;

}

#endif //DIMENSIONAL_SYMBOL_UTILS_H
