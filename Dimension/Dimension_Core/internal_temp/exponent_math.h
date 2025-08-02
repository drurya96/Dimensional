#ifndef DIMENSIONAL_EXPONENT_MATH_H
#define DIMENSIONAL_EXPONENT_MATH_H

#include <cmath>

namespace dimension::math
{
   template<int Power>
   constexpr double PowInt(double value)
   {
      if constexpr (Power % 2 == 0)
      {
         double half = PowInt<Power / 2>(value);
         return half * half;
      }
      else
      {
         return value * PowInt<Power - 1>(value);
      }
   }

   template<int Power>
   requires (Power < 0)
   constexpr double PowInt(double value)
   {
      return 1.0 / PowInt<-Power>(value);
   }

   template<>
   constexpr double PowInt<0>(double)
   {
      return 1.0;
   }

   template<>
   constexpr double PowInt<1>(double value)
   {
      return value;
   }

   template<>
   constexpr double PowInt<2>(double value)
   {
      return value * value;
   }

   template<>
   constexpr double PowInt<3>(double value)
   {
      return value * value * value;
   }

   template<>
   constexpr double PowInt<4>(double value)
   {
      return value * value * value * value;
   }

   template<>
   constexpr double PowInt<5>(double value)
   {
      return value * value * value * value * value;
   }

   template<int Root>
   constexpr double RootInt(double value)
   {
      static_assert(Root != 0, "RootInt<0> is undefined.");

      return std::pow(value, 1.0 / static_cast<double>(Root));
   }

   template<>
   inline double RootInt<1>(double value)
   {
      return value;
   }

   template<>
   inline double RootInt<2>(double value)
   {
      return std::sqrt(value);
   }

   template<>
   inline double RootInt<3>(double value)
   {
      return std::cbrt(value);
   }

   template<>
   inline double RootInt<4>(double value)
   {
      return std::sqrt(std::sqrt(value));
   }
}

#endif // DIMENSIONAL_EXPONENT_MATH_H
