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

   /// @brief Convert the given value when units are cancelled out
   /// @details Recursive base-case
   template <size_t I = 0, bool inverse = false, typename toTuple, typename... Ts>
   typename std::enable_if<I == sizeof...(Ts), void>::type
      GetConvertedValue(const std::tuple<Ts...>& tup, double& value)
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
   GetConvertedValue(const std::tuple<Ts...>& fromTup, double& value)
   {
      if constexpr (inverse)
      {
         value /= ConvertValue<std::tuple_element_t<I, std::tuple<Ts...>>, std::tuple_element_t<I, toTuple>>(std::get<I>(fromTup)).GetValue();
      }
      else
      {
         value *= ConvertValue<std::tuple_element_t<I, std::tuple<Ts...>>, std::tuple_element_t<I, toTuple>>(std::get<I>(fromTup)).GetValue();
      }
      GetConvertedValue<I + 1, inverse, toTuple>(fromTup, value);
   }

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

   /// @brief Update the given unit tuple with valid values
   /// @details specialization when there are no more items to update
   template<int index = 0, typename T, typename ... RealTypes>
   typename std::enable_if < index == sizeof...(RealTypes), bool>::type
   UpdateRealUnits(const T& Unit, std::tuple<RealTypes...>& RealTup)
   {
      return false;
   }

   /// @brief Update the given unit tuple with valid values
   /// @tparam index The index to of the tuple to update
   /// @tparam T The type of the unit to update the value of
   /// @tparam RealTypes... The types in the tuple of units to update
   /// @param[in] Unit The unit to use when updating
   /// @param[in,out] RealTup The tuple of units to update.
   ///    Invalid values will be NaN
   template<int index = 0, typename T, typename ... RealTypes>
   typename std::enable_if<index < sizeof...(RealTypes), bool>::type
   UpdateRealUnits(const T& Unit, std::tuple<RealTypes...>& RealTup)
   {
      if constexpr (std::is_same_v<std::tuple_element_t<index, std::tuple<RealTypes...>>::Dim, T::Dim>) // TODO: This is apparently safe for empty tuple, but would like to investigate
      {
         if (std::isnan(std::get<index>(RealTup).GetValue()))
         {
            std::get<index>(RealTup).SetValue(ConvertValue<T, std::tuple_element_t<index, std::tuple<RealTypes...>>>(Unit).GetValue());
            return true;
         }
      }
      return UpdateRealUnits<index + 1>(Unit, RealTup);
   }

   /// @brief Unit cancellation implementation
   /// @details recursive base-case
   template<bool inverse = false, int index = 0, typename ... IncomingTypes, typename ... RealTypes>
   typename std::enable_if < index == sizeof...(IncomingTypes), void>::type
   CancelUnitsImpl(const std::tuple<IncomingTypes...>& IncomingTup, std::tuple<RealTypes...>& RealTup, double& value)
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
   template<bool inverse = false, int index = 0, typename ... IncomingTypes, typename ... RealTypes>
   typename std::enable_if<index < sizeof...(IncomingTypes), void>::type
   CancelUnitsImpl(const std::tuple<IncomingTypes...>& IncomingTup, std::tuple<RealTypes...>& RealTup, double& value)
   {
      if (!UpdateRealUnits<0>(std::get<index>(IncomingTup), RealTup))
      {
         // Modify value by canceling
         if constexpr (inverse)
         {
            value /= std::get<index>(IncomingTup).GetPrimary().GetValue();
         }
         else
         {
            value *= std::get<index>(IncomingTup).GetPrimary().GetValue();
         }
      }
      CancelUnitsImpl<inverse, index + 1>(IncomingTup, RealTup, value);
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
   void CancelUnits(const std::tuple<NumTypes...>& NumTup, const std::tuple<DenTypes...>& DenTup, std::tuple<RealNumTypes...>& RealNumTup, std::tuple<RealDenTypes...>& RealDenTup, double& value)
   {
      CancelUnitsImpl<false>(NumTup, RealNumTup, value);
      CancelUnitsImpl<true>(DenTup, RealDenTup, value);
   }

   /// @brief Add two units accounting for necessary scalars
   /// @tparam T The Unit type to add and update
   /// @param[in] Unit1 The first unit to add
   /// @param[in] Unit2 The second unit to add
   /// @param[out] OutUnit The unit to update results in
   /// @param[in] scalar1 Scalar value for first unit
   /// @param[in] scalar2 Scalar value for second unit
   template<typename T>
   void AddUnits(const T& Unit1, const T& Unit2, T& OutUnit, double scalar1, double scalar2)
   {
      OutUnit.SetValue((Unit1.GetValue() * scalar1) + (Unit2.GetValue() * scalar2));
   }
   
   /// @brief Add two tuples of units
   /// @details Recursive base-case
   template<int index = 0, typename ... T1, typename ... T2>
   typename std::enable_if < sizeof...(T1) == sizeof...(T2) && index == sizeof...(T1), void>::type
   AddUnitTuples(const std::tuple<T1...>& Tup1, const std::tuple<T2...>& Tup2, std::tuple<T1...>& OutTup, double scalar1 = 1.0, double scalar2 = 1.0)
   {
      return;
   }

   /// @brief Add two tuples of units
   /// @details tuples must be of the same size, and units at
   ///    corresponding positions must be convertible.
   /// @tparam index Position in tuple to add
   /// @tparam T1... Types of the first unit tuple
   /// @tparam T2... Types of the second unit tuple
   /// @param[in] Tup1 First tuple to add
   /// @param[in] Tup2 Second tuple to add
   /// @param[out] OutTup Tuple to write output to.
   ///    This will be the same type as Tup1
   /// @param[in] scalar1 Scalar values for Tup1
   /// @param[in] scalar2 Scalar values for Tup2
   template<int index = 0, typename ... T1, typename ... T2>
   typename std::enable_if < sizeof...(T1) == sizeof...(T2) && index < sizeof...(T1), void>::type
   AddUnitTuples(const std::tuple<T1...>& Tup1, const std::tuple<T2...>& Tup2, std::tuple<T1...>& OutTup, double scalar1 = 1.0, double scalar2 = 1.0)
   {
      using Type1 = std::tuple_element_t<index, std::tuple<T1...>>;
      using Type2 = std::tuple_element_t<index, std::tuple<T2...>>;

      AddUnits(std::get<index>(Tup1), ConvertValue<Type2, Type1>(std::get<index>(Tup2)), std::get<index>(OutTup), scalar1, scalar2);
      AddUnitTuples<index + 1>(Tup1, Tup2, OutTup, scalar1, scalar2);
   }

   /// @brief Produce a tuple of doubles
   /// @details Returns a tuple of doubles of the given size,
   ///    initialized to the indicated value.
   /// @tparam Is... An index sequence informing the size of the tuple
   /// @param[in] The index sequence
   /// @param[in] value The value to initialize all items to
   /// @return The tuple of the indicated size initialized to the given value
   template<std::size_t... Is>
   auto make_tuple_of_doubles(std::index_sequence<Is...>, double value = 1.0) {
      return std::make_tuple((static_cast<void>(Is), value)...);
   }
}
#endif // STATIC_DIMENSION_UTILITIES_H
