#ifndef STATIC_DIMENSION_UTILITIES_H
#define STATIC_DIMENSION_UTILITIES_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction, std::conditional, std::enable_if
#include <utility> // For std::declval
#include <cmath> // For std::isnan

namespace StaticDimension
{
   // Forward declarations
   template<typename NumTuple, typename DenTuple>
   class BaseDimension;

   template<typename Unit>
   class BaseUnit;

   /// @brief Convenience alias for retrieving the type of a tuple of types
   /// @tparam Ts Parameter pack to types to concatenate
   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

   /// @brief Check if two units are of the same dimension
   template<typename T, typename U>
   struct is_same_dim;

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename T, typename U>
   struct is_same_dim : std::is_same<typename std::remove_cv<typename T::Dim>::type, typename std::remove_cv<typename U::Dim>::type> {};

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   template<typename Dim, typename Tuple>
   struct has_same_dim;

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename Dim, typename... Us>
   struct has_same_dim<Dim, std::tuple<Us...>> : std::disjunction<is_same_dim<Dim, Us>...> {};

   /// @brief Struct to remove one instance of a type from a tuple of types
   template<typename T, typename Tuple>
   struct RemoveOneInstance;

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Specialization for an empty tuple
   /// @typedef type An empty tuple
   template<typename T>
   struct RemoveOneInstance<T, std::tuple<>> {
      using type = std::tuple<>;
   };

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Main specialization
   /// @tparam T The type to remove
   /// @tparam Head the first item of the tuple
   /// @tparam Tail the remaining items in the tuple
   /// @typedef type A tuple type matching the input tuple type, except the first instance of T is removed
   template<typename T, typename Head, typename... Tail>
   struct RemoveOneInstance<T, std::tuple<Head, Tail...>> {
      using type = std::conditional_t<is_same_dim<T, Head>::value,
         std::tuple<Tail...>,
         decltype(tuple_cat_t<std::tuple<Head>, typename RemoveOneInstance<T, std::tuple<Tail...>>::type>())>;
   };

   /// @brief Find the difference between two type-tuples
   template<typename T, typename ...>
   struct tuple_diff;

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty minuend tuple
   /// @typedef type Empty tuple
   template<typename ... subtrahendTypes>
   struct tuple_diff<std::tuple<>, std::tuple<subtrahendTypes...>>
   {
      using type = std::tuple<>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty subtrahend tuple
   /// @typedef type The entire minuend tuple
   template<typename... restMinuendTypes>
   struct tuple_diff<std::tuple<restMinuendTypes...>, std::tuple<>> {
      using type = std::tuple<restMinuendTypes...>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Computes the multiset difference between two tuples, where the multiset
   ///    difference of the minuend and subtrahend contains all elements from the
   ///    minuend except those that also appear in the subtrahend.The multiplicity 
   ///    of elements is taken into account.
   /// @tparam T The first type in the minuend tuple
   /// @tparam restMinuendTypes The remaining types in the minuend tuple
   /// @tparam subtrahendTypes The types in the subtrahend tuple
   /// @typedef type A tuple of types contain the multiset different between the minuend and subtrahend
   template<typename T, typename... restMinuendTypes, typename... subtrahendTypes>
   struct tuple_diff<std::tuple<T, restMinuendTypes...>, std::tuple<subtrahendTypes...>> {
      using type = std::conditional_t<
         has_same_dim<T, std::tuple<subtrahendTypes...>>::value,
         typename tuple_diff<std::tuple<restMinuendTypes...>, typename RemoveOneInstance<T, std::tuple<subtrahendTypes...>>::type>::type,
         tuple_cat_t<std::tuple<T>, typename tuple_diff<std::tuple<restMinuendTypes...>, std::tuple<subtrahendTypes...>>::type>
      >;
   };

   /// @brief Struct to simplify units by cancelling out as necessary
   template<typename NumTypes1, typename NumTypes2, typename DenTypes1, typename DenTypes2>
   struct UnitSimplifier;

   /// @brief Struct to simplify units by cancelling out as necessary
   /// @tparam NumTypes1... The types in the first group of numerator types
   /// @tparam NumTypes2... The types in the second group of numerator types
   /// @tparam DenTypes1... The types in the first group of denominator types
   /// @tparam DenTypes2... The types in the second group of denominator types
   /// @typedef newNum The simpliefied numerator types
   /// @typedef newDen The simpliefied denominator types
   /// @typedef dimType A BaseDimension templated on the simplified types
   template<typename ... NumTypes1, typename ... NumTypes2, typename ... DenTypes1, typename ... DenTypes2>
   struct UnitSimplifier<std::tuple<NumTypes1...>, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>, std::tuple<DenTypes2...>>
   {
   private:
      using remainingNum1 = tuple_diff<std::tuple<NumTypes1...>, std::tuple<DenTypes2...>>;
      using remainingNum2 = tuple_diff<std::tuple<NumTypes2...>, std::tuple<DenTypes1...>>;

      using remainingDen1 = tuple_diff<std::tuple<DenTypes1...>, std::tuple<NumTypes2...>>;
      using remainingDen2 = tuple_diff<std::tuple<DenTypes2...>, std::tuple<NumTypes1...>>;
   public:
      using newNum = tuple_cat_t<typename remainingNum1::type, typename remainingNum2::type>;
      using newDen = tuple_cat_t<typename remainingDen1::type, typename remainingDen2::type>;
      using dimType = BaseDimension<newNum, newDen>;
   };

   struct NullUnit
   {
      using Dim = void;
      using Primary = void;
   };



   /// @brief Struct to remove one instance of a type from a tuple of types
   template<typename T, typename Tuple>
   struct get_first_match;

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Specialization for an empty tuple
   /// @typedef type An empty tuple
   template<typename T>
   struct get_first_match<T, std::tuple<>> {
      using type = NullUnit;
   };

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Main specialization
   /// @tparam T The type to remove
   /// @tparam Head the first item of the tuple
   /// @tparam Tail the remaining items in the tuple
   /// @typedef type A tuple type matching the input tuple type, except the first instance of T is removed
   template<typename T, typename Head, typename... Tail>
   struct get_first_match<T, std::tuple<Head, Tail...>> {
      using type = std::conditional_t<is_same_dim<T, Head>::value,
         Head,
         typename get_first_match<T, std::tuple<Tail...>>::type>;
   };














   /// @brief Unit cancellation implementation
   /// @details recursive base-case
   template<bool inverse = false, int index = 0, typename RealTupType, typename ... IncomingTypes>
   typename std::enable_if < index == sizeof...(IncomingTypes), void>::type
      CancelUnitsImplNEW(const std::tuple<IncomingTypes...>& IncomingTup, double& value)
   {
      return;
   }

   /// @brief Unit cancellation implementation
   /// @details Updates the given value when units are cancelled,
   ///    and updates the given unit tuple when a unit is not cancelled.
   /// @tparam inverse bool indicating whether the item is in numerator (false) or denominator (true)
   /// @tparam index The tuple index to attempt cancelling
   /// @tparam IncomingTypes... The types to attempt cancelling
   /// @tparam RealTypes... The types to update as needed
   /// @param[in] IncomingTup The reference tuple to cancel or update form
   /// @param[in,out] RealTup The tuple to update
   /// @param[in,out] value The value to update when cancelling units
   template<bool inverse = false, int index = 0, typename RealTupType, typename ... IncomingTypes>
   typename std::enable_if < index < sizeof...(IncomingTypes), void>::type
      CancelUnitsImplNEW(const std::tuple<IncomingTypes...>& IncomingTup, double& value)
   {
      using currentType = std::tuple_element_t<index, std::tuple<IncomingTypes...>>;
      if constexpr (has_same_dim<currentType, RealTupType>::value)
      {
         // Don't cancel

         if constexpr (inverse)
         {
            value /= ConvertDouble<currentType, get_first_match<currentType, RealTupType>::type>(1.0);
         }
         else
         {
            value *= ConvertDouble<currentType, get_first_match<currentType, RealTupType>::type>(1.0);
         }


         CancelUnitsImplNEW<inverse, index + 1, RemoveOneInstance<currentType, RealTupType>::type>(IncomingTup, value);
      }
      else
      {
         if constexpr (inverse)
         {
            value /= ConvertDouble<currentType, currentType::Primary>(1.0);
         }
         else
         {
            value *= ConvertDouble<currentType, currentType::Primary>(1.0);
         }

         CancelUnitsImplNEW<inverse, index + 1, RealTupType>(IncomingTup, value);
      }
   }

   /// @brief Perform unit cancellation
   /// @tparam NumTypes... Incoming numerator types
   /// @tparam DenTypes... Incoming denominator types
   /// @tparam RealNumTypes... Numerator types to update
   /// @tparam RealDenTypes... Denominator types to udpate
   /// @param[in] NumTup Incoming numerator tuple
   /// @param[in] DenTup Incoming denominator tuple
   /// @param[in,out] RealNumTup Numerator tuple to update
   /// @param[in,out] RealDenTup Denominator tuple to update
   /// @param[in,out] value Value to update
   template<typename ... NumTypes, typename ... DenTypes, typename ... RealNumTypes, typename ... RealDenTypes>
   void CancelUnitsNew(const std::tuple<NumTypes...>& NumTup, const std::tuple<DenTypes...>& DenTup, std::tuple<RealNumTypes...>& RealNumTup, std::tuple<RealDenTypes...>& RealDenTup, double& value)
   {
      CancelUnitsImplNEW<false, 0, std::tuple<RealNumTypes...>>(NumTup, value);
      CancelUnitsImplNEW<true, 0, std::tuple<RealDenTypes...>>(DenTup, value);
   }



   /// @brief Convert the given value when units are cancelled out
   /// @details Recursive base-case
   template <size_t I = 0, bool inverse = false, typename toTuple, typename... Ts>
   typename std::enable_if<I == sizeof...(Ts), void>::type
      GetConvertedDouble(const std::tuple<Ts...>& tup, double& value)
   {
      return;
   }

   /// @brief Convert the given value when units are cancelled out
   /// @tparam I The current index of both tuples
   /// @tparam inverse Whether the conversion is inverted (i.e. in the denominator)
   /// @tparam toTuple The tuple type to convert to
   /// @tparam Ts... The types within the tuples to convert from
   /// @param fromTup[in] Tuple of types to convert from
   /// @param value[in,out] Reference of a value to update
   template <size_t I = 0, bool inverse = false, typename toTuple, typename... Ts>
   typename std::enable_if<(I < sizeof...(Ts)), void>::type
      GetConvertedDouble(const std::tuple<Ts...>& fromTup, double& value)
   {
      if constexpr(inverse)
      {
         value /= ConvertDouble<std::tuple_element_t<I, std::tuple<Ts...>>, std::tuple_element_t<I, toTuple>>(1.0);        
      }
      else
      {
         value *= ConvertDouble<std::tuple_element_t<I, std::tuple<Ts...>>, std::tuple_element_t<I, toTuple>>(1.0);
      }
      GetConvertedDouble<I + 1, inverse, toTuple>(fromTup, value);
   }

}
#endif // STATIC_DIMENSION_UTILITIES_H
