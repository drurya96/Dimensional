#ifndef STATIC_DIMENSION_ELECTRIC_POTENTIAL_H
#define STATIC_DIMENSION_ELECTRIC_POTENTIAL_H

#include "Dimension_Impl/DerivedDimensions/electric_potential_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  struct volts
  {
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<seconds, -2, 1>, unit_exponent<coulombs, -1, 1>>;
  };

  template<> struct unit_traits<volts>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<seconds, -2, 1>, unit_exponent<coulombs, -1, 1>>;
  };
}


#endif // STATIC_DIMENSION_ELECTRIC_POTENTIAL_H