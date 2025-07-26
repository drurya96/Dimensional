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

   template <typename T>
   constexpr T constexpr_int_pow(T base, int exp) {
      T result = T(1);
      bool negative = exp < 0;
      exp = negative ? -exp : exp;

      while (exp) {
         if (exp % 2)
            result *= base;
         base *= base;
         exp /= 2;
      }

      return negative ? T(1) / result : result;
   }

   template<typename T>
   constexpr T constexpr_root(T x, int N, int iterations = 20) {
      static_assert(std::is_floating_point_v<T>, "T must be a floating-point type");

      if (x == T(0)) return T(0);
      if (x < T(0) && N % 2 == 0)
         return std::numeric_limits<T>::quiet_NaN();

      T guess = x / N;
      for (int i = 0; i < iterations; ++i)
         guess = ((N - 1) * guess + x / constexpr_int_pow(guess, N - 1)) / N;

      return guess;
   }

   template<typename T>
   constexpr T constexpr_pow(T base, int num, int den = 1) {
      T raised = constexpr_int_pow(base, num);
      if (den == 1)
         return raised;
      else
         return constexpr_root<T>(raised, den); // placeholder, will fail to compile
   }

   template<typename T>
   constexpr T pow_impl(T base, int num, int den = 1) {
      static_assert(std::is_floating_point_v<T>, "T must be a floating-point type");

      if (std::is_constant_evaluated()) {
         return constexpr_pow(base, num, den);
      } else {
         return std::pow(base, static_cast<T>(num) / static_cast<T>(den));
      }
   }

}

#endif // DIMENSION_RATIO_UTILS_H