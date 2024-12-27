#ifndef DIMENSION_TUPLE_HANDLING_H
#define DIMENSION_TUPLE_HANDLING_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction, std::conditional
#include <utility> // For std::declval
#include "Quantity.h"

#include "StringLiteral.h"

namespace Dimension
{
   /// @brief Check if two units are of the same dimension
   template<typename T, typename U>
   struct is_same_dim;

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename T, typename U>
   requires OneIsQuantity<T, U>
   struct is_same_dim<T, U> : std::integral_constant<
      bool,
      false
   > {};

   template<typename T, typename U>
   requires BothAreQuantity<T, U>
   struct is_same_dim<T, U> : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<typename T::unit::Dim>::type, typename std::remove_cv<typename U::unit::Dim>::type>::value &&
      (T::unit::ID == U::unit::ID)
   > {};

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename T, typename U>
   requires NeitherIsQuantity<T, U>
   struct is_same_dim<T, U> : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<typename T::Dim>::type, typename std::remove_cv<typename U::Dim>::type>::value &&
      (T::ID == U::ID)
   > {};

   /// @brief Check if two units are of the same dimension
   template<typename Dim, typename U>
   struct is_dim;

   template<typename Dim, typename U>
   requires (is_quantity_v<U>)
   struct is_dim<Dim, U> : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<Dim>::type, typename std::remove_cv<typename U::unit::Dim>::type>::value
   > {};


   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename Dim, typename U>
   requires (!is_quantity_v<U>)
   struct is_dim<Dim, U> : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<Dim>::type, typename std::remove_cv<typename U::Dim>::type>::value
   > {};

   // TODO: add a requirement that this only works for `Quantity` types
   /// @brief Check if two units are of the same dimension
   template<typename T, typename U>
   struct is_same_quantity_dim_diff_unit;

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename T, typename U>
   requires (!(is_quantity_v<T> && is_quantity_v<U>))
   struct is_same_quantity_dim_diff_unit<T, U> : std::false_type {};

   template<typename T, typename U>
   requires (is_quantity_v<T> && is_quantity_v<U>)
   struct is_same_quantity_dim_diff_unit<T, U> : std::integral_constant<
      bool,
      //is_quantity_v<T> && is_quantity_v<U> &&
      !std::is_same<typename std::remove_cv<T>::type, typename std::remove_cv<U>::type>::value &&
      std::is_same<typename std::remove_cv<typename T::unit::Dim>::type, typename std::remove_cv<typename U::unit::Dim>::type>::value &&
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

   // 1. Helper to slice a tuple based on an index_sequence
   template <typename Tuple, typename Indices>
   struct tuple_slice_impl;

   // Partial specialization for std::index_sequence
   template <typename Tuple, std::size_t... Indices>
   struct tuple_slice_impl<Tuple, std::index_sequence<Indices...>> {
      using type = std::tuple<std::tuple_element_t<Indices, Tuple>...>;
   };


   // 2. Minimal Helper to Offset an index_sequence by a certain value
   template <std::size_t Offset, typename Indices>
   struct offset_index_sequence;

   template <std::size_t Offset, std::size_t... Is>
   struct offset_index_sequence<Offset, std::index_sequence<Is...>> {
      using type = std::index_sequence<(Offset + Is)...>;
   };

   // 3. Struct to extract the first N types from a tuple
   template <std::size_t N, typename Tuple>
   struct first_N_types {
      static_assert(N <= std::tuple_size_v<Tuple>, "N is larger than the tuple size");

      using indices = std::make_index_sequence<N>;
      using type = typename tuple_slice_impl<Tuple, indices>::type;
   };

   // 4. Struct to extract the last N types from a tuple
   template <std::size_t N, typename Tuple>
   struct last_N_types {
      static constexpr std::size_t M = std::tuple_size_v<Tuple>;
      static_assert(N <= M, "N is larger than the tuple size");

      // Compute the starting index (M - N)
      static constexpr std::size_t Start = M - N;

      // Generate an index sequence starting from (M - N)
      using indices = typename offset_index_sequence<Start, std::make_index_sequence<N>>::type;

      // Slice the tuple using the generated indices
      using type = typename tuple_slice_impl<Tuple, indices>::type;
   };


   template <std::size_t N, typename Tuple>
   struct rest_types {
      static_assert(N <= std::tuple_size_v<Tuple>, "N is larger than the tuple size");

      // Calculate the number of remaining types
      static constexpr std::size_t remaining = std::tuple_size_v<Tuple> - N;

      using indices = std::make_index_sequence<remaining>;
      using shifted_indices = typename offset_index_sequence<N, indices>::type;
      using type = typename tuple_slice_impl<Tuple, shifted_indices>::type;
   };

   template <std::size_t N, typename Tuple>
   struct tuple_slice {
      static_assert(N <= std::tuple_size_v<Tuple>, "N is larger than the tuple size");

      using first = first_N_types<N, Tuple>::type;
      using last = rest_types<N, Tuple>::type;
   };


   // =====================================================
   // From this point, reconsider which pieces are actually necessary
   // =====================================================

   template <typename Tuple>
   struct map_dim_tuple;

   template <typename... Ts>
   struct map_dim_tuple<std::tuple<Ts...>> {
      using type = std::tuple<typename type_from_quantity_or_delta<Ts>::type::Dim...>;
   };

   // Helper to check if a type is in a tuple
   template <typename T, typename Tuple>
   struct tuple_contains;

   template <typename T>
   struct tuple_contains<T, std::tuple<>> : std::false_type {};

   template <typename T, typename U, typename... Us>
   struct tuple_contains<T, std::tuple<U, Us...>> 
      : std::conditional_t<std::is_same_v<T, U>, std::true_type, tuple_contains<T, std::tuple<Us...>>> {};

   // Helper to append a type to a tuple if it's not already present
   template <typename Tuple, typename T>
   struct append_unique;

   template <typename... Ts, typename T>
   struct append_unique<std::tuple<Ts...>, T> {
      using type = std::conditional_t<
         tuple_contains<T, std::tuple<Ts...>>::value,
         std::tuple<Ts...>,
         std::tuple<Ts..., T>
      >;
   };

   // Remove duplicates by iterating through the tuple
   template <typename Tuple>
   struct unique_tuple;

   template <>
   struct unique_tuple<std::tuple<>> {
      using type = std::tuple<>;
   };

   template <typename Head, typename... Tail>
   struct unique_tuple<std::tuple<Head, Tail...>> {
   private:
      using tail_unique = typename unique_tuple<std::tuple<Tail...>>::type;
   public:
      using type = typename append_unique<tail_unique, Head>::type;
   };

   template <typename Tuple>
   struct get_dim_tuple {
   private:
      using dim_mapped = typename map_dim_tuple<Tuple>::type;
      using unique_dims = typename unique_tuple<dim_mapped>::type;
   public:
      using type = unique_dims;
   };

   template<typename T>
   struct is_absolute : std::integral_constant<bool, true> {};

   template<typename Tuple>
   struct is_single_quantity;

   template<typename Tuple>
   requires (std::tuple_size_v<Tuple> != 1)
   struct is_single_quantity<Tuple> : std::integral_constant<
      bool,
      false
   > {};

   template<typename Tuple>
   requires (std::tuple_size_v<Tuple> == 1)
   struct is_single_quantity<Tuple> : std::integral_constant<
      bool,
      is_quantity_v<std::tuple_element_t<0, Tuple>>
   > {};

   template<typename Tuple>
   struct is_single_non_absolute_quantity;

   template<typename Tuple>
   requires (is_single_quantity<Tuple>::value)
   struct is_single_non_absolute_quantity<Tuple> : std::integral_constant<
      bool,
      !is_absolute<type_from_quantity_or_delta_t<std::tuple_element_t<0, Tuple>>>::value
   > {};

   template<typename Tuple>
   requires (!is_single_quantity<Tuple>::value)
   struct is_single_non_absolute_quantity<Tuple> : std::integral_constant<
      bool,
      false
   > {};

   template<typename Tuple>
   struct make_tuple_of_single_absolute_quantity;

   template<typename Tuple>
   requires (is_single_non_absolute_quantity<Tuple>::value)
   struct make_tuple_of_single_absolute_quantity<Tuple>
   {
      using primary = type_from_quantity_or_delta_t<std::tuple_element_t<0, Tuple>>::Primary;
      using type = std::tuple<Quantity<primary>>;
      static constexpr bool value = true;
   };

   template<typename Tuple>
   requires (!is_single_non_absolute_quantity<Tuple>::value)
   struct make_tuple_of_single_absolute_quantity<Tuple>
   {
      using type = void;
      static constexpr bool value = false;
   };

   template<typename Tuple>
   struct has_non_absolute_quantity : std::false_type {};

   // Partial specialization for std::tuple
   template<typename... Ts>
   struct has_non_absolute_quantity<std::tuple<Ts...>> 
      : std::bool_constant<( (is_quantity_v<Ts> && !is_absolute<type_from_quantity_or_delta_t<Ts>>::value ) || ... )> {};

   // Helper variable template for easier usage
   template<typename Tuple>
   inline constexpr bool has_non_absolute_quantity_v = has_non_absolute_quantity<Tuple>::value;

} // end Dimension

#endif // DIMENSION_TUPLE_HANDLING_H
