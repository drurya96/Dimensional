#ifndef DIMENSION_UTILITIES_H
#define DIMENSION_UTILITIES_H

#include <vector>
#include <type_traits>
#include <tuple>

namespace Dimension
{
   // Forward declare BaseDimension
   template <typename...>
   class BaseDimension;

   // Forward declare BaseUnit
   template <typename...>
   class BaseUnit;


   // Define a variadic template for myClass types
   template<typename ... T_Units>
   struct myUnitTypes {};

   class Inverse
   {
      Inverse();
   };

   template<typename...>
   struct is_inverse_dimension : std::true_type {};

   template<>
   struct is_inverse_dimension<> : std::false_type {};


   // ==========================================
   // Helpers

   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

   // Primary template declaration
   template <int N, typename T, typename = void>
   struct Repeat;

   // Recursive case: define type as a tuple containing N copies of T
   template <int N, typename T>
   struct Repeat<N, T, std::enable_if_t<(N > 0)>> {
      using type = decltype(std::tuple_cat(std::tuple<T>(), typename Repeat<N - 1, T>::type()));
   };

   // Recursive case: define type as a tuple containing N copies of T
   template <int N, typename T>
   struct Repeat<N, T, std::enable_if_t<(N < 0)>> {
      using type = std::tuple<>;
   };

   // Base case: when N is 0, define type as an empty tuple
   template <typename T>
   struct Repeat<0, T> {
      using type = std::tuple<>;
   };

   // Base case: count is 0
   template<typename T>
   constexpr size_t count_type() {
      return 0;
   }

   // Recursive case: if first type matches, increment count
   template<typename T, typename First, typename... Rest>
   constexpr size_t count_type() {
      return std::is_same_v<T, First> +count_type<T, Rest...>();
   }

   template<typename T1, typename T2, typename...Ts>
   using remove_t = tuple_cat_t<
      typename std::conditional<
      std::is_same<T1, Ts>::value || std::is_same<T2, Ts>::value,
      std::tuple<>,
      std::tuple<Ts>
      >::type...
   >;


   // Helper function to concatenate vectors in a single line
   inline std::vector<BaseUnit<>*> ConcatenateUnitVectors(std::vector<BaseUnit<>*> a, std::vector<BaseUnit<>*> b)
   {
      std::vector<BaseUnit<>*> result;
      result.reserve(a.size() + b.size());

      result.insert(result.end(), a.begin(), a.end());
      result.insert(result.end(), b.begin(), b.end());
      return result;
   }




   // Check if two packs are equal
   template<typename...>
   struct arePacksEqual : std::true_type {};

   template<typename T1, typename... Ts1, typename T2, typename... Ts2>
   struct arePacksEqual<std::tuple<T1, Ts1...>, std::tuple<T2, Ts2...>> : std::conditional_t<std::is_same_v<T1, T2>, arePacksEqual<std::tuple<Ts1...>, std::tuple<Ts2...>>, std::false_type> {};

   // Check if two flags are equal
   template<typename... T_Flag1, typename... T_Flag2>
   constexpr bool areFlagsEqual(const myUnitTypes<T_Flag1...>&, const myUnitTypes<T_Flag2...>&) {
      return arePacksEqual<std::tuple<T_Flag1...>, std::tuple<T_Flag2...>>::value;
   }

   // Extract flags from myUnitTypes
   template<typename ... T_Classes>
   constexpr auto extractFlags(const myUnitTypes<T_Classes...>&) {
      return std::tuple<T_Classes...>{};
   }

   // Define a helper type trait to deduce the return type of invertDimension for each type
   template <typename T>
   struct InvertReturnType {
      using type = decltype(invertDimension(std::declval<T>()));
   };

   // Specialization for when invertDimension is not defined for a type
   template <>
   struct InvertReturnType<void> {
      using type = void;
   };



   template<template<typename...> class T, typename ... Args>
   typename std::enable_if<is_inverse_dimension<Args...>::value, T<>>::type
      invertDimension(const T<Args...>& obj) {
      return T<>();
   }

   template<template<typename...> class T, typename ... Args>
   typename std::enable_if<!is_inverse_dimension<Args...>::value, T<Inverse>>::type
      invertDimension(const T<Args...>& obj) {
      return T<Inverse>();
   }


}




#endif // DIMENSION_UTILITIES_H