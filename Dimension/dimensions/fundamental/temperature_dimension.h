#ifndef STATIC_DIMENSION_TEMPERATURE_H
#define STATIC_DIMENSION_TEMPERATURE_H

#include <string_view>

#include "Dimension_Impl/FundamentalDimensions/temperature_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{
  struct kelvin{};

  template<> struct unit_traits<kelvin>
  {
     using dimension = temperatureType;
     static constexpr std::string_view name   = "Kelvin";
     static constexpr std::string_view abbr   = "K";
     static constexpr std::string_view dimName= "temperature";
     static constexpr int id = 0;
  };
  struct rankine{};

  template<> struct unit_traits<rankine>
  {
     using dimension = temperatureType;
     static constexpr std::string_view name   = "Rankine";
     static constexpr std::string_view abbr   = "R";
     static constexpr std::string_view dimName= "temperature";
     static constexpr int id = 0;
  };

  template<> struct Conversion<kelvin, rankine>
  {
     using scale = factor_t<
        std::ratio<9, 5>>;
  };

}

#include "extensions/temperatureExtras.h"

#endif // STATIC_DIMENSION_TEMPERATURE_H