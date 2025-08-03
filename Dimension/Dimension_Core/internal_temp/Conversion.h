#ifndef DIMENSION_CONVERSION_H
#define DIMENSION_CONVERSION_H

#include <concepts>
#include <iostream>

#include "TupleHandling.h"
#include "rep_type.h"
#include "base_dimension_from_tuple.h"
#include "collapse_units.h"
#include "full_simplify.h"
#include "convertible.h"

namespace dimension
{

   /// @brief A type-trait with void Dim and Primary, only used to satisfy a metaprogramming condition
   struct NullUnit
   {
      using Dim = void;
      using Primary = void;
      static constexpr int ID = 0;
   };

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

   // Specialization when RhsType is Delta
   template <typename LhsType, typename RhsUnit>
   struct transform_rhs_element
   {
      using type = typename extract_type<LhsType>::type;
   };

   // Metafunction to transform the entire rhs tuple based on lhs tuple
   template <typename LhsTuple, typename RhsTuple, size_t... Is>
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

   /// @brief Conversion traits to be defined for each conversion
   /// @details For each conversion, define a slope and optionally an offset
   /// @tparam From Unit to convert from
   /// @tparam To Unit to convert to
   template<typename From, typename To>
   struct Conversion;

   /// @brief Implementation of convert function
   /// @tparam Conv Conversion struct
   /// @tparam Inverse Whether to use the inverse of the conversion traits
   /// @param input value to convert
   /// @return converted value
   template<typename Conv, bool Inverse>
   constexpr auto ConvertImpl(rep_type auto input)
   {
      if constexpr (!Inverse)
      {
         return input * Conv::slope;
      }
      else
      {
         return input / Conv::slope;
      }
   }


   /// @brief Convert from one unit to another
   /// @tparam From Unit to convert from
   /// @tparam To Unit to convert to
   /// @tparam Inverse Whether to use the inverse of the conversion traits
   /// @param input value to convert
   /// @return converted value
   template<typename From, typename To, bool Inverse = false>
   constexpr auto Convert(rep_type auto input)
   {
      using FromT = extract_type<From>::type;
      using ToT = extract_type<To>::type;

      // Checks on base_dimension ensure a conversion to/from Primary exists.
      // No need to further constrain this function.

      if constexpr (std::is_same_v<FromT, ToT>) // Conversion to self
      {
         return input;
      }
      else if constexpr (HasConversion<FromT, ToT>) // Direct conversion exists
      {
         return ConvertImpl<Conversion<FromT, ToT>, Inverse>(input);
      }
      else if constexpr (HasConversion<ToT, FromT>) // Inverse direct conversion exists
      {
         return ConvertImpl<Conversion<ToT, FromT>, !Inverse>(input);
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






   // =================================
   // ======= More Things... ==========
   // =================================


   // ============================================================
   // ============== Find Matching Unit by Dimension =============
   // ============================================================

   // THIS IS NAMED VERY WRONG AND IS CONFUSING
   // THIS IS REALLY FINDING MATCHING UNITS, NOT DIMENSIONS

   template<typename Target, typename Tuple>
   struct find_unit_by_dimension;

   // Empty case
   template<typename Target>
   struct find_unit_by_dimension<Target, std::tuple<>> {
      static constexpr bool found = false;

      struct Dummy
      {
         using exponent = std::ratio<0>;
      };
      using type = Dummy;
   };

   // Recursive case
   template<typename Target, typename Head, typename... Tail>
   struct find_unit_by_dimension<Target, std::tuple<Head, Tail...>> {
   private:
      static constexpr bool is_match = std::is_same_v<typename Target::unit, typename Head::unit>;

   public:
      static constexpr bool found = is_match || find_unit_by_dimension<Target, std::tuple<Tail...>>::found;
      using type = std::conditional_t<is_match, Head, typename find_unit_by_dimension<Target, std::tuple<Tail...>>::type>;
   };

   template<typename From, typename ToTuple>
   struct MatchUnit;
   

   template<typename From>
   struct MatchUnit<From, std::tuple<>>
   {
      using type = void; // Should never occur
   };

   template<typename From, typename Head, typename... Tail>
   struct MatchUnit<From, std::tuple<Head, Tail...>>
   {
      //using unit_type = typename From::unit;
   
      static constexpr bool match = is_same_dim<typename From::unit, typename Head::unit>::value;

      using type = std::conditional_t<
         match,
         Head,
         typename MatchUnit<From, std::tuple<Tail...>>::type
      >;
   };

   template<typename... Units>
   struct ConvertSimplified;

   template<typename... ToUnits>
   struct ConvertSimplified<std::tuple<>, std::tuple<ToUnits...>>
   {
      static constexpr double scalar = 1.0;
   };

   template<typename FromUnit, typename... FromRest, typename... ToUnits>
   struct ConvertSimplified<std::tuple<FromUnit, FromRest...>, std::tuple<ToUnits...>>
   {
      using ToMatch = typename MatchUnit<FromUnit, std::tuple<ToUnits...>>::type;

      static constexpr double scalar =
         details::do_conversion<typename ToMatch::unit, FromUnit>(1.0) *
         ConvertSimplified<std::tuple<FromRest...>, std::tuple<ToUnits...>>::scalar;
   };







   // ============================================================
   // ==================== Subtract Tuples =======================
   // ============================================================

   template<typename TupleA, typename TupleB>
   struct Subtractunit_exponents;

   // Empty base case
   template<>
   struct Subtractunit_exponents<std::tuple<>, std::tuple<>> {
      using type = std::tuple<>;
   };

   // General case: A and B are std::tuple<unit_exponent<...>...>
   template<typename... UnitsA, typename... UnitsB>
   struct Subtractunit_exponents<std::tuple<UnitsA...>, std::tuple<UnitsB...>> {
   private:
      template<typename UnitA>
      struct subtract_one {
         using matching = find_unit_by_dimension<UnitA, std::tuple<UnitsB...>>;
         
         using result = std::conditional_t<
               matching::found,
               unit_exponent<
                  typename UnitA::unit,
                  std::ratio_subtract<
                     typename UnitA::exponent,
                     typename matching::type::exponent
                  >::num,
                  std::ratio_subtract<
                     typename UnitA::exponent,
                     typename matching::type::exponent
                  >::den
               >,
               UnitA
         >;
      };

   public:
      using type = tuple_cat_t<
         std::tuple<typename subtract_one<UnitsA>::result>...
      >;
   };

   // ============================================================
   // ======================= ConvertDim =========================
   // ============================================================

   template<typename FromTuple, typename ToTuple>
   struct ConvertDim
   {

       using RawFrom = typename base_dimension_from_tuple<FromTuple>::dim;
       using RawTo = typename base_dimension_from_tuple<ToTuple>::dim;

       using SimplifiedFrom = typename collapse_units<FromTuple>::units;
       using SimplifiedTo = typename collapse_units<ToTuple>::units;
   
       using FromRemainingRaw = typename Subtractunit_exponents<SimplifiedFrom, SimplifiedTo>::type;
       using ToRemainingRaw = typename Subtractunit_exponents<SimplifiedTo, SimplifiedFrom>::type;
   
       using FromRemaining = typename RemoveZeros<FromRemainingRaw>::units;
       using ToRemaining = typename RemoveZeros<ToRemainingRaw>::units;

      static constexpr double Convert(double value) 
      {

         using FromFullySimplified = decltype(full_simplify(RawFrom(1.0)));
         using ToFullySimplified = decltype(full_simplify(RawTo(1.0)));

         FromFullySimplified fullSimplified = full_simplify(RawFrom(value));
         constexpr double inverse_scalar = 1.0 / (full_simplify(RawTo(1.0)).template get_tuple_scalar<typename ToFullySimplified::units>());

         using converter = ConvertSimplified<typename FromFullySimplified::units, typename ToFullySimplified::units>;

         return fullSimplified.template get_tuple_scalar<typename FromFullySimplified::units>() * inverse_scalar * converter::scalar;
   }
   };









} // end Dimension

#endif // DIMENSION_CONVERSION_H