#ifndef DIMENSIONAL_EXPONENTIATION_H
#define DIMENSIONAL_EXPONENTIATION_H

#include "../ratio/ratio_utils.h"

#include "detail/exponentiation_detail.h"
#include "detail/exponent_specializations.h"
#include "../ratio/ratio_utils.h"

namespace dimension {

   template<class T, class Num, class Den = std::integral_constant<int,1>>
   requires (std::floating_point<T> && std::integral<Num> && std::integral<Den>) 
   constexpr T pow_rational(T base, Num num, Den den = {}) { return detail::math::pow_rational_impl(base, num, den); }

   template<class T, is_ratio R>
   constexpr T pow_rational(T base, R) { return detail::math::pow_rational_impl(base, R::num, R::den); }

}

#endif // DIMENSIONAL_EXPONENTIATION_H
