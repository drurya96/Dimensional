#ifndef DIMENSIONAL_REP_TYPE_H
#define DIMENSIONAL_REP_TYPE_H

#include <concepts>
#include <type_traits>

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

}

#endif //DIMENSIONAL_REP_TYPE_H
