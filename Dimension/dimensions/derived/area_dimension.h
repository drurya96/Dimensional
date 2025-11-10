#ifndef STATIC_DIMENSION_AREA_H
#define STATIC_DIMENSION_AREA_H

#include "Dimension_Impl/DerivedDimensions/area_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  // Helper fundamental units used only to compose derived units
  struct acre_length{};

  template<> struct unit_traits<acre_length>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Acrelength";
     static constexpr std::string_view abbr   = "Acrelength";
     static constexpr std::string_view dimName= "area";
     static constexpr int id = 0;
  };

template<> struct Conversion<acre_length, meters>
{
  using scale = factor_t<
    std::ratio<1, 1>,
    std::tuple<ratio_exponent<std::ratio<3, 1>, 2, 1>, ratio_exponent<std::ratio<5, 1>, -4, 1>, ratio_exponent<std::ratio<11, 1>, 1, 1>, ratio_exponent<std::ratio<127, 1>, 1, 1>, ratio_exponent<std::ratio<10, 1>, 1, 2>>>;
};

  struct hectares
  {
     using units = std::tuple<unit_exponent<hecto_meters, 2, 1>>;
  };

  template<> struct unit_traits<hectares>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<hecto_meters, 2, 1>>;
  };
  struct acres
  {
     using units = std::tuple<unit_exponent<acre_length, 2, 1>>;
  };

  template<> struct unit_traits<acres>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<acre_length, 2, 1>>;
  };
}


#endif // STATIC_DIMENSION_AREA_H