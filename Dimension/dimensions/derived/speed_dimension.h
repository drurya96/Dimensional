#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "Dimension_Impl/DerivedDimensions/speed_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  struct knots
  {
     using units = std::tuple<unit_exponent<nautical_miles, 1, 1>, unit_exponent<hours, -1, 1>>;
  };

  template<> struct unit_traits<knots>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<nautical_miles, 1, 1>, unit_exponent<hours, -1, 1>>;
  };
}


#endif // STATIC_DIMENSION_SPEED_H