#ifndef DIMENSION_UTILITIES_H
#define DIMENSION_UTILITIES_H

#include <type_traits>
#include <tuple>
#include <functional>
#include <utility>

namespace Dimension
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

   /// @brief Struct to return the underlying type a pointer points to
   template<typename T>
   struct remove_pointer;

   /// @brief Struct to return the underlying type a pointer points to
   /// @tparam T The underlying type
   /// @typedef type The underlying type of the given pointer
   template<typename T>
   struct remove_pointer<T*> {
      using type = T;
   };

   /// @brief Struct to remove one instance of a type from a tuple of types
   template<typename T, typename Tuple>
   struct RemoveOneInstance;

   /// @brief Struct to remove one instance of a type from a tuple of types
   ///    Specialization for an empty tuple
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
      using type = std::conditional_t<std::is_same_v<T, Head>,
         std::tuple<Tail...>,
         decltype(tuple_cat_t<std::tuple<Head>, typename RemoveOneInstance<T, std::tuple<Tail...>>::type>())>;
   };

   /// @brief Struct to check if a tuple of types contains a type
   template<typename T, typename Tuple>
   struct has_type;

   /// @brief Struct to check if a tuple of types contains a type
   /// @details Main specialization
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename T, typename... Us>
   struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<typename std::remove_cv<typename remove_pointer<T>::type>::type, typename std::remove_cv<typename remove_pointer<Us>::type>::type>...> {};
   
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
         has_type<T, std::tuple<subtrahendTypes...>>::value,
         typename tuple_diff<std::tuple<restMinuendTypes...>, typename RemoveOneInstance<T, std::tuple<subtrahendTypes...>>::type>::type,
         tuple_cat_t<std::tuple<T>, typename tuple_diff<std::tuple<restMinuendTypes...>, std::tuple<subtrahendTypes...>>::type>
      >;
   };
   
   /// @brief Function to convert given units to a tuple of pointers
   /// @details Specialization for an empty set
   /// @return Empty tuple
   inline std::tuple<> UnitList()
   {
      return std::tuple<>();
   }

   /// @brief Function to convert given units to a tuple of pointers
   /// @details Main specialization
   /// @tparam Ts... The types given
   /// @param ts... The objects, as references, to create pointers to
   /// @return A tuple of pointers to the given references
   template<typename ... Ts>
   auto UnitList(Ts&... ts) -> std::tuple<Ts*...>
   {
      return std::make_tuple(&ts...);
   }

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

   /// @brief Simple stuct exposing the ConvertValue callable based on the isNumerator flag
   /// @tparam isNumerator Bool indicating whether conversions should occur in context of a
   ///    numerator, which are inverted relative to denominator conversions.
   template<bool isNumerator>
   struct ConvertFunctor {
      /// @brief Operator overload binding () to ConvertValue
      /// @details Binds ConvertValue callable to the operator () using the
      ///    isNumerator flag, which converts from unit FromUnit to unit ToUnit.
      /// @tparam FromUnit The unit to convert from
      /// @tparam ToUnit The unit to convert from
      /// @param[in] fromUnit Pointer to the Unit object to convert from
      /// @param[in] toUnit Pointer to the Unit object to convert to
      /// @param[in,out] value The input value to be converted
      ///    which is updated through this call
      template <typename FromUnit, typename ToUnit>
      void operator()(const FromUnit* fromUnit, const ToUnit* toUnit, double& value) const {
         value = fromUnit->ConvertValue<isNumerator>(value, toUnit);
      }
   };

   /// @brief Update the given tuple at position I to the given value
   /// @details Recursive base-case after reaching the end of the tuple
   template <size_t I = 0, typename... Ts, typename UpdateType>
   typename std::enable_if<I == sizeof...(Ts),
      void>::type
      UpdateResultTuple(std::tuple<Ts...>& tup, UpdateType update)
   {
      return;
   }

   /// @brief Update the given tuple at position I to the given value
   /// @details Main specialization. Redirects the pointer to type UpdateType
   ///    at position I to point to the object update. This redirection
   ///    only occurs if the types match and the current point is nullptr.
   /// @tparam I Current tuple index
   /// @tparam Ts... Content types of the tuple
   /// @tparam UpdateType The type to update
   /// @param[in,out] tup The tuple to update
   /// @param[in] update The object to redirect the element of tup to
   template <size_t I = 0, typename... Ts, typename UpdateType>
   typename std::enable_if<(I < sizeof...(Ts)),
      void>::type
      UpdateResultTuple(std::tuple<Ts...>& tup, UpdateType update)
   {
      if constexpr (std::is_same_v<UpdateType, std::tuple_element_t<I, std::tuple<Ts...>>>) {
         if (std::get<I>(tup) == nullptr)
            {
               std::get<I>(tup) = update;
            }
      }
      UpdateResultTuple<I + 1>(tup, update);
   }

   /// @brief Update the given value using the conversion function from fromUnit to index I of tup
   /// @details Recursive base-case after reaching the end of the tuple
   template <bool convertValue, size_t I = 0, typename... Ts, typename FromUnit>
   typename std::enable_if<I == sizeof...(Ts),
      bool>::type
      CancelUnit(const std::tuple<Ts...>& toUnits, const FromUnit& fromUnit, double& value, int counter, int currentCount = 0)
   {
      return false;
   }

   /// @brief Update the given value using the conversion function from fromUnit to index I of tup
   /// @details Main specialization. If element I of ToUnits is the same type as FromUnit,
   ///    increment the counter indicating a correct unit type was found. If the counter is now greater
   ///    than the given counter of already handled units, update the given value accordingly.
   ///    This means call the conversion function from fromUnit to index I of toUnits if these objects
   ///    aren't the same. If they are the same, no conversion is necessary.
   ///    Update the given Cancel parameter to indicate a unit was cancelled.
   /// @tparam I Index of toUnits to check
   /// @tparam Ts... Types of toUnits
   /// @tparam FromUnit Type of fromUnit
   /// @param[in] toUnits Tuple of unit pointers to convert to
   /// @param[in] fromUnit Unit pointer to convert from
   /// @param[in,out] value The given value to update
   /// @param[in,out] counter The number of units of type FromUnit already cancelled
   /// @param[in] currentCount The number of units of type FromUnit checked in this recurssive check
   /// @return Bool indicating whether a unit was cancelled
   template <bool convertValue, size_t I = 0, typename... Ts, typename FromUnit>
   typename std::enable_if<(I < sizeof...(Ts)),
      bool>::type
      CancelUnit(const std::tuple<Ts...>& toUnits, const FromUnit& fromUnit, double& value, int counter, int currentCount = 0)
   {
      if constexpr (std::is_same<std::decay_t<FromUnit>, std::decay_t<decltype(std::get<I>(toUnits))>>::value)
      {
         currentCount++;
         if (currentCount > counter)
         {
            if constexpr (convertValue)
            {
               if (fromUnit != std::get<I>(toUnits))
               {
                  value = fromUnit->ConvertValue<true>(value, std::get<I>(toUnits));
               }
            }
            return true;
         }
      }
      return CancelUnit<convertValue, I + 1>(toUnits, fromUnit, value, counter, currentCount);
   }

   /// @brief Add T to the tuple of Ts if not already present
   /// @tparam T The type to add
   /// @tparam Ts... The types in the tuple
   /// @typedef type Tuple of Ts... including T if not already found
   template <typename T, typename... Ts>
   struct AddIfUnique {
      using type = std::conditional_t<has_type<T, std::tuple<Ts...>>::value, std::tuple<Ts...>, std::tuple<Ts..., T>>;
   };

   /// @brief Implementation of MakeCounters
   /// @todo Consider updating this to a tuple of pairs
   ///    rather than tuple of tuples
   template <typename Tuple, typename UniqueTypes>
   struct MakeCountersImpl;


   // Given OrigNumTuple as tuple
   // Consider std::tuple<LengthUnit*, TimeUnit*>

   /// @brief Implementation of MakeCounters
   /// @details Specialization to recursively call MakeCountersImpl
   ///    to generate a tuple tuples, where the inner tuple 
   ///    is a tuple of unit pointer type and int
   /// @tparam T The first item in the tuple
   /// @tparam Ts... The remaining items in the tuple
   /// @tparam UniqueTypes... Tuple of unique types already added
   /// @typedef type The final tuple of tuple counters
   template <typename T, typename... Ts, typename... UniqueTypes>
   struct MakeCountersImpl<std::tuple<T, Ts...>, std::tuple<UniqueTypes...>> {
      using type = typename MakeCountersImpl<std::tuple<Ts...>, typename AddIfUnique<T, UniqueTypes...>::type>::type;
   };

   /// @brief Implementation of MakeCounters
   /// @details Base case specialization to call when the input
   ///    tuple is empty
   /// @tparam UniqueTypes... Tuple of unique types already added
   /// @typedef type The final tuple of tuple counters
   template <typename... UniqueTypes>
   struct MakeCountersImpl<std::tuple<>, std::tuple<UniqueTypes...>> {
      using type = std::tuple<typename std::tuple<UniqueTypes, int>...>;
   };

   /// @brief Main MakeCounters struct to generate tuple of counters
   /// @details Calls MakeCountersImpl to generate these counters
   /// @tparam Tuple Tuple to make counters based on
   /// @typedef type the output tuple of counters
   template <typename Tuple>
   struct MakeCounters {
      using type = typename MakeCountersImpl<Tuple, std::tuple<>>::type;
   };

   /// @brief Alias to directly access the counter tuple of MakeCounters
   /// @tparam Tuple Tuple to make counters based on
   template <typename Tuple>
   using MakeCounters_t = typename MakeCounters<Tuple>::type;

   /// @brief return a reference to the counter corresponding to the given type
   /// @tparam I The current index to check
   /// @tparam T The type to search for
   /// @tparam CounterArgs... The types of the counter tuple
   /// @param[in,out] counterTuple Tuple of tuples of Unit type, int
   /// @return Reference of the int corresponding to T in CounterArgs...
   template<size_t I = 0, typename T, typename ... CounterArgs>
   typename std::enable_if<I < sizeof...(CounterArgs), int&>::type
      getCounter(std::tuple<CounterArgs...>& counterTuple)
   {
      if constexpr(std::is_same_v<std::tuple<T, int>, std::tuple_element_t<I, std::tuple<CounterArgs...>>>)
      {
         return std::get<1>(std::get<I>(counterTuple));
      }
      else
      {
         return getCounter<I + 1, T>(counterTuple);
      }
   }

   /// @brief Update the given value, counter tuple, and output tuple by "cancelling" units
   /// @details Recursive base-case after reaching the end of the tuple
   template <bool convertValue, size_t I = 0, typename... PrimaryTuple, typename ... SecondaryTuple, typename ... OutTuple, typename ... CountArgs>
   typename std::enable_if<I == sizeof...(PrimaryTuple), void>::type
      CancelUnits(const std::tuple<PrimaryTuple...>& primaryTuple, const std::tuple<SecondaryTuple...>& secondaryTuple, std::tuple<OutTuple...>& outTuple, std::tuple<CountArgs...>& counterTup, double& value)
   {
      return;
   }

   /// @brief Update the given value, counter tuple, and output tuple by "cancelling" units
   /// @details Main specialization. Update the outTuple, counterTup, and value based on
   ///    the given the primaryTuple and secondaryTuple.
   /// @tparam I The current index of primaryTuple to attempt cancelling
   /// @tparam PrimaryTuple... The units in the primaryTuple
   /// @tparam SecondaryTuple... The units in the secondaryTuple
   /// @tparam OutTuple... The units in the resulting tuple
   /// @tparam CountArgs... The types in counterTup
   /// @param[in] primaryTuple Tuple of units to cancel
   /// @param[in] secondaryTuple Tuple of units to be cancelled with
   /// @param[in,out] outTuple The tuple of output units that will be updated
   /// @param[in,out] counterTup Tuple of counters to count how many of each unit type
   ///    have been cancelled.
   /// @param[in,out] value The value to update while cancelling
   template <bool convertValue, size_t I = 0, typename... PrimaryTuple, typename ... SecondaryTuple, typename ... OutTuple, typename ... CountArgs>
   typename std::enable_if<(I < sizeof...(PrimaryTuple)), void>::type
      CancelUnits(const std::tuple<PrimaryTuple...>& primaryTuple, const std::tuple<SecondaryTuple...>& secondaryTuple, std::tuple<OutTuple...>& outTuple, std::tuple<CountArgs...>& counterTup, double& value)
   {
      bool cancel = false;
      if constexpr (!is_same_v<std::tuple<PrimaryTuple...>, std::tuple<>>)
      {
         using compareType = std::tuple_element_t<I, std::tuple<PrimaryTuple...>>;
         if constexpr (!is_same_v<std::tuple<SecondaryTuple...>, std::tuple<>>)
         {
            if constexpr(has_type<compareType, std::tuple<SecondaryTuple...>>::value)
            {
               int& counter = getCounter<0, compareType>(counterTup);
               cancel = CancelUnit<convertValue, 0>(secondaryTuple, std::get<I>(primaryTuple), value, counter);
               counter++;
            }
         }
      }
      if (!cancel)
      {
         UpdateResultTuple(outTuple, std::get<I>(primaryTuple));
      }
      CancelUnits<convertValue, I + 1>(primaryTuple, secondaryTuple, outTuple, counterTup, value);
   }




















   /// @brief Struct to check if a tuple of types contains a type
   template<typename T, typename Tuple>
   struct has_match;

   /// @brief Struct to check if a tuple of types contains a type
   /// @details Main specialization
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename T, typename... Us>
   struct has_match<T, std::tuple<Us...>> : std::disjunction<std::is_same<typename std::remove_cv<typename T::Dim>::type, typename std::remove_cv<typename Us::Dim>::type>...> {};














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
         typename static_tuple_diff<std::tuple<restMinuendTypes...>, typename RemoveOneInstance<T, std::tuple<subtrahendTypes...>>::type>::type,
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
      return false;
   }


   template<int index = 0, typename T, typename ... RealTypes>
   typename std::enable_if<index < sizeof...(RealTypes), bool>::type
   StaticUpdateReal(T& Unit, std::tuple<RealTypes...>& RealTup)
   {
      if constexpr (std::is_same_v<std::tuple_element_t<index, std::tuple<RealTypes...>>::Dim, T::Dim>)
      {
         if (std::isnan(std::get<index>(RealTup).GetValue()))
         {
            std::get<index>(RealTup).SetValue(Unit.GetValue());
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
         std::cout << "Value: " << std::get<index>(NumTup).GetValue() << std::endl;
         if constexpr (inverse)
         {
            value *= std::get<index>(NumTup).GetPrimary().GetValue();
         }
         else
         {
            value /= std::get<index>(NumTup).GetPrimary().GetValue();
         }


      }
      StaticCancelUnits<index + 1, inverse>(NumTup, RealTup, value);
   }














   /// @brief Simplify dimensions
   /// @details Given two tuples of dimensions, produce
   ///    two new tuples that simplified after "cancelling out"
   ///    dimensions as appropriate.
   /// @tparam NumTuple Tuple representing numerator dimensions
   /// @tparam DenTuple Tuple representing denominator dimensions
   /// @typedef numTuple The numerator tuple after simplification
   /// @typedef denTuple The denominator tuple after simplification
   template<typename NumTuple, typename DenTuple>
   struct AllUnitSimplifier
   {
      using numTuple = typename tuple_diff<NumTuple, DenTuple>::type;
      using denTuple = typename tuple_diff<DenTuple, NumTuple>::type;
   };

   /// @brief Function to construct a simplified BaseDimension from a given BaseDimension
   /// @details Call AllUnitSimplifier on the given BaseDimension to simplify the units.
   ///    The resulting BaseDimension has simplified units (i.e. no instances of the same
   ///    type in both the numerator and denominator). The value will be adjusted accordingly.
   /// @tparam OrigNumTuple The type of the numerator tuple of obj
   /// @tparam OrigDenTuple The type of the denominator tuple of obj
   /// @param[in] obj The object to simplify
   /// @return A new BaseDimension with simplified units, value adjusted accordingly
   template<typename OrigNumTuple, typename OrigDenTuple>
   auto SimplifyBaseDimension(const BaseDimension<OrigNumTuple, OrigDenTuple>& obj) {

      using Simplified = typename AllUnitSimplifier<OrigNumTuple, OrigDenTuple>;

      Simplified::numTuple newNumTuple{};
      Simplified::denTuple newDenTuple{};

      // TODO: Check performance of this vs using the raw value using some performance tool
      //double value = obj.GetRawValue();
      double value = obj.GetVal(obj.numList, obj.denList);
      double dummyVal = 0;

      using numCounterType = MakeCounters_t<OrigNumTuple>;
      using denCounterType = MakeCounters_t<OrigDenTuple>;

      numCounterType numCounter{};
      denCounterType denCounter{};


      CancelUnits<true>(obj.numList, obj.denList, newNumTuple, denCounter, value);
      CancelUnits<false>(obj.denList, obj.numList, newDenTuple, numCounter, dummyVal);

      return BaseDimension<Simplified::numTuple, Simplified::denTuple>(value, newNumTuple, newDenTuple);
   }








   template<typename ... NumTypes, typename ... DenTypes>
   void TestDivision(const std::tuple<NumTypes...>& NumTuple, const std::tuple<DenTypes...>& DenTuple)
   {

   }







}
#endif // DIMENSION_UTILITIES_H
