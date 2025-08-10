#ifndef DIMENSIONAL_RATIO_UTILS_H
#define DIMENSIONAL_RATIO_UTILS_H

#include "detail/ratio_detail.h"

namespace dimension {
   
   template<class T>
   concept is_ratio = detail::ratio_impl::is_ratio_v<T>;

   template<class... Rs>
   using ratio_add_t = typename detail::ratio_impl::ratio_add_fold<Rs...>::type;

   template<class... Ts>
   using extract_ratio_t = detail::ratio_impl::ratio_multiply_tuple_t<detail::ratio_impl::extract_ratios_t<Ts...>>;

   template<is_ratio R>
   inline constexpr double ratio_v = detail::ratio_impl::ratio_value<R>();

   template<is_ratio R, int N>
   using raise_ratio_t = typename detail::ratio_impl::raise_ratio<R, N>::type;

} // namespace dimension

#endif // DIMENSIONAL_RATIO_UTILS_H
