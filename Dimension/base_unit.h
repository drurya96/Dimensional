#ifndef DIMENSIONAL_BASE_UNIT_H
#define DIMENSIONAL_BASE_UNIT_H

#include <tuple> // For std::tuple and related functions
#include <cmath> // For std::hypot, std::modf, std::fmod // @todo move this to Utilities
#include <stdexcept> // For std::invalid_argument
#include <numbers> // For std::numbers::pi
#include <ratio>
#include <concepts>
#include <type_traits>
#include <limits>
#include <utility>

#include "Dimension_Core/internal_temp/uuid.h"
#include "Dimension_Core/internal_temp/units/UnitValidation.h"
#include "Dimension_Core/internal_temp/units/unit_decomposition.h"
#include "Dimension_Core/internal_temp/units/unit_dim.h"
#include "Dimension_Core/internal_temp/Conversion.h"
#include "Dimension_Core/internal_temp/SI_Macro.h"
#include "Dimension_Core/streaming/Stream.h"
#include "Dimension_Core/serialization/Serialization.h"
#include "Dimension_Core/serialization/exact_tag_policy.h"
#include "Dimension_Core/serialization/raw_value_policy.h"

#include "Dimension_Core/internal_temp/point/point.h"

namespace dimension
{

   struct FundamentalUnitTag {};

   /// @brief A base class representing a unit
   /// @details This abstract class represents a Unit,
   ///    such as meters, seconds, Grams, etc.
   template<typename Unit, int UnitID = 0>
   struct BaseUnit : FundamentalUnitTag
   {
   public:
      /// @brief No constructor
      BaseUnit() = delete;

      /// @brief Used to handle subscripting
      /// @details Units only cancel if this value is the same.
      ///    This means, creating units with different IDs and combining them
      ///    into one dimension will prevent them from canelling out.
      constexpr static int ID = UnitID;
   };

}

#endif // DIMENSIONAL_BASE_UNIT_H
