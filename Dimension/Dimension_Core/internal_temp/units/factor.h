#ifndef DIMENSIONAL_FACTOR_H
#define DIMENSIONAL_FACTOR_H

#include "convertible.h"
#include "../exponentiation/exponentiation.h"

namespace dimension {


   template<class Ratio = std::ratio<1>, class... SE>
   struct factor_t {
      using ratio   = Ratio;                 // std::ratio
      using symbols = std::tuple<SE...>; // treat as a multiset to be merged
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

         return static_cast<Rep>(base_ratio * symbol_val);
      }
   }

}

#endif // DIMENSIONAL_FACTOR_H