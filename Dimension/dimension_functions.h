#ifndef DIMENSIONAL_FUNCTIONS_H
#define DIMENSIONAL_FUNCTIONS_H

// TODO: Add appropriate includes
#include "base_dimension_impl.h"

namespace dimension
{

   /// @brief Implementation for Pow (exponential)
   /// @details Uses exponentiation by squares method
   /// @tparam exponent the exponent to raise to. This must be a positive integer
   /// @tparam Dim The input dimension type
   /// @tparam TransitiveDim The dimension type returned from this recursive call of PowImpl
   /// @param[in] obj The input dimension object
   /// @param[in] transitiveDim The transitive object from the previous recursive call
   /// @return transitiveDim multiplied by obj either one or two times
   ///    for even or odd exponents, respectively.
   template<unsigned int exponent, is_base_dimension T>
   constexpr auto PowImpl(const T& base)
   {
      if constexpr (exponent == 0)
      {
         // 0th power: returns dimensionless with value 1.0
         return base_dimension_impl<double>{1.0};
      }
      else if constexpr (exponent == 1)
      {
         return base;
      }
      else if constexpr (exponent % 2 == 0)
      {
         auto half = PowImpl<exponent / 2>(base);
         return half * half;
      }
      else
      {
         auto half = PowImpl<exponent / 2>(base);
         return half * half * base;
      }
   }

   /// @brief Implementation for Pow (exponential)
   /// @tparam exponent the exponent to raise to. This must be a positive integer
   /// @tparam Dim The input dimension type
   /// @param[in] obj The object to raise to exponent power
   /// @return A dimension object of units raised to exponent power,
   ///    and a value raised to exponent power
   template<unsigned int exponent, typename Dim>
   [[nodiscard]] constexpr auto Pow(Dim obj)
   {
      return PowImpl<exponent>(obj);
   }

   /// @brief Calculate hypotenuse from two sides of a right triangle
   /// @tparam T Side type, must be a base_dimension
   /// @param obj1 Side of right triangle
   /// @param obj2 Side of right triangle
   /// @return Hypotenuse of right triangle
   template<is_base_dimension T>
   [[nodiscard]] constexpr T hypot(T obj1, T obj2)
   {
      return T(std::hypot(
         call_unpack<typename T::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj1); }),
         call_unpack<typename T::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj2); })
      ));
   }

   /// @brief Calculate absolute value of a dimension
   /// @tparam T dimension type
   /// @param obj dimension object
   /// @return Absolute value of the dimension
   template<is_base_dimension T>
   [[nodiscard]] constexpr T abs(T obj)
   {
      return obj < T{0} ? -obj : obj;
   }

   /// @brief Round dimension down to nearest whole number
   /// @tparam T dimension type
   /// @param obj dimension object
   /// @return dimension object rounded down to nearest whole number
   template<is_base_dimension T>
   [[nodiscard]] constexpr T floor(const T& obj)
   {
      return T(static_cast<T::rep>(static_cast<int>(get_dimension_tuple<typename T::units>(obj)) - 
              (get_dimension_tuple<typename T::units>(obj) < static_cast<int>(get_dimension_tuple<typename T::units>(obj)))));
   }

   /// @brief Round dimension up to nearest whole number
   /// @tparam T dimension type
   /// @param obj dimension object
   /// @return dimension object rounded up to nearest whole number
   template<is_base_dimension T>
   [[nodiscard]] constexpr T ceil(const T& obj)
   {
      return T(static_cast<T::rep>(static_cast<int>(get_dimension_tuple<typename T::units>(obj)) +
               (get_dimension_tuple<typename T::units>(obj) > static_cast<int>(get_dimension_tuple<typename T::units>(obj)))));
   }

   /// @brief Round dimension to nearest whole number
   /// @tparam T dimension type
   /// @param obj dimension object
   /// @return dimension object rounded to nearest whole number
   template<is_base_dimension T>
   [[nodiscard]] constexpr T round(const T& obj)
   {
      const auto val = get_dimension_tuple<typename T::units>(obj);
      const auto floor_val = static_cast<T::rep>(static_cast<int>(val));
      return T((val - floor_val < 0.5) ? floor_val : floor_val + 1.0);
   }

   /// @brief Decompose dimension into integer and floating point type
   /// @tparam T dimension type
   /// @param obj dimension object to decompose
   /// @param[out] intPart A pointer to a dimension object which will store the integer part
   /// @return The fractional component of the dimension
   /// @todo Consider marking this function constexpr in C++23 (modf is constexpr in C++23)
   template<is_base_dimension T>
   [[nodiscard]] constexpr T modf(const T& obj, T* intPart = nullptr)
   {
      const auto val = get_dimension_tuple<typename T::units>(obj);
      const auto int_val = static_cast<T::rep>(static_cast<long long>(val));
      if (intPart) {
         *intPart = T(int_val);
      }
      return T(val - int_val);
   }

   /// @brief Floating point remainder division of dividend / divisor
   /// @tparam T dimension type
   /// @param dividend dimension object to be divided
   /// @param divisor dimension object to divide by
   /// @return dimension object of floating point remainder division (modulus)
   /// @todo Consider marking this function constexpr in C++23 (fmod is constexpr in C++23)
   template<is_base_dimension T>
   [[nodiscard]] constexpr T fmod(const T& dividend, const T& divisor)
   {
      const auto num = get_dimension_tuple<typename T::units>(dividend);
      const auto denom = get_dimension_tuple<typename T::units>(divisor);
      return T(denom == 0 ? throw std::invalid_argument("Divisor cannot be zero.")
                          : num - static_cast<T::rep>(static_cast<long long>(num / denom)) * denom);
   }
   
}

#endif // DIMENSIONAL_FUNCTIONS_H
