#ifndef STATIC_DIMENSION_UTILITIES_H
#define STATIC_DIMENSION_UTILITIES_H

#include <tuple>

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

   template<typename T, typename U>
   struct is_match;

   template<typename T, typename U>
   struct is_match : std::is_same<typename std::remove_cv<typename T::Dim>::type, typename std::remove_cv<typename U::Dim>::type> {};


   /// @brief Struct to check if a tuple of types contains a type
   template<typename T, typename Tuple>
   struct has_match;

   /// @brief Struct to check if a tuple of types contains a type
   /// @details Main specialization
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename T, typename... Us>
   struct has_match<T, std::tuple<Us...>> : std::disjunction<is_match<T, Us>...> {};


   /// @brief Struct to remove one instance of a type from a tuple of types
   template<typename T, typename Tuple>
   struct Static_RemoveOneInstance;

   /// @brief Struct to remove one instance of a type from a tuple of types
   ///    Specialization for an empty tuple
   /// @typedef type An empty tuple
   template<typename T>
   struct Static_RemoveOneInstance<T, std::tuple<>> {
      using type = std::tuple<>;
   };

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Main specialization
   /// @tparam T The type to remove
   /// @tparam Head the first item of the tuple
   /// @tparam Tail the remaining items in the tuple
   /// @typedef type A tuple type matching the input tuple type, except the first instance of T is removed
   template<typename T, typename Head, typename... Tail>
   struct Static_RemoveOneInstance<T, std::tuple<Head, Tail...>> {
      using type = std::conditional_t<is_match<T, Head>::value,
         std::tuple<Tail...>,
         decltype(tuple_cat_t<std::tuple<Head>, typename Static_RemoveOneInstance<T, std::tuple<Tail...>>::type>())>;
   };

   /// @brief Update the input value, using the input callable, based on the given units
   /// @details Recursive base-case
   template <size_t I = 0, bool inverse = false, typename Qs, typename... Ts>
   typename std::enable_if<I == sizeof...(Ts), void>::type
      GetConvertedValue(const std::tuple<Ts...>& tup, double& value)
   {
      return;
   }

   /// @brief Update the input value, using the input callable, based on the given units
   /// @details Main specialization, fromTup and toTup are of the same type.
   /// @tparam I The current index of both tuples
   /// @tparam Ts... The types within the tuples
   /// @tparam Functor The callable type
   /// @param fromTup[in] Tuple of types to convert from
   /// @param toTup[in] Tuple of types to conver to
   /// @param value[in,out] Reference of a value to update
   /// @param func[in] The callable to use for conversion.
   ///    This should be either NumFunctor or DenFunctor
   template <size_t I = 0, bool inverse = false, typename Qs, typename... Ts>
   typename std::enable_if<(I < sizeof...(Ts)), void>::type
      GetConvertedValue(const std::tuple<Ts...>& fromTup, double& value)
   {
      if constexpr (inverse)
      {
         value /= ConvertValue<std::tuple_element_t<I, std::tuple<Ts...>>, std::tuple_element_t<I, Qs>>(std::get<I>(fromTup)).GetValue();
      }
      else
      {
         value *= ConvertValue<std::tuple_element_t<I, std::tuple<Ts...>>, std::tuple_element_t<I, Qs>>(std::get<I>(fromTup)).GetValue();
      }
      GetConvertedValue<I + 1, inverse, Qs>(fromTup, value);
   }

   /// @brief Find the difference between two type-tuples
   template<typename T, typename ...>
   struct static_tuple_diff;

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty minuend tuple
   /// @typedef type Empty tuple
   template<typename ... subtrahendTypes>
   struct static_tuple_diff<std::tuple<>, std::tuple<subtrahendTypes...>>
   {
      using type = std::tuple<>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty subtrahend tuple
   /// @typedef type The entire minuend tuple
   template<typename... restMinuendTypes>
   struct static_tuple_diff<std::tuple<restMinuendTypes...>, std::tuple<>> {
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
   struct static_tuple_diff<std::tuple<T, restMinuendTypes...>, std::tuple<subtrahendTypes...>> {
      using type = std::conditional_t<
         has_match<T, std::tuple<subtrahendTypes...>>::value,
         typename static_tuple_diff<std::tuple<restMinuendTypes...>, typename Static_RemoveOneInstance<T, std::tuple<subtrahendTypes...>>::type>::type,
         tuple_cat_t<std::tuple<T>, typename static_tuple_diff<std::tuple<restMinuendTypes...>, std::tuple<subtrahendTypes...>>::type>
      >;
   };

   template<typename NumTypes1, typename NumTypes2, typename DenTypes1, typename DenTypes2>
   struct StaticUnitSimplifier;

   template<typename ... NumTypes1, typename ... NumTypes2, typename ... DenTypes1, typename ... DenTypes2>
   struct StaticUnitSimplifier<std::tuple<NumTypes1...>, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>, std::tuple<DenTypes2...>>
   {

      using a = static_tuple_diff<std::tuple<NumTypes1...>, std::tuple<DenTypes2...>>;
      using b = static_tuple_diff<std::tuple<NumTypes2...>, std::tuple<DenTypes1...>>;
      //static_assert(std::is_same_v<a, std::tuple<NumTypes1...>>);
      //static_assert(std::is_same_v<b, std::tuple<NumTypes2...>>);
      using newNum = tuple_cat_t<typename a::type, typename b::type>;

      using c = static_tuple_diff<std::tuple<DenTypes1...>, std::tuple<NumTypes2...>>;
      using d = static_tuple_diff<std::tuple<DenTypes2...>, std::tuple<NumTypes1...>>;
      using newDen = tuple_cat_t<typename c::type, typename d::type>;

      using dimType = BaseDimension<newNum, newDen>;
   };

   template<int index = 0, typename T, typename ... RealTypes>
   typename std::enable_if < index == sizeof...(RealTypes), bool>::type
   StaticUpdateReal(T& Unit, std::tuple<RealTypes...>& RealTup)
   {
      if constexpr (sizeof...(RealTypes) == 0)
      {

      }
      return false;
   }


   template<int index = 0, typename T, typename ... RealTypes>
   typename std::enable_if<index < sizeof...(RealTypes), bool>::type
   StaticUpdateReal(T& Unit, std::tuple<RealTypes...>& RealTup)
   {
      if constexpr (std::is_same_v<std::tuple_element_t<index, std::tuple<RealTypes...>>::Dim, T::Dim>) // TODO: This is apparently safe for empty tuple, but would like to investigate
      {
         if (std::isnan(std::get<index>(RealTup).GetValue()))
         {
            //std::get<index>(RealTup).SetValue(Unit.GetValue());
            std::get<index>(RealTup).SetValue(ConvertValue<T, std::tuple_element_t<index, std::tuple<RealTypes...>>>(Unit).GetValue());
            return true;
         }
      }
      return StaticUpdateReal<index + 1>(Unit, RealTup);
   }


   template<int index = 0, bool inverse = false, typename ... NumTypes, typename ... RealTypes>
   typename std::enable_if < index == sizeof...(NumTypes), void>::type
   StaticCancelUnits(std::tuple<NumTypes...> NumTup, std::tuple<RealTypes...>& RealTup, double& value)
   {
      return;
   }

   template<int index = 0, bool inverse = false, typename ... NumTypes, typename ... RealTypes>
   typename std::enable_if<index < sizeof...(NumTypes), void>::type
   StaticCancelUnits(std::tuple<NumTypes...> NumTup, std::tuple<RealTypes...>& RealTup, double& value)
   {
      if (!StaticUpdateReal<0>(std::get<index>(NumTup), RealTup))
      {
         // Modify value by canceling
         if constexpr (inverse)
         {
            value /= std::get<index>(NumTup).GetPrimary().GetValue();
         }
         else
         {
            value *= std::get<index>(NumTup).GetPrimary().GetValue();
         }


      }
      StaticCancelUnits<index + 1, inverse>(NumTup, RealTup, value);
   }

   template<typename T>
   void AddUnits(const T& Unit1, const T& Unit2, T& OutUnit, double scalar1, double scalar2)
   {
      OutUnit.SetValue((Unit1.GetValue() * scalar1) + (Unit2.GetValue() * scalar2));
   }
   
   template<int index = 0, typename ... T1, typename ... T2>
   typename std::enable_if < sizeof...(T1) == sizeof...(T2) && index == sizeof...(T1), void>::type
   AddUnitTuples(const std::tuple<T1...>& Tup1, const std::tuple<T2...>& Tup2, std::tuple<T1...>& OutTup, double scalar1 = 1.0, double scalar2 = 1.0)
   {
      return;
   }

   template<int index = 0, typename ... T1, typename ... T2>
   typename std::enable_if < sizeof...(T1) == sizeof...(T2) && index < sizeof...(T1), void>::type
   AddUnitTuples(const std::tuple<T1...>& Tup1, const std::tuple<T2...>& Tup2, std::tuple<T1...>& OutTup, double scalar1 = 1.0, double scalar2 = 1.0)
   {
      
      using Type1 = std::tuple_element_t<index, std::tuple<T1...>>;
      using Type2 = std::tuple_element_t<index, std::tuple<T2...>>;

      AddUnits(std::get<index>(Tup1), ConvertValue<Type2, Type1>(std::get<index>(Tup2)), std::get<index>(OutTup), scalar1, scalar2);
      AddUnitTuples<index + 1>(Tup1, Tup2, OutTup, scalar1, scalar2);
   }

   template<std::size_t... Is>
   auto make_tuple_of_doubles(std::index_sequence<Is...>) {
      return std::make_tuple((static_cast<void>(Is), 1.0)...);
   }

   template<typename NumTuple, typename DenTuple>
   auto Simplify(BaseDimension<NumTuple, DenTuple>& obj)
   {
      using simplified = StaticUnitSimplifier<NumTuple, std::tuple<>, std::tuple<>, DenTuple>;

      simplified::dimType ret{};
      ret.value = obj.value;

      StaticCancelUnits<0, false>(obj.numList, ret.numList, ret.value);
      StaticCancelUnits<0, false>(std::tuple<>{}, ret.numList, ret.value);
      StaticCancelUnits<0, true>(obj.denList, ret.denList, ret.value);
      StaticCancelUnits<0, true>(std::tuple<>{}, ret.denList, ret.value);

      return ret;
   }
}
#endif // STATIC_DIMENSION_UTILITIES_H
