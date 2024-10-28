#ifndef DIMENSION_CONVERSION_H
#define DIMENSION_CONVERSION_H

#include "GenericTypeTraits.h"

namespace Dimension
{
   /// Concept to check if T has a numeric slope attribute.
   template<typename T>
   concept HasSlope = requires { T::slope; } && std::is_arithmetic_v<decltype(T::slope)>;

   /// Concept to check if T has a numeric offset attribute.
   template<typename T>
   concept HasOffset = requires { T::offset; } && std::is_arithmetic_v<decltype(T::offset)>;

   /// @brief Return the slope as a constexpr if one exists,
   ///    otherwise return 1.0
   template<typename T>
   constexpr PrecisionType GetSlope()
   {
      if constexpr (HasSlope<T>)
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
      
      if constexpr (HasOffset<T>)
      {
         return T::offset;
      }
      else
      {
         return 0.0;
      }
   }

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

} // end Dimension

#endif // DIMENSION_CONVERSION_H