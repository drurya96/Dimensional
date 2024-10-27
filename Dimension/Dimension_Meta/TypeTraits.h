#ifndef DIMENSION_TYPE_TRAITS_H
#define DIMENSION_TYPE_TRAITS_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction, std::conditional
#include <utility> // For std::declval

#include "Concepts.h"

namespace Dimension
{

   // Forward declarations
   template<typename NumTuple, typename DenTuple>
   class BaseDimension;

   //template<typename Unit>
   struct BaseUnit;

   /// @brief Convenience alias for retrieving the type of a tuple of types
   /// @tparam Ts Parameter pack to types to concatenate
   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

   /// @brief Struct to remove one instance of a type from a tuple of types
   template<template<typename, typename> typename Compare, typename T, typename Tuple>
   struct RemoveOneInstance;

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Specialization for an empty tuple
   /// @typedef type An empty tuple
   template<template<typename, typename> typename Compare, typename T>
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
      using type = typename tuple_diff<Compare, std::tuple<restMinuendTypes...>, typename RemoveOneInstance<is_same_dim, T, std::tuple<subtrahendTypes...>>::type>::type;
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
      using num1AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<NumTypes1...>, std::tuple<DenTypes2...>>;
      using num2AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<NumTypes2...>, std::tuple<DenTypes1...>>;

      using den1AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<DenTypes1...>, std::tuple<NumTypes2...>>;
      using den2AfterSimpleCancel = tuple_diff<std::is_same, std::tuple<DenTypes2...>, std::tuple<NumTypes1...>>;

      using remainingNum1 = tuple_diff<has_same_dim, typename num1AfterSimpleCancel::type, typename den2AfterSimpleCancel::type>;
      using remainingNum2 = tuple_diff<has_same_dim, typename num2AfterSimpleCancel::type, typename den1AfterSimpleCancel::type>;

      using remainingDen1 = tuple_diff<has_same_dim, typename den1AfterSimpleCancel::type, typename num2AfterSimpleCancel::type>;
      using remainingDen2 = tuple_diff<has_same_dim, typename den2AfterSimpleCancel::type, typename num1AfterSimpleCancel::type>;
      

   public:
      using newNum = tuple_cat_t<typename remainingNum1::type, typename remainingNum2::type>;
      using newDen = tuple_cat_t<typename remainingDen1::type, typename remainingDen2::type>;

      using numSimple = tuple_cat_t<typename num1AfterSimpleCancel::type, typename num2AfterSimpleCancel::type>;
      using denSimple = tuple_cat_t<typename den1AfterSimpleCancel::type, typename den2AfterSimpleCancel::type>;

      using dimType = BaseDimension<newNum, newDen>;

      constexpr static bool isDelta = !((std::tuple_size_v<newNum> == 1) && (std::tuple_size_v<newDen> == 0));
   };

   /// @brief A type-trait with void Dim and Primary, only used to satisfy a metaprogramming condition
   struct NullUnit
   {
      using Dim = void;
      using Primary = void;
   };

   /// @brief get the first unit in the tuple matching the dimension of T
   template<typename T, typename Tuple>
   struct get_first_match;

   /// @brief get the first unit in the tuple matching the dimension of T
   /// @details Specialization for no match found, return a NullUnit
   ///    This should not typical occur and is a sign of problematic code elsewhere
   /// @tparam T Unit to match against
   /// @tparam Tuple Tuple of units
   template<typename T>
   struct get_first_match<T, std::tuple<>> {
      using type = NullUnit;
   };

   /// @brief get the first unit in the tuple matching the dimension of T
   /// @details Primary specialization
   /// @tparam T Unit to match against
   /// @tparam Tuple Tuple of units
   /// @typedef type The type of unit of matching dimension to T
   template<typename T, typename Head, typename... Tail>
   struct get_first_match<T, std::tuple<Head, Tail...>> {
      using type = std::conditional_t<same_dim_v<T, Head>,
         Head,
         typename get_first_match<T, std::tuple<Tail...>>::type>;
   };

   /// @brief Type trait to check if a type has a T::slope attribute
   template <typename, typename = std::void_t<>>
   struct has_slope : std::false_type {};

   /// @brief Type trait to check if a type has a T::slope attribute
   template <typename T>
   struct has_slope<T, std::void_t<decltype(T::slope)>> : std::integral_constant<bool, std::is_same_v<decltype(T::slope), const double>> {};

   /// @brief Type trait to check if a type has a T::offset attribute
   template <typename, typename = std::void_t<>>
   struct has_offset : std::false_type {};

   /// @brief Type trait to check if a type has a T::offset attribute
   template <typename T>
   struct has_offset<T, std::void_t<decltype(T::offset)>> : std::integral_constant<bool, std::is_same_v<decltype(T::offset), const double>> {};

   /// @brief Conversion implementation
   /// @details base case when no conversion is defined, throws a compile-time error.
   template<typename fromUnit, typename toUnit, typename Enable = void>
   struct ConversionBase
   {
      static_assert(sizeof(fromUnit) == -1, "Conversion not defined for these units.");
   };

   /// @brief Struct defining the linear relationship between two units
   /// @details This relationship is meant to be specialized for each unit,
   ///    and by users when extensions are needed.
   template<typename fromUnit, typename toUnit>
   struct Conversion : ConversionBase<fromUnit, toUnit> {};

   /// @brief Struct defining the linear relationship between two units
   /// @details This specialization will be used when a final specialization is not provided, but the dimensions
   ///    are the same. This will step through the primary unit of this dimension.
   /// @typedef slope The linear relationship between the units
   /// @typedef offset The intercept between two units.
   ///    This will typically be 0.0, but there are some special cases, such as temparature.
   /// @todo Provide some insight to the user when this is used, so they may accurately
   ///    define useful specializations.
   template<typename fromUnit, typename toUnit>
   requires std::is_same_v<typename fromUnit::Dim, typename toUnit::Dim>
   struct ConversionBase<fromUnit, toUnit>
   {

      #ifdef REQUIRE_CONVERSIONS
         static_assert(sizeof(fromUnit) == -1, "No specialized conversion found. See compiler output for more details");
      #endif

      using toPrimary = Conversion<fromUnit, typename fromUnit::Primary>;
      using fromPrimary = Conversion<typename fromUnit::Primary, toUnit>;

      static constexpr PrecisionType slope = GetSlope<toPrimary>() * GetSlope<fromPrimary>();
      static constexpr PrecisionType offset = GetOffset<toPrimary>() + GetOffset<fromPrimary>();
   };

   /// @brief Struct defining the linear relationship between two units
   /// @details This relationship is meant to be specialized for each unit,
   ///    and by users when extensions are needed.
   ///    This specialization is used when converting to the same unit.
   template<typename Unit>
   struct Conversion<Unit, Unit>
   {
      static constexpr PrecisionType slope = 1.0;
      static constexpr PrecisionType offset = 0.0;
   };

   /// @brief Struct to inherit for derived units, used for type-trait
   /// @details Here, "Derived" means a unit which is made up of other units,
   ///    for example Newtons (kg-m/s-s), NOT a derived class/struct.
   struct DerivedUnit {};

   /// @brief Return true it T derives from DerivedUnit
   template <typename T>
   using is_derived_unit = std::is_base_of<DerivedUnit, T>;

   /// @brief Alias to get the value of is_derived_unit
   template <typename T>
   constexpr bool is_derived_unit_v = is_derived_unit<T>::value;

   /// @brief Extract the numerator and denominator of a unit
   /// @details When the unit is fundamental, it simply becomes the sole numerator
   ///    but if its derived, the numerator an denominator may be more complex.
   ///    Note this struct has no knowledge of whether this unit is, itself, found
   ///    in the numerator or denominator of a dimension, so flip accordingly.
   /// @tparam U The unit to extract from
   /// @typedef Num The numerator tuple of this unit
   /// @typedef Den The denominator tuple of this unit
   template<typename U, bool = is_derived_unit_v<U>>
   struct DerivedHelper {
      using Num = std::tuple<U>;
      using Den = std::tuple<>;
   };

   /// @brief Extract the numerator and denominator of a unit
   /// @tparam U The unit to extract from
   /// @typedef Num The numerator tuple of this unit
   /// @typedef Den The denominator tuple of this unit
   template<typename U>
   struct DerivedHelper<U, true> {
      using Num = typename U::NumTuple;
      using Den = typename U::DenTuple;
   };

   /// @brief Resolve all numerators and denominators of a given tuple
   template<typename Tuple>
   struct HandleDerived;

   /// @brief Resolve all numerators and denominators of a given tuple
   /// @details Base case returning empty tuples
   template<>
   struct HandleDerived<std::tuple<>> {
      using Num = std::tuple<>;
      using Den = std::tuple<>;
   };

   /// @brief Resolve all numerators and denominators of a given tuple
   /// @details Base case for the final item.
   /// @tparam T The unit to split
   /// @typedef Num The numerator tuple for this unit
   /// @typedef Den The denominator tuple for this unit
   template<typename T>
   struct HandleDerived<std::tuple<T>> {
      using Num = typename DerivedHelper<T>::Num;
      using Den = typename DerivedHelper<T>::Den;
   };

   /// @brief Resolve all numerators and denominators of a given tuple
   /// @details Recursive case for multiple units
   /// @tparam T The first unit to split
   /// @tparam Rest... The remaining items to split
   /// @typedef Num The numerator tuple for all units, recursive
   /// @typedef Den The denominator tuple for all units, recursive
   template<typename T, typename ... Rest>
   struct HandleDerived<std::tuple<T, Rest...>> {
      using Num = tuple_cat_t<typename DerivedHelper<T>::Num, typename HandleDerived<std::tuple<Rest...>>::Num>;
      using Den = tuple_cat_t<typename DerivedHelper<T>::Den, typename HandleDerived<std::tuple<Rest...>>::Den>;
   };

   /// @brief Extract all fundamental units from a given numerator and denominator
   /// @tparam NumTuple The numerator tuple of units
   /// @tparam DenTuple The denominator tuple of units
   /// @typedef Num The numerator tuple of fundamental units, after handling all derived units
   /// @typedef Den The denominator tuple of fundamental units, after handling all derived units
   template<typename NumTuple, typename DenTuple>
   struct FundamentalUnitExtractor {
      using Num = tuple_cat_t<typename HandleDerived<NumTuple>::Num, typename HandleDerived<DenTuple>::Den>;
      using Den = tuple_cat_t<typename HandleDerived<DenTuple>::Num, typename HandleDerived<NumTuple>::Den>;
   };

} // end Dimension

#endif // DIMENSION_TYPE_TRAITS_H