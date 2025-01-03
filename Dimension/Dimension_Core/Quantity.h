#ifndef DIMENSION_QUANTITY_H
#define DIMENSION_QUANTITY_H

namespace Dimension
{

   /// @brief A type-trait with void Dim and Primary, only used to satisfy a metaprogramming condition
   struct NullUnit
   {
      using Dim = void;
      using Primary = void;
      static constexpr int ID = 0;
   };

   template<typename Unit>
   // Need a requirement that Quantity is exactly one unit
   struct Quantity
   {
      using unit = Unit;

      // Unsure if this is right...
      using NumTuple = std::tuple<Quantity<unit>>;
      using DenTuple = std::tuple<>;
   };

   // Primary template: Defaults to false
   template<typename T>
   struct is_quantity : std::false_type {};

   // Specialization for Quantity
   template<typename Unit>
   struct is_quantity<Quantity<Unit>> : std::true_type {};

   // Helper variable template
   template<typename T>
   constexpr bool is_quantity_v = is_quantity<T>::value;


   template<typename T1, typename T2>
   constexpr bool both_quantity()
   {
      if constexpr (std::is_same_v<T1, NullUnit> && std::is_same_v<T2, NullUnit>)
      {
         return false;
      }
      else if constexpr (std::is_same_v<T1, NullUnit>)
      {
         return is_quantity_v<T2>;
      }
      else if constexpr (std::is_same_v<T2, NullUnit>)
      {
         return is_quantity_v<T1>;
      }
      else
      {
         return is_quantity_v<T1>;
      }

   return true;
   }

   template<typename T>
   struct type_from_quantity_or_delta;

   template<typename T>
   requires (is_quantity_v<T>)
   struct type_from_quantity_or_delta<T>
   {
      using type = T::unit;
   };

   template<typename T>
   requires (!is_quantity_v<T>)
   struct type_from_quantity_or_delta<T>
   {
      using type = T;
   };

   // Convenience alias template
   template <typename T>
   using type_from_quantity_or_delta_t = typename type_from_quantity_or_delta<T>::type;

   // Primary template declaration
   template <typename Tuple>
   struct make_all_delta;

   // Partial specialization for std::tuple
   template <typename... Ts>
   struct make_all_delta<std::tuple<Ts...>> {
      using type = std::tuple<type_from_quantity_or_delta_t<Ts>...>;
   };

   // Convenience alias template
   template <typename Tuple>
   using make_all_delta_t = typename make_all_delta<Tuple>::type;

   template<template<typename, typename> typename Compare>
   concept Comparable = requires
   {
      { Compare<NullUnit, NullUnit>::value } -> std::convertible_to<bool>;
   };

   template<typename T, typename U>
   concept OneIsQuantity = (is_quantity_v<T> && !is_quantity_v<U>) || (!is_quantity_v<T> && is_quantity_v<U>);

   template<typename T, typename U>
   concept BothAreQuantity = is_quantity_v<T> && is_quantity_v<U>;

   template<typename T, typename U>
   concept NeitherIsQuantity = !is_quantity_v<T> && !is_quantity_v<U>;

}

#endif // DIMENSION_QUANTITY_H