#ifndef DIMENSIONAL_EQUIVALENCE_H
#define DIMENSIONAL_EQUIVALENCE_H

#include <concepts>
#include <type_traits>
#include <ratio>

#include "TemplateUtils/RatioUtils.h"
#include "TemplateUtils/GenericUtils.h"

#include "TupleHandling.h"

#include "unit_decomposition.h"

#include "exponent_math.h"

#include "ratio_utils.h"
#include "utils.h"

#include "full_simplify.h"

#include "collapse_units.h"
#include "unit_exponent.h"

namespace dimension {
   namespace dimensional_equivalence
   {

      template<is_unit_exponent T, is_unit_exponent U>
      struct Convertibleunit_exponent
      {
         static constexpr bool value =
            is_same_dim<typename T::unit, typename U::unit>::value &&
            std::ratio_equal<typename T::exponent, typename U::exponent>::value;
      };

      template<typename Unit, typename Tuple>
      struct ContainsEquivalentUnit;
      
      template<typename Unit, typename First, typename... Rest>
      struct ContainsEquivalentUnit<Unit, std::tuple<First, Rest...>> {
         static constexpr bool value =
            Convertibleunit_exponent<Unit, First>::value ||
            ContainsEquivalentUnit<Unit, std::tuple<Rest...>>::value;
      };
      
      template<typename Unit>
      struct ContainsEquivalentUnit<Unit, std::tuple<>> {
         static constexpr bool value = false;
      };

      template<typename Tuple1, typename Tuple2>
      struct AreUnitTuplesEquivalent;
      
      template<typename... Units1, typename Tuple2>
      struct AreUnitTuplesEquivalent<std::tuple<Units1...>, Tuple2> {
         static constexpr bool value =
            (ContainsEquivalentUnit<Units1, Tuple2>::value && ...) &&
            (sizeof...(Units1) == std::tuple_size_v<Tuple2>);
      };
   }

   template<typename T, typename U>
   concept dimensionally_equivalent =
      dimensional_equivalence::AreUnitTuplesEquivalent<
         simplified_units_t<typename T::units>,
         simplified_units_t<typename U::units>
       >::value;

   template<typename T1, typename T2>
   concept same_unit_representation = unit_exponent_sets_equal<
      typename collapse_units<T1>::units,
      typename collapse_units<T2>::units
   >::value;

}

#endif // DIMENSIONAL_EQUIVALENCE_H
