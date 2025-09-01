#ifndef STATIC_DIMENSION_RESISTANCE_H
#define STATIC_DIMENSION_RESISTANCE_H

#include "Dimension_Impl/DerivedDimensions/resistance_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  struct ohms
  {
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<seconds, -1, 1>, unit_exponent<coulombs, -2, 1>>;
  };

  template<> struct unit_traits<ohms>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<seconds, -1, 1>, unit_exponent<coulombs, -2, 1>>;
  };
}


#endif // STATIC_DIMENSION_RESISTANCE_H