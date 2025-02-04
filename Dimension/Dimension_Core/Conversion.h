#ifndef DIMENSION_CONVERSION_H
#define DIMENSION_CONVERSION_H

#include "TupleHandling.h"
#include <concepts>

#include <iostream>

namespace Dimension
{

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   template<typename Dim, typename Tuple>
   struct has_same_dim;

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename Dim, typename... Us>
   struct has_same_dim<Dim, std::tuple<Us...>> : std::disjunction<is_same_dim<Dim, Us>...> {};

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   template<typename Dim, typename Tuple>
   struct has_same_quantity_dim;

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename Dim, typename... Us>
   struct has_same_quantity_dim<Dim, std::tuple<Us...>> : std::disjunction<is_same_quantity_dim_diff_unit<Dim, Us>...> {};

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   template<typename Dim, typename Tuple>
   struct has_dim;

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename Dim, typename... Us>
   struct has_dim<Dim, std::tuple<Us...>> : std::disjunction<is_dim<Dim, Us>...> {};

   /// @brief get the first unit in the tuple matching the dimension of T
   template<template<typename, typename> typename Compare, typename T, typename Tuple>
   struct get_first_match;

   /// @brief get the first unit in the tuple matching the dimension of T
   /// @details Specialization for no match found, return a NullUnit
   ///    This should not typical occur and is a sign of problematic code elsewhere
   /// @tparam T Unit to match against
   /// @tparam Tuple Tuple of units
   template<template<typename, typename> typename Compare, typename T>
   struct get_first_match<Compare, T, std::tuple<>> {
      using type = NullUnit;
   };

   /// @brief get the first unit in the tuple matching the dimension of T
   /// @details Primary specialization
   /// @tparam T Unit to match against
   /// @tparam Tuple Tuple of units
   /// @typedef type The type of unit of matching dimension to T
   template<template<typename, typename> typename Compare, typename T, typename Head, typename... Tail>
   struct get_first_match<Compare, T, std::tuple<Head, Tail...>> {
      using type = std::conditional_t<Compare<T, Head>::value,
         Head,
         typename get_first_match<Compare, T, std::tuple<Tail...>>::type>;
   };

   template <typename Dim, typename Tuple>
   struct quantity_tuple;

   template <typename Dim, typename Tuple>
   requires (has_dim<Dim, Tuple>::value)
   struct quantity_tuple<Dim, Tuple>
   {
   private:
      using firstType = get_first_match<is_dim, Dim, Tuple>::type;
      using T = std::conditional_t<is_quantity_v<firstType>, std::tuple<firstType>, std::tuple<>>;

   public:
      using type = tuple_cat_t<T, typename quantity_tuple<Dim, typename RemoveOneInstance<is_dim, Dim, Tuple>::type>::type>;
   };

   template <typename Dim, typename Tuple>
   requires (!has_dim<Dim, Tuple>::value)
   struct quantity_tuple<Dim, Tuple>
   {
      using type = std::tuple<>;
   };

   template <typename Dim, typename Tuple>
   struct delta_tuple;

   template <typename Dim, typename Tuple>
   requires (has_dim<Dim, Tuple>::value)
   struct delta_tuple<Dim, Tuple>
   {
   private:
      using firstType = get_first_match<is_dim, Dim, Tuple>::type;
      using T = std::conditional_t<!is_quantity_v<firstType>, std::tuple<firstType>, std::tuple<>>;

   public:
      using type = tuple_cat_t<T, typename delta_tuple<Dim, typename RemoveOneInstance<is_dim, Dim, Tuple>::type>::type>;
   };

   template <typename Dim, typename Tuple>
   requires (!has_dim<Dim, Tuple>::value)
   struct delta_tuple<Dim, Tuple>
   {
      using type = std::tuple<>;
   };

   // Determine quantity-delta type for addition/subtraction
   // Start with subtraction

   template<typename Dim, typename lhsTuple, typename rhsTuple>
   struct expand_tuples_delta_quantity
   {
      using lhsQuantityTuple = quantity_tuple<Dim, lhsTuple>::type;
      using rhsQuantityTuple = quantity_tuple<Dim, rhsTuple>::type;

      using lhsDeltaTuple = delta_tuple<Dim, lhsTuple>::type;
      using rhsDeltaTuple = delta_tuple<Dim, rhsTuple>::type;
   };

   // Metafunction to transform a single pair of types
   template <typename LhsType, typename RhsType>
   struct transform_rhs_element;

   // Specialization when RhsType is Quantity
   template <typename LhsType, typename RhsUnit>
   requires (is_quantity_v<RhsUnit>)
   struct transform_rhs_element<LhsType, RhsUnit>
   {
      using type = Quantity<typename type_from_quantity_or_delta<LhsType>::type>;
   };

   // Specialization when RhsType is Delta
   template <typename LhsType, typename RhsUnit>
   requires (!is_quantity_v<RhsUnit>)
   struct transform_rhs_element<LhsType, RhsUnit>
   {
      using type = typename type_from_quantity_or_delta<LhsType>::type;
   };


   // Metafunction to transform the entire rhs tuple based on lhs tuple
   template <typename LhsTuple, typename RhsTuple, size_t... Is>
   // cppcheck-suppress unusedFunction // Only used for type deduction
   auto transform_rhs_impl(std::index_sequence<Is...>) 
      -> std::tuple< typename transform_rhs_element<
                     std::tuple_element_t<Is, LhsTuple>, 
                     std::tuple_element_t<Is, RhsTuple>
                  >::type... >
   {
      return {};
   }

   template <typename LhsTuple, typename RhsTuple>
   struct transform_rhs_tuple {
      static constexpr size_t N = std::tuple_size_v<LhsTuple>;
      static_assert(N == std::tuple_size_v<RhsTuple>, "Tuples must be of the same size.");
      
      using type = decltype(transform_rhs_impl<LhsTuple, RhsTuple>(std::make_index_sequence<N>{}));
   };
   
   template<typename Dim, typename lhsTuple, typename rhsTuple>
   struct subtract_tuples
   {
   private:
      using expanded = expand_tuples_delta_quantity<Dim, lhsTuple, rhsTuple>;

   public:
      static_assert(std::tuple_size_v<typename expanded::lhsDeltaTuple> <= std::tuple_size_v<typename expanded::rhsDeltaTuple>, "Cannot subtract Quantity from Delta!");

      // handle lhsDeltas
      using resultDeltaTupInitial = typename expanded::lhsDeltaTuple;

      static constexpr int remainingDeltas = std::tuple_size_v<typename expanded::rhsDeltaTuple> - std::tuple_size_v<typename expanded::lhsDeltaTuple>;

      // handle lhsQuantities

      // Quantities to subtract deltas from (yields quantity)
      using quantityTupleSlice = tuple_slice<remainingDeltas, typename expanded::lhsQuantityTuple>;
      using resultQuantityTup = quantityTupleSlice::first;

      // Remaining quantities subtract quantities, yielding deltas
      // Get that tuple first, then concatenate with resultDeltaTupInitial
      using resultDeltaTupFromQuantities = make_all_delta_t<typename quantityTupleSlice::last>;


      using resultDeltaTup = tuple_cat_t<resultDeltaTupInitial, resultDeltaTupFromQuantities>;


      using type = tuple_cat_t<resultDeltaTup, resultQuantityTup>;

      using lhsTypeOrder = tuple_cat_t<typename expanded::lhsDeltaTuple, typename expanded::lhsQuantityTuple>;
      using rhsTypeOrderPreConvert = tuple_cat_t<typename expanded::rhsDeltaTuple, typename expanded::rhsQuantityTuple>;
      using rhsTypeOrder = transform_rhs_tuple<lhsTypeOrder, rhsTypeOrderPreConvert>::type;
   };

   template<typename lhsTuple, typename rhsTuple, typename dimTuple, size_t Index = 0>
   struct subtract_all_impl;


   // Specialization for valid Index
   template<typename lhsTuple, typename rhsTuple, typename dimTuple, size_t Index>
   requires (Index < std::tuple_size_v<dimTuple>)
   struct subtract_all_impl<lhsTuple, rhsTuple, dimTuple, Index>
   {
      // Current dimension
      using currentDim = typename std::tuple_element_t<Index, dimTuple>;

      // Perform subtraction for the current dimension
      using subtractType = subtract_tuples<currentDim, lhsTuple, rhsTuple>;

      // Recursive call: Only one instance
      using next = subtract_all_impl<lhsTuple, rhsTuple, dimTuple, Index + 1>;

      // Concatenate the current result with the recursive result
      using type = tuple_cat_t<typename subtractType::type, typename next::type>;

      using rhsTypeOrder = tuple_cat_t<typename subtractType::rhsTypeOrder, typename next::rhsTypeOrder>;

      using lhsTypeOrder = tuple_cat_t<typename subtractType::lhsTypeOrder, typename next::lhsTypeOrder>;
   };

   // Base case specialization: When Index == size of dimTuple
   template<typename lhsTuple, typename rhsTuple, typename dimTuple, size_t Index>
   requires (Index >= std::tuple_size_v<dimTuple>)
   struct subtract_all_impl<lhsTuple, rhsTuple, dimTuple, Index>
   {
      using type = std::tuple<>;
      using rhsTypeOrder = std::tuple<>;
      using lhsTypeOrder = std::tuple<>;
   };

   template<typename lhsTuple, typename rhsTuple>
   struct subtract_all
   {
   private:
      using DimTuple = get_dim_tuple<lhsTuple>::type;
      using result = subtract_all_impl<lhsTuple, rhsTuple, DimTuple>;
   public:
      using type = result::type;
      using rhsTypeOrder = result::rhsTypeOrder;
      using lhsTypeOrder = result::lhsTypeOrder;
   };

   template<typename Lhs, typename Rhs>
   concept Subtractable = requires {
      typename subtract_all<Lhs, Rhs>::type;
   };

   template<typename Dim, typename lhsTuple, typename rhsTuple>
   struct add_tuples
   {
   private:
      using expanded = expand_tuples_delta_quantity<Dim, lhsTuple, rhsTuple>;

   public:
      static_assert(std::tuple_size_v<typename expanded::lhsQuantityTuple> <= std::tuple_size_v<typename expanded::rhsDeltaTuple>, "Cannot Add two Quantities!");
      static_assert(std::tuple_size_v<typename expanded::rhsQuantityTuple> <= std::tuple_size_v<typename expanded::lhsDeltaTuple>, "Cannot Add two Quantities!");

      using quantitiesFromLhs = typename expanded::lhsQuantityTuple;
      using quantitiesFromRhs = typename expanded::rhsQuantityTuple;

      using quantities = tuple_cat_t<quantitiesFromLhs, quantitiesFromRhs>;

      static constexpr int remainingDeltasOnLhs = std::tuple_size_v<typename expanded::lhsDeltaTuple> - std::tuple_size_v<typename expanded::rhsQuantityTuple>;
      static constexpr int remainingDeltasOnRhs = std::tuple_size_v<typename expanded::rhsDeltaTuple> - std::tuple_size_v<typename expanded::lhsQuantityTuple>;

      static_assert(remainingDeltasOnLhs == remainingDeltasOnRhs, "SOMETHING WENT TERRIBLY WRONG, PLEASE OPEN A BUG REPORT IF THIS IS EVER SEEN DURING COMPILATION! (Unequal number of remaining deltas during addition)");

      using deltasRemaining = last_N_types<remainingDeltasOnLhs, typename expanded::lhsDeltaTuple>::type;

      using type = tuple_cat_t<deltasRemaining, quantities>;

      using lhsTypeOrder = tuple_cat_t<typename expanded::lhsQuantityTuple, typename expanded::lhsDeltaTuple>;

      using rhsTypeOrderPreConvert = tuple_cat_t<typename expanded::rhsDeltaTuple, typename expanded::rhsQuantityTuple>;
      using rhsTypeOrder = transform_rhs_tuple<lhsTypeOrder, rhsTypeOrderPreConvert>::type;
   };

   template<typename lhsTuple, typename rhsTuple, typename dimTuple, size_t Index = 0>
   struct add_all_impl;


   template<typename lhsTuple, typename rhsTuple, typename dimTuple, size_t Index>
   requires (Index < std::tuple_size_v<dimTuple>)
   struct add_all_impl<lhsTuple, rhsTuple, dimTuple, Index>
   {
      // Current dimension
      using currentDim = typename std::tuple_element_t<Index, dimTuple>;

      // Perform addition for the current dimension
      using addType = add_tuples<currentDim, lhsTuple, rhsTuple>;

      // Recursive call: Only one instance
      using next = add_all_impl<lhsTuple, rhsTuple, dimTuple, Index + 1>;

      // Concatenate the current result with the recursive result
      using type = tuple_cat_t<typename addType::type, typename next::type>;

      using rhsTypeOrder = tuple_cat_t<typename addType::rhsTypeOrder, typename next::rhsTypeOrder>;

      using lhsTypeOrder = tuple_cat_t<typename addType::lhsTypeOrder, typename next::lhsTypeOrder>;                               
   };

   template<typename lhsTuple, typename rhsTuple, typename dimTuple, size_t Index>
   requires (Index >= std::tuple_size_v<dimTuple>)
   struct add_all_impl<lhsTuple, rhsTuple, dimTuple, Index>
   {
      using type = std::tuple<>;
      using rhsTypeOrder = std::tuple<>;
      using lhsTypeOrder = std::tuple<>;
   };

   template<typename lhsTuple, typename rhsTuple>
   struct add_all
   {
   private:
      using DimTuple = get_dim_tuple<lhsTuple>::type;
      using result = add_all_impl<lhsTuple, rhsTuple, DimTuple>;
   public:
      using type = result::type;
      using rhsTypeOrder = result::rhsTypeOrder;
      using lhsTypeOrder = result::lhsTypeOrder;
   };

   template<typename Lhs, typename Rhs>
   concept Addable = requires {
      typename add_all<Lhs, Rhs>::type;
   };

   /// @brief Concept to check if T has a numeric offset attribute.
   template<typename T>
   concept HasOffset = requires { T::offset; } && std::is_arithmetic_v<decltype(T::offset)> && requires { T::offset != 0; };

   /// @brief Return the offset as a constexpr if one exists,
   ///    otherwise return 0.0
   template<typename T>
   constexpr PrecisionType GetOffset()
   {
      
      if constexpr (HasOffset<T>)
      {
         return T::offset;
      }
      else
      {
         return 0.0;
      }
   }

   /// @brief Conversion traits to be defined for each conversion
   /// @details For each conversion, define a slope and optionally an offset
   /// @tparam From Unit to convert from
   /// @tparam To Unit to convert to
   template<typename From, typename To>
   struct Conversion;

   /// @brief Concept to check if Conversion is defined
   template<typename From, typename To>
   concept HasConversion = requires { Conversion<From, To>::slope; };

   /// @brief Concept to check if the unit is convertible to a primary unit (either direction)
   template<typename T>
   concept PrimaryConvertible = requires {
      typename T::Primary;
      requires (std::is_same_v<T, typename T::Primary> || HasConversion<T, typename T::Primary> || HasConversion<typename T::Primary, T>);
   };

   /// @brief Implementation of convert function
   /// @tparam Conv Conversion struct
   /// @tparam IsDelta Bool indicating if the unit being converted is sole unit of the dimension 
   ///   and is in the numerator (False), otherwise True
   /// @tparam Inverse Whether to use the inverse of the conversion traits
   /// @param input value to convert
   /// @return converted value
   template<typename Conv, bool IsDelta, bool Inverse>
   constexpr PrecisionType ConvertImpl(PrecisionType input)
   {
      // Assumes all checks have been made

      if constexpr (!Inverse)
      {
         if constexpr (HasOffset<Conv> && !IsDelta) // Conversion provides offset
         {
            return input * Conv::slope + GetOffset<Conv>();
         }
         else // Conversion does not provides offset
         {
            return input * Conv::slope;
         }
      }
      else
      {
         if constexpr (HasOffset<Conv> && !IsDelta) // Conversion provides offset
         {
            return (input - GetOffset<Conv>()) / Conv::slope;
         }
         else // Conversion does not provides offset
         {
            return input / Conv::slope;
         }
      }
   }


   /// @brief Convert from one unit to another
   /// @tparam From Unit to convert from
   /// @tparam To Unit to convert to
   /// @tparam IsDelta Bool indicating if the unit being converted is sole unit of the dimension 
   ///   and is in the numerator (False), otherwise True
   /// @tparam Inverse Whether to use the inverse of the conversion traits
   /// @param input value to convert
   /// @return converted value
   template<typename From, typename To, bool Inverse = false>
   //requires (IsDelta || !Inverse)
   constexpr PrecisionType Convert(PrecisionType input)
   {

      constexpr bool IsDelta = !both_quantity<From, To>();

      using FromT = type_from_quantity_or_delta<From>::type;
      using ToT = type_from_quantity_or_delta<To>::type;

      // Checks on BaseDimension ensure a conversion to/from Primary exists.
      // No need to further constrain this function.

      if constexpr (std::is_same_v<FromT, ToT>) // Conversion to self
      {
         return input;
      }
      else if constexpr (HasConversion<FromT, ToT>) // Direct conversion exists
      {
         return ConvertImpl<Conversion<FromT, ToT>, IsDelta, Inverse>(input);
      }
      else if constexpr (HasConversion<ToT, FromT>) // Inverse direct conversion exists
      {
         return ConvertImpl<Conversion<ToT, FromT>, IsDelta, !Inverse>(input);
      }
      else // No direct conversion exists, fall back to primary
      {
         #ifdef REQUIRE_CONVERSIONS
            static_assert(sizeof(fromUnit) == -1, "No specialized conversion found. See compiler output for more details");
         #endif
         // Temporary variable is NOT created to allow full constexpr behavior in situations where input itself is constexpr
         return Convert<typename FromT::Primary, To, Inverse>
         (
            Convert<FromT, typename FromT::Primary, Inverse>(input)
         );
      }
   }

   /// @brief Convert the given value when units are cancelled out
   /// @details Recursive base-case
   template <size_t I = 0, bool inverse = false, typename toTuple, typename fromTup, bool isDelta = false>
   requires (I == std::tuple_size_v<fromTup>)
   constexpr PrecisionType ConvertDimension(PrecisionType value)
   {
      return value;
   }

   /// @brief Convert the given value when units are cancelled out
   /// @tparam I The current index of both tuples
   /// @tparam inverse Whether the conversion is inverted (i.e. in the denominator)
   /// @tparam toTuple The tuple type to convert to
   /// @tparam fromTuple The tuple type to convert from
   /// @tparam isDelata Bool indicating whether this conversion is of a single unit in the normator (false)
   ///    or not (true).
   /// @param value[in,out] Reference of a value to update
   template <size_t I = 0, bool inverse = false, typename toTuple, typename fromTup, bool isDelta = false>
   requires (I < std::tuple_size_v<fromTup>)
   constexpr PrecisionType ConvertDimension(PrecisionType value)
   {
      // cppcheck-suppress templateRecursion // cppcheck isn't aware of the requires clause preventing infinite recursion
      using fromType = std::tuple_element_t<I, fromTup>;
      using toType = typename get_first_match<is_same_dim, fromType, toTuple>::type;

      using remainingToTuple = typename RemoveOneInstance<is_same_dim, fromType, toTuple>::type;

      const PrecisionType convertedValue = Convert<fromType, toType, inverse>(value);
      return ConvertDimension<I + 1, inverse, remainingToTuple, fromTup, isDelta>(convertedValue);
   }

   template <bool B>
   struct implicit_cast_to_build_warning
   {
      static void call() {}
   };

   template <>
   struct implicit_cast_to_build_warning<true>
   {
      [[deprecated("Attempting to create new dimension ...")]]
      static void call() {}
   };



   template<typename NumTuple, typename NumAfterAbsConversion, typename TupleToCheck, bool ConvertToAbsolute>
   constexpr PrecisionType MakeAbsoluteQuantity(PrecisionType value)
   {
      if constexpr (ConvertToAbsolute)
      {
         // This emits a compiler warning (deprecation warning) if an implicit cast occurs and persists in the resulting dimension.
         // Generally, users should make these casts explicitly if the goal is to maintain the unit.
         // However, if the implicit cast occurs only to "cancel" an existing unit, this is typical and does NOT emit a warning.
         implicit_cast_to_build_warning<has_same_dim<std::tuple_element_t<0, NumAfterAbsConversion>, TupleToCheck>::value>::call();
         return Convert<std::tuple_element_t<0, NumTuple>, std::tuple_element_t<0, NumAfterAbsConversion>>(value);
      }
      else
      {
         return value;
      }
   }


} // end Dimension

#endif // DIMENSION_CONVERSION_H