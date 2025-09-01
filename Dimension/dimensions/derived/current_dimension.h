#ifndef STATIC_DIMENSION_CURRENT_H
#define STATIC_DIMENSION_CURRENT_H

#include "Dimension_Impl/DerivedDimensions/current_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  struct amperes
  {
     using units = std::tuple<unit_exponent<coulombs, 1, 1>, unit_exponent<seconds, -1, 1>>;
  };

  template<> struct unit_traits<amperes>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<coulombs, 1, 1>, unit_exponent<seconds, -1, 1>>;
  };
}


#endif // STATIC_DIMENSION_CURRENT_H