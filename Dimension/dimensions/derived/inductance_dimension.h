#ifndef STATIC_DIMENSION_INDUCTANCE_H
#define STATIC_DIMENSION_INDUCTANCE_H

#include "Dimension_Impl/DerivedDimensions/inductance_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  struct henrys
  {
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<coulombs, -2, 1>>;
  };

  template<> struct unit_traits<henrys>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<kilo_grams, 1, 1>, unit_exponent<meters, 2, 1>, unit_exponent<coulombs, -2, 1>>;
  };
}


#endif // STATIC_DIMENSION_INDUCTANCE_H