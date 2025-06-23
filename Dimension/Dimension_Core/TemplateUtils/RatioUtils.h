#ifndef DIMENSION_RATIO_UTILS_H
#define DIMENSION_RATIO_UTILS_H

#include <ratio>

namespace dimension
{
   
   template<typename R>
   struct is_zero_ratio : std::bool_constant<R::num == 0> {};


   template<typename... R>
   struct ratio_add_fold {
       using type = std::ratio<0>;
   };
   
   template<typename R1, typename... Rs>
   struct ratio_add_fold<R1, Rs...> {
       using type = typename std::ratio_add<R1, typename ratio_add_fold<Rs...>::type>::type;
   };

   template<typename... Rs>
   using ratio_add_fold_t = typename ratio_add_fold<Rs...>::type;


   template<typename... R>
   struct ratio_multiply_fold {
       using type = std::ratio<0>;
   };
   
   template<typename R1, typename... Rs>
   struct ratio_multiply_fold<R1, Rs...> {
       using type = typename std::ratio_multiply<R1, typename ratio_multiply_fold<Rs...>::type>::type;
   };

   template<typename... Rs>
   using ratio_multiply_fold_t = typename ratio_multiply_fold<Rs...>::type;

}

#endif // DIMENSION_RATIO_UTILS_H