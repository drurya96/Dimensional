#ifndef DIMENSION_UTILITIES_H
#define DIMENSION_UTILITIES_H

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

   struct Inverse;

   template<typename...>
   struct is_inverse_dimension : std::true_type {};

   template<>
   struct is_inverse_dimension<> : std::false_type {};

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

   // Repeat for negative number of occurences
   // This should NEVER occur, but the compiler must be able to resolve all types
   //    that may be produced by conditional_t
   // Consider a different approach, but this is sufficient for now
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

   // Helper function to convert tuple types to BaseDimension
   template<typename... Ts, std::size_t... Is, typename ... Args>
   auto TupleToBaseDimension(const std::tuple<Ts...>&, std::index_sequence<Is...>, const BaseDimension<Args...>& obj) {

      std::vector<BaseUnit<>*> newNumList = obj.numList;
      std::vector<BaseUnit<>*> newDenList = obj.denList;
      double newValue = obj.value;

      // TODO: Consider ways to improve effeciency of this section

      for (auto numIter = newNumList.begin(); numIter != newNumList.end();)
      {
         bool erased = false; // Flag to track if an element was erased

         for (auto denIter = newDenList.begin(); denIter != newDenList.end(); ++denIter)
         {
            if ((*numIter)->GetDimName() == (*denIter)->GetDimName())
            {
               if (*numIter != *denIter)
               {
                  newValue = (*numIter)->conversions[(*denIter)->GetUnitName()](newValue);
               }
               // Remove the current items from both lists
               numIter = newNumList.erase(numIter);
               denIter = newDenList.erase(denIter);
               erased = true; // Set erased flag to true
               // Break out of the inner loop
               break;
            }
         }

         // Check if an element was erased and if we need to break out of the outer loop
         if (!erased)
         {
            ++numIter;
         }

         if (numIter == newNumList.end())
         {
            break;
         }
      }


      return BaseDimension<Ts...>(newValue, newNumList, newDenList);
   }
}

#endif // DIMENSION_UTILITIES_H