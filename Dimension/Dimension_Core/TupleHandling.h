#ifndef DIMENSION_TUPLE_HANDLING_H
#define DIMENSION_TUPLE_HANDLING_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction, std::conditional
#include <utility> // For std::declval

#include "StringLiteral.h"

namespace Dimension
{

   /// @brief A type-trait with void Dim and Primary, only used to satisfy a metaprogramming condition
   struct NullUnit
   {
      using Dim = void;
      using Primary = void;
      static constexpr int ID = 0;
   };

   template<template<typename, typename> typename Compare>
   concept Comparable = requires
   {
      { Compare<NullUnit, NullUnit>::value } -> std::convertible_to<bool>;
   };

   /// @brief Check if two units are of the same dimension
   template<typename T, typename U>
   struct is_same_dim;

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename T, typename U>
   struct is_same_dim : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<typename T::Dim>::type, typename std::remove_cv<typename U::Dim>::type>::value &&
      (T::ID == U::ID)
   > {};

   /// @brief Convenience alias for retrieving the type of a tuple of types
   /// @tparam Ts Parameter pack to types to concatenate
   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

   /// @brief Struct to remove one instance of a type from a tuple of types
   template<template<typename, typename> typename Compare, typename T, typename Tuple>
   requires Comparable<Compare>
   struct RemoveOneInstance;

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Specialization for an empty tuple
   /// @typedef type An empty tuple
   template<template<typename, typename> typename Compare, typename T>
   requires Comparable<Compare>
   struct RemoveOneInstance<Compare, T, std::tuple<>>
   {
      using type = std::tuple<>;
   };

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Main specialization
   /// @tparam T The type to remove
   /// @tparam Head the first item of the tuple
   /// @tparam Tail the remaining items in the tuple
   /// @typedef type A tuple type matching the input tuple type, except the first instance of T is removed
   template<template<typename, typename> typename Compare, typename T, typename Head, typename... Tail>
   requires Comparable<Compare>
   struct RemoveOneInstance<Compare, T, std::tuple<Head, Tail...>>
   {
      using type = std::conditional_t < Compare<T, Head>::value,
         std::tuple<Tail...>,
         tuple_cat_t<std::tuple<Head>, typename RemoveOneInstance<Compare, T, std::tuple<Tail...>>::type>
      >;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Computes the multiset difference between two tuples, where the multiset
   ///    difference of the minuend and subtrahend contains all elements from the
   ///    minuend except those that also appear in the subtrahend.The multiplicity 
   ///    of elements is taken into account.
   template<template<typename, typename> typename Compare, typename T, typename U, typename Enable = void>
   struct tuple_diff;

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty minuend tuple
   /// @typedef type Empty tuple
   template<template<typename, typename> typename Compare>
   struct tuple_diff<Compare, std::tuple<>, std::tuple<>>
   {
      using type = std::tuple<>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty minuend tuple
   /// @typedef type Empty tuple
   template<template<typename, typename> typename Compare, typename ... subtrahendTypes>
   struct tuple_diff<Compare, std::tuple<>, std::tuple<subtrahendTypes...>>
   {
      using type = std::tuple<>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty subtrahend tuple
   /// @typedef type The entire minuend tuple
   template<template<typename, typename> typename Compare, typename... restMinuendTypes>
   struct tuple_diff<Compare, std::tuple<restMinuendTypes...>, std::tuple<>>
   {
      using type = std::tuple<restMinuendTypes...>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Diff when subtrahend is non-empty and Compare resolves to True
   ///   Removes one instance of T from subtrahendTypes
   /// @tparam Compare A comparison type-trait used to determine if T is comparable to a tuple of subtrahendTypes
   /// @tparam T The first type in the minuend tuple
   /// @tparam restMinuendTypes The remaining types in the minuend tuple
   /// @tparam subtrahendTypes The types in the subtrahend tuple
   /// @typedef type A tuple of types contain the multiset different between the minuend and subtrahend
   template<template<typename, typename> typename Compare, typename T, typename... restMinuendTypes, typename... subtrahendTypes>
   requires (
      sizeof...(subtrahendTypes) > 0 &&
      Compare<T, std::tuple<subtrahendTypes...>>::value
   )
   struct tuple_diff<Compare, std::tuple<T, restMinuendTypes...>, std::tuple<subtrahendTypes...>>
   {
      using type = typename tuple_diff
      <
         Compare, 
         std::tuple<restMinuendTypes...>, 
         typename RemoveOneInstance<is_same_dim, T, std::tuple<subtrahendTypes...>>::type
      >::type;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Diff when subtrahend is non-empty and Compare resolves to False
   ///   Continues comparison with the next type in the restMinuendTypes set
   /// @tparam Compare A comparison type-trait used to determine if T is comparable to a tuple of subtrahendTypes
   /// @tparam T The first type in the minuend tuple
   /// @tparam restMinuendTypes The remaining types in the minuend tuple
   /// @tparam subtrahendTypes The types in the subtrahend tuple
   /// @typedef type A tuple of types contain the multiset different between the minuend and subtrahend
   template<template<typename, typename> typename Compare, typename T, typename... restMinuendTypes, typename... subtrahendTypes>
   requires (
      sizeof...(subtrahendTypes) > 0 &&
      !Compare<T, std::tuple<subtrahendTypes...>>::value
   )
   struct tuple_diff<Compare, std::tuple<T, restMinuendTypes...>, std::tuple<subtrahendTypes...>>
   {
      using type = tuple_cat_t<std::tuple<T>, typename tuple_diff<Compare, std::tuple<restMinuendTypes...>, std::tuple<subtrahendTypes...>>::type>;
   };

   /// @brief Swap two indecies of a tuple type
   /// @tparam Tuple Tuple type to swap
   /// @tparam I First index to swap
   /// @tparam J Second index to swap
   template <typename Tuple, std::size_t I, std::size_t J>
   struct tuple_swap
   {
   private:
      template <std::size_t Index, typename T>
      using Replace = std::conditional_t<
         Index == I, 
         std::tuple_element_t<J, Tuple>, 
         std::conditional_t<
            Index == J, 
            std::tuple_element_t<I, Tuple>, 
            T
         >
      >;

      // Recursively rebuild the tuple with the swapped types
      template <std::size_t... Indices>
      static auto rebuild_tuple(std::index_sequence<Indices...>) -> std::tuple<Replace<Indices, std::tuple_element_t<Indices, Tuple>>...>;

   public:
      // The swapped tuple type
      using type = decltype(rebuild_tuple(std::make_index_sequence<std::tuple_size_v<Tuple>>{}));
   };

   /// @brief Sort tuple type based on qualified name
   /// @tparam Tuple Tuple type to sort
   /// @tparam N Current index to evaluate
   template <typename Tuple, std::size_t N = std::tuple_size_v<Tuple>>
   struct tuple_bubble_sort 
   {
   private:
      // Perform one pass of bubble sort
      template <std::size_t Index = 0, typename CurrentTuple = Tuple>
      struct one_pass 
      {

         using ThisElement = std::tuple_element_t<Index, CurrentTuple>;
         using NextElement = std::tuple_element_t<Index + 1, CurrentTuple>;

         using type = std::conditional_t<
            (Index < N - 1) && !(ThisElement::qualifiedName < NextElement::qualifiedName),
            typename tuple_swap<CurrentTuple, Index, Index + 1>::type, 
            CurrentTuple>;

         using next_type = typename one_pass<Index + 1, type>::type;
      };

      // Base case for one_pass recursion
      template <typename CurrentTuple>
      struct one_pass<N - 1, CurrentTuple>
      {
         using type = CurrentTuple;
      };

   public:
      // Apply bubble sort recursively
      using type = typename tuple_bubble_sort<typename one_pass<0, Tuple>::type, N - 1>::type;
   };

   // Base case for bubble sort recursion
   template <typename Tuple>
   struct tuple_bubble_sort<Tuple, 1>
   {
      using type = Tuple;
   };

   // Bubble sort empty tuple
   template <>
   struct tuple_bubble_sort<std::tuple<>, 0>
   {
      using type = std::tuple<>;
   };

} // end Dimension

#endif // DIMENSION_TUPLE_HANDLING_H
