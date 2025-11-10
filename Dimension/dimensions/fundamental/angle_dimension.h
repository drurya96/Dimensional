#ifndef STATIC_DIMENSION_ANGLE_H
#define STATIC_DIMENSION_ANGLE_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/angle_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{
  struct radians{};

  template<> struct unit_traits<radians>
  {
     using dimension = angleType;
     static constexpr std::string_view name   = "radians";
     static constexpr std::string_view abbr   = "rad";
     static constexpr std::string_view dimName= "angle";
     static constexpr int id = 0;
  };
  struct degrees{};

  template<> struct unit_traits<degrees>
  {
     using dimension = angleType;
     static constexpr std::string_view name   = "Degrees";
     static constexpr std::string_view abbr   = "deg";
     static constexpr std::string_view dimName= "angle";
     static constexpr int id = 0;
  };

template<> struct Conversion<radians, degrees>
{
using scale = factor_t<
   std::ratio<1, 1>,
   std::tuple<ratio_exponent<std::ratio<2, 1>, 2, 1>, ratio_exponent<std::ratio<3, 1>, 2, 1>, ratio_exponent<std::ratio<5, 1>, 1, 1>>, std::tuple<symbol_exponent<symbols::pi, -1, 1>>>;
};



}

#include "extensions/angleExtras.h"

#endif // STATIC_DIMENSION_ANGLE_H