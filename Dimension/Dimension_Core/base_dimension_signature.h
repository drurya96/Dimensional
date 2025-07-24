#ifndef STATIC_DIMENSION_BASE_SIGN_H
#define STATIC_DIMENSION_BASE_SIGN_H

#include <concepts>
#include <ratio>
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

   struct symbol{};

   template<typename T>
   concept is_symbol = std::derived_from<std::remove_cvref_t<T>, symbol>;

   template<is_symbol sym, int Num, int Den>
   //requires std::is_base_of_v<symbol, sym>
   struct symbol_exponent
   {
      using symbol = sym;
      using exponent = std::ratio<Num, Den>;
   };

   template<typename> inline constexpr bool is_symbol_exponent_v = false;

   template<typename Sym, int Num, int Den>
   inline constexpr bool
   is_symbol_exponent_v<symbol_exponent<Sym, Num, Den>> = true;

   template<typename T>
   concept is_symbol_exponent = is_symbol_exponent_v<std::remove_cvref_t<T>>;

   template<typename T>
   struct is_ratio_type : std::false_type {};

   template<intmax_t Num, intmax_t Den>
   struct is_ratio_type<std::ratio<Num, Den>> : std::true_type {};

   template<typename T>
   concept is_ratio = is_ratio_type<T>::value;

   template<typename T>
   concept is_coefficient = is_ratio<T> || is_symbol<T> || is_symbol_exponent<T>;

   template<typename T>
   concept is_coefficient_or_unit = is_unit_exponent<T> || is_coefficient<T>;

   // Forward declarations
   template<rep_type Rep, is_coefficient_or_unit... Ts>
   class base_dimension_impl;

   template<typename... Args>
   struct base_dimension_wrapper;

   template<typename... Args>
   using base_dimension = typename base_dimension_wrapper<Args...>::type;


   namespace symbols{
      struct sqrt2 : public symbol{
         static constexpr double value = 1.41421356237;
      };

      struct pi : public symbol{
         static constexpr double value = 3.1415;
      };
   }

   template<is_ratio T>
   constexpr double ratio_value()
   {
      return static_cast<double>(T::num) / static_cast<double>(T::den);
   }

}

#endif //STATIC_DIMENSION_BASE_SIGN_H
