#ifndef DIMENSIONAL_RATIO_H
#define DIMENSIONAL_RATIO_H

#include <concepts>
#include <ratio>
#include <type_traits>

#include "symbols.h"

namespace dimension
{

   template<typename T>
   struct is_ratio_type : std::false_type {};

   template<intmax_t Num, intmax_t Den>
   struct is_ratio_type<std::ratio<Num, Den>> : std::true_type {};

   template<typename T>
   concept is_ratio = is_ratio_type<T>::value;

   template<is_ratio T>
   constexpr double ratio_value()
   {
      return static_cast<double>(T::num) / static_cast<double>(T::den);
   }

}

#endif //DIMENSIONAL_RATIO_H
