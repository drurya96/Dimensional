#ifndef STATIC_DIMENSION_ENERGY_H
#define STATIC_DIMENSION_ENERGY_H

#include "Dimension_Impl/DerivedDimensions/energy_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  // Helper fundamental units used only to compose derived units
  struct calorie_mass{};

  template<> struct unit_traits<calorie_mass>
  {
     using dimension = massType;
     static constexpr std::string_view name   = "Caloriemass";
     static constexpr std::string_view abbr   = "Caloriemass";
     static constexpr std::string_view dimName= "energy";
     static constexpr int id = 0;
  };

  template<> struct Conversion<calorie_mass, grams>
  {
     using scale = factor_t<
        std::ratio<4184, 1>>;
  };

  struct joules
  {
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<joules>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct foot_pounds
  {
     using units = std::tuple<unit_exponent<slugs, 1, 1>, unit_exponent<feet, 2, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<foot_pounds>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<slugs, 1, 1>, unit_exponent<feet, 2, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct btus
  {
     using units = std::tuple<unit_exponent<slugs, 1, 1>, unit_exponent<feet, 2, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<btus>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<slugs, 1, 1>, unit_exponent<feet, 2, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct calories
  {
     using units = std::tuple<unit_exponent<calorie_mass, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<calories>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<calorie_mass, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<seconds, -2, 1>>;
  };
}


#endif // STATIC_DIMENSION_ENERGY_H