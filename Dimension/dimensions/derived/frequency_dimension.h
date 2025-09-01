#ifndef STATIC_DIMENSION_FREQUENCY_H
#define STATIC_DIMENSION_FREQUENCY_H

#include "Dimension_Impl/DerivedDimensions/frequency_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  struct hertz
  {
     using units = std::tuple<unit_exponent<seconds, -1, 1>>;
  };

  template<> struct unit_traits<hertz>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<seconds, -1, 1>>;
  };
}


#endif // STATIC_DIMENSION_FREQUENCY_H