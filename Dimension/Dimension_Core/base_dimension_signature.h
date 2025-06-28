#ifndef STATIC_DIMENSION_BASE_SIGN_H
#define STATIC_DIMENSION_BASE_SIGN_H

#include <concepts>

#include "UnitValidation.h"

namespace dimension
{

   template<typename T>
   concept rep_type = requires(T a, T b) {
      { T(a) };                                   // copy constructible
      { a = b } -> std::same_as<T&>;              // copy assignable

      { a + b } -> std::convertible_to<T>;
      { a - b } -> std::convertible_to<T>;
      { a * b } -> std::convertible_to<T>;
      { a / b } -> std::convertible_to<T>;

      { a += b } -> std::same_as<T&>;
      { a -= b } -> std::same_as<T&>;
      { a *= b } -> std::same_as<T&>;
      { a /= b } -> std::same_as<T&>;

      { a == b } -> std::convertible_to<bool>;
      { a != b } -> std::convertible_to<bool>;
      { a <  b } -> std::convertible_to<bool>;
      { a <= b } -> std::convertible_to<bool>;
      { a >  b } -> std::convertible_to<bool>;
      { a >= b } -> std::convertible_to<bool>;
   };

   // Forward declarations
   template<rep_type Rep, are_unit_exponents... Units>
   class base_dimension_impl;

   template<typename... Args>
   struct base_dimension_wrapper;

   template<typename... Args>
   using base_dimension = typename base_dimension_wrapper<Args...>::type;

}

#endif //STATIC_DIMENSION_BASE_SIGN_H
