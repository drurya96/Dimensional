#ifndef DIMENSION_CONVERSION_H
#define DIMENSION_CONVERSION_H

#include "TupleHandling.h"

namespace Dimension
{

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

   /// @brief Concept to check if T has a numeric offset attribute.
   template<typename T>
   concept HasOffset = requires { T::offset; } && std::is_arithmetic_v<decltype(T::offset)>;

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
   template<typename From, typename To, bool IsDelta = false, bool Inverse = false>
   requires (IsDelta || !Inverse)
   constexpr PrecisionType Convert(PrecisionType input) 
   {

      // Checks on BaseDimension ensure a conversion to/from Primary exists.
      // No need to further constrain this function.

      if constexpr (std::is_same_v<From, To>) // Conversion to self
      {
         return input;
      }
      else if constexpr (HasConversion<From, To>) // Direct conversion exists
      {
         return ConvertImpl<Conversion<From, To>, IsDelta, Inverse>(input);
      }
      else if constexpr (HasConversion<To, From>) // Inverse direct conversion exists
      {
         return ConvertImpl<Conversion<To, From>, IsDelta, !Inverse>(input);
      }
      else // No direct conversion exists, fall back to primary
      {
         #ifdef REQUIRE_CONVERSIONS
            static_assert(sizeof(fromUnit) == -1, "No specialized conversion found. See compiler output for more details");
         #endif
         // Temporary variable is NOT created to allow full constexpr behavior in situations where input itself is constexpr
         return Convert<typename From::Primary, To, IsDelta, Inverse>
         (
            Convert<From, typename From::Primary, IsDelta, Inverse>(input)
         );
      }

   }

   /// @brief Convert the given value when units are cancelled out
   /// @details Recursive base-case
   template <size_t I = 0, bool inverse = false, typename toTuple, typename fromTup, bool isDelta = false>
   requires (I == std::tuple_size_v<fromTup>)
   void ConvertDimension(PrecisionType&)
   {
      return;
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
   void ConvertDimension(PrecisionType& value)
   {
      using fromType = std::tuple_element_t<I, fromTup>;
      using toType = typename get_first_match<is_same_dim, fromType, toTuple>::type;

      using remainingToTuple = typename RemoveOneInstance<is_same_dim, fromType, toTuple>::type;

      value = Convert<fromType, toType, isDelta, inverse>(value);
      ConvertDimension<I + 1, inverse, remainingToTuple, fromTup, isDelta>(value);
   }

} // end Dimension

#endif // DIMENSION_CONVERSION_H