#ifndef DIMENSION_CONVERSION_H
#define DIMENSION_CONVERSION_H

#include "GenericTypeTraits.h"

namespace Dimension
{
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
} // end Dimension

#endif // DIMENSION_CONVERSION_H