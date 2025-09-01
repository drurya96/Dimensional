#ifndef DIMENSIONAL_FUNCTIONS_H
#define DIMENSIONAL_FUNCTIONS_H

// TODO: Add appropriate includes
#include "base_dimension_impl.h"
#include "Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

namespace dimension
{
   // TODO: IMPORTANT: Write an issue for future work:
   //   We could have a `ratio_exponent` alongside `unit_exponent` and `symbol_exponent`
   //   This would allow "perfect" representation of ratio-roots.
   //   API surface should remain unchanged if this is added later,
   //   so it will not be include in Dimensional 3.0.0

   template<int Num, unsigned int Den = 1, is_base_dimension T>
   constexpr auto pow_dimension_impl(T base)
   {
      if constexpr (Num == 0)
      {
         // 0th power: returns dimensionless with value 1.0
         return base_dimension_impl<double>{1.0};
      }
      else if constexpr (Num == 1 && Den == 1)
      {
         return base;
      }
      else if constexpr (Den == 1)
      {
         // Exact ratio handling
         using ratio = raise_ratio_t<typename T::ratio, Num>;
         using symbols = detail::raise_all_symbol_exponents_t<typename T::symbols, std::ratio<Num, Den>>;
         using units = raise_all_unit_exponents_t<typename T::units, std::ratio<Num, Den>>;

         return typename base_dimension_from_tuple<typename T::rep, ratio, units, symbols>::dim(
            pow_rational(base.template get_tuple_scalar<typename T::units>(), Num, Den)
         );
      }
      else
      {
         // Root - cannot do exact ratio handling
         using symbols = detail::raise_all_symbol_exponents_t<typename T::symbols, std::ratio<Num, Den>>;
         using units = raise_all_unit_exponents_t<typename T::units, std::ratio<Num, Den>>;

         return typename base_dimension_from_tuple<typename T::rep, units, symbols>::dim(
            pow_rational(base.template get_tuple_scalar<typename T::units>() * ratio_v<typename T::ratio>, Num, Den)
         );
      }
   }



   
   template<unsigned int exponent, typename Dim>
   [[nodiscard]] constexpr auto Pow(Dim obj)
   {
      return pow_dimension_impl<exponent>(obj);
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
