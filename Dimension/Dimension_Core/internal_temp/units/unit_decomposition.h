#ifndef DIMENSION_UNIT_DECOMPOSITION_H
#define DIMENSION_UNIT_DECOMPOSITION_H

#include "../TupleHandling.h"
#include "unit_exponent.h"
#include "new_unit_stuff.h"

namespace dimension
{

   template<typename... Units>
   struct unit_decomposition;

   template<>
   struct unit_decomposition<std::tuple<>>
   {
      using units = std::tuple<>;
   };

   template<typename Unit>
   struct unit_decomposition<std::tuple<Unit>>
   {
      using units = raise_all_unit_exponents_t<
         label_all_unit_exponents_t<unit_units_t<typename Unit::unit>, typename Unit::label>,
         typename Unit::exponent>;
   };

   template<typename Unit, typename... Units>
   struct unit_decomposition<std::tuple<Unit, Units...>>
   {
      using units = tuple_cat_t<typename unit_decomposition<std::tuple<Unit>>::units,
         typename unit_decomposition<std::tuple<Units...>>::units>;
   };

} // end Dimension

#endif // DIMENSION_UNIT_DECOMPOSITION_H