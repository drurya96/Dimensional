#ifndef STATIC_DIMENSION_UTILITIES_H
#define STATIC_DIMENSION_UTILITIES_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same

#include "Dimension_Meta/PrecisionType.h"
#include "Dimension_Meta/TypeTraits.h"

namespace Dimension
{
   // Forward declarations
   template<typename NumTuple, typename DenTuple>
   class BaseDimension;

   //template<typename Unit>
   struct BaseUnit;


   /// @brief Return the slope as a constexpr if one exists,
   ///    otherwise return 1.0
   template<typename T>
   constexpr PrecisionType GetSlope()
   {
      if constexpr (has_slope<T>::value)
      {
         return T::slope;
      }
      else
      {
         return 1.0;
      }
   }

   /// @brief Return the offset as a constexpr if one exists,
   ///    otherwise return 0.0
   template<typename T>
   constexpr PrecisionType GetOffset()
   {
      if constexpr (has_offset<T>::value)
      {
         return T::offset;
      }
      else
      {
         return 0.0;
      }
   }

   /// @brief Method to convert a value of fromUnit to a value of toUnit
   /// @details This method relies on the slope and offset of the necessary conversion.
   ///    While this method can be specialized to bypass the slope-offset relationship,
   ///    this should be done with great care as other parts of this library make assumptions
   ///    about this relationship.
   /// @tparam fromUnit Unit to convert from
   /// @tparam toUnit Unit to convert to
   /// @tparam isDelata Bool indicating whether this conversion is of a single unit in the normator (false)
   ///    or not (true).
   /// @tparam inverse Indicates whether value is in denominator (true) or numerator (false)
   /// @param[in] input Value to convert as a floating-point type
   /// @return floating-point type after conversion
   template<typename fromUnit, typename toUnit, bool isDelta = false, bool inverse = false>
   PrecisionType Convert(PrecisionType input)
   {
      if constexpr (std::is_same_v<fromUnit, toUnit>)
      {
         return input;
      }
      else if constexpr (std::is_same_v<typename fromUnit::Dim, typename toUnit::Dim>)
      {
         // Do conversion using Conversion struct
         using conv = Conversion<fromUnit, toUnit>;
         constexpr PrecisionType slope = GetSlope<conv>();
         constexpr PrecisionType offset = GetOffset<conv>();
         if constexpr (isDelta)
         {
            if constexpr (inverse)
            {
               return (input / slope);
            }
            else
            {
               return (input * slope);
            }

         }
         else
         {
            return (input * slope) + offset;
         }
      }
      else
      {
         static_assert(sizeof(fromUnit) == -1, "No possible conversion for between these types.");
      }
   }

   
   /// @brief Unit cancellation implementation
   /// @details recursive base-case
   template<bool inverse = false, int index = 0, typename RealTupType, typename IncomingTupType, bool isDelta = false>
   requires (index == std::tuple_size_v<IncomingTupType>)
   void CancelUnitsImpl(PrecisionType&)
   {
      return;
   }

   /// @brief Unit cancellation implementation
   /// @details Updates the given value when units are cancelled,
   ///    and updates the given unit tuple when a unit is not cancelled.
   /// @tparam inverse bool indicating whether the item is in numerator (false) or denominator (true)
   /// @tparam index The tuple index to attempt cancelling
   /// @tparam IncomingTupType The types to attempt cancelling
   /// @tparam RealTupType The types to update as needed
   /// @tparam isDelata Bool indicating whether this conversion is of a single unit in the normator (false)
   ///    or not (true).
   /// @param[in,out] value The value to update when cancelling units
   template<bool inverse = false, int index = 0, typename RealTupType, typename IncomingTupType, bool isDelta = false>
   requires (index < std::tuple_size_v<IncomingTupType>)
   void CancelUnitsImpl(PrecisionType& value)
   {
      using currentType = std::tuple_element_t<index, IncomingTupType>;
      if constexpr (has_same_dim<currentType, RealTupType>::value)
      {
         value = Convert<currentType, typename get_first_match<currentType, RealTupType>::type, isDelta, inverse>(value);
         CancelUnitsImpl<inverse, index + 1, typename RemoveOneInstance<is_same_dim, currentType, RealTupType>::type, IncomingTupType, isDelta>(value);
      }
      else
      {
         value = Convert<currentType, typename currentType::Primary, isDelta, inverse>(value);
         CancelUnitsImpl<inverse, index + 1, RealTupType, IncomingTupType, isDelta>(value);
      }
   }

   /// @brief Perform unit cancellation
   /// @tparam NumTupType Incoming numerator types
   /// @tparam DenTupType Incoming denominator types
   /// @tparam RealNumTupType Numerator types to update
   /// @tparam RealDenTupType Denominator types to udpate
   /// @tparam isDelata Bool indicating whether this conversion is of a single unit in the normator (false)
   ///    or not (true).
   /// @param[in,out] value Value to update
   template<typename NumTupType, typename DenTupType, typename RealNumTupType, typename RealDenTupType, bool isDelta = false>
   void CancelUnits(PrecisionType& value)
   {
      CancelUnitsImpl<false, 0, RealNumTupType, NumTupType, isDelta>(value);
      CancelUnitsImpl<true, 0, RealDenTupType, DenTupType, isDelta>(value);
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
      value = Convert<std::tuple_element_t<I, fromTup>, std::tuple_element_t<I, toTuple>, isDelta, inverse>(value);
      ConvertDimension<I + 1, inverse, toTuple, fromTup, isDelta>(value);
   }
}
#endif // STATIC_DIMENSION_UTILITIES_H
