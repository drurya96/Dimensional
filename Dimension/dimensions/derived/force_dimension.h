#ifndef STATIC_DIMENSION_FORCE_H
#define STATIC_DIMENSION_FORCE_H

#include "Dimension_Impl/DerivedDimensions/force_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  struct newtons
  {
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 1, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<newtons>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 1, 1>, unit_exponent<seconds, -2, 1>>;
  };
  struct pound_force
  {
     using units = std::tuple<unit_exponent<slugs, 1, 1>, unit_exponent<feet, 1, 1>, unit_exponent<seconds, -2, 1>>;
  };

  template<> struct unit_traits<pound_force>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<slugs, 1, 1>, unit_exponent<feet, 1, 1>, unit_exponent<seconds, -2, 1>>;
  };
}


#endif // STATIC_DIMENSION_FORCE_H