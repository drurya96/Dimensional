#ifndef DIMENSION_CONVERSION_H
#define DIMENSION_CONVERSION_H

#include <concepts>
#include <iostream>

#include "../TupleHandling.h"
#include "../rep_type.h"
#include "collapse_units.h"
#include "convertible.h"
#include "unit_filter.h" // Shouldn't be needed after cleanup
#include "unit_exponent_utils.h"

// TODO: IMPORTANT: Figure out best way to handle do_conversion

namespace dimension
{
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
         return input * factor::eval_factor<typename Conv::scale, double>();
      }
      else
      {
         return input / factor::eval_factor<typename Conv::scale, double>();
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
      using FromT = typename From::unit;
      using ToT = typename To::unit;

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
         return Convert<unit_primary_t<FromT>, To, Inverse>
         (
            Convert<FromT, unit_primary_t<FromT>, Inverse>(input)
         );
      }
   }

   namespace detail {

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
   } // namespace detail

} // end Dimension

#endif // DIMENSION_CONVERSION_H