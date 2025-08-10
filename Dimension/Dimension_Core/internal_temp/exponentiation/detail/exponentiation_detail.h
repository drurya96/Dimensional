#ifndef DIMENSIONAL_EXPONENTIATION_DETAIL_H
#define DIMENSIONAL_EXPONENTIATION_DETAIL_H

#include <cmath>
#include <numeric>
#include <concepts>

// TODO: Re-evaluate the mathematics to make sure there are no holes or UB
//    Its good enough to move on for now.

namespace dimension::detail::math {

   // ----- small constexpr helpers -----
   template<std::floating_point T>
   constexpr T cx_abs(T x) { return x < T(0) ? -x : x; }

   template<std::floating_point T>
   constexpr T ipow(T base, unsigned long long e) {
      T r = T(1);
      while (e) {
         if (e & 1ULL) r *= base;
         base *= base;
         e >>= 1ULL;
      }
      return r;
   }

   template<std::floating_point T>
   constexpr T nth_root(T a, unsigned int n) {
      // preconditions: n>=1; if a<0, n must be odd (returns real root)
      bool neg = a < T(0);
      if (neg) { a = -a; }                // require odd n in that case

      // simple, stable Newton iteration; fixed step count keeps it constexpr
      T x = (a >= T(1)) ? a : T(1);
      for (int i = 0; i < 50; ++i) {
         // x_{k+1} = ((n-1)*x_k + a / x_k^{n-1}) / n
         T xnm1 = (n == 1) ? T(1) : ipow(x, n - 1);
         x = ((n - 1) * x + a / xnm1) / n;
      }
      return neg ? -x : x;
   }

   template<std::floating_point T, std::integral I, std::integral J>
   constexpr T pow_rational_impl(T base, I num, J den = J{1}) {
      // normalize fraction
      long long n = static_cast<long long>(num);
      long long d = static_cast<long long>(den);
      if (d == 0) { // UB by contract; keep it simple
         // you can replace with your preferred error handling
         return T(1) / T(0); // inf/NaN at runtime; ill-formed in consteval contexts
      }
      if (d < 0) { d = -d; n = -n; }
      auto g = std::gcd(n < 0 ? -n : n, d);
      n /= g; d /= g;

      // integer power fast-path
      if (d == 1) {
         return (n >= 0) ? ipow(base, static_cast<unsigned long long>( n))
                           : T(1) / ipow(base, static_cast<unsigned long long>(-n));
      }

      if (std::is_constant_evaluated()) {
         // all-constexpr path (no <cmath>)
         // Compute a^(n/d) as (a^(1/d))^n to keep domain handling simple.
         T root = nth_root(base, static_cast<unsigned int>(d));     // requires: base>=0 or d odd
         return (n >= 0) ? ipow(root, static_cast<unsigned long long>( n))
                           : T(1) / ipow(root, static_cast<unsigned long long>(-n));
      } else {
         // fast runtime path
         using FP = T;
         return std::pow(base, static_cast<FP>(n) / static_cast<FP>(d));
      }
   }

} // namespace dimension::detail::math

#endif // DIMENSIONAL_EXPONENTIATION_DETAIL_H
