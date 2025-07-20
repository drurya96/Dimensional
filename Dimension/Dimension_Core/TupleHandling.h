#ifndef DIMENSION_TUPLE_HANDLING_H
#define DIMENSION_TUPLE_HANDLING_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction, std::conditional
#include <utility> // For std::declval

#include "StringLiteral.h"

namespace dimension
{
   class base_dimension_marker;

   template<typename T>
   concept is_base_dimension = std::is_base_of_v<base_dimension_marker, T>;

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename T, typename U>
   // TODO: Consider a requirement that T and U are dimensions
   struct is_same_dim : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<typename T::Dim>::type, typename std::remove_cv<typename U::Dim>::type>::value &&
      (T::ID == U::ID)
   > {};

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename Dim, typename U>
   struct is_dim : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<Dim>::type, typename std::remove_cv<typename U::Dim>::type>::value
   > {};

   /// @brief Convenience alias for retrieving the type of a tuple of types
   /// @tparam Ts Parameter pack to types to concatenate
   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

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

   // TODO: Important: Get rid of this, this is just to more easily find/replace some things.
   template<typename T>
   struct extract_type
   {
      using type = T::unit;
   };

   template <typename T>
   using extract_type_t = typename extract_type<T>::type;


   template <typename Tuple>
   struct map_dim_tuple;

   template <typename... Ts>
   struct map_dim_tuple<std::tuple<Ts...>> {
      using type = std::tuple<typename extract_type<Ts>::type::Dim...>;
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

} // end Dimension

#endif // DIMENSION_TUPLE_HANDLING_H
