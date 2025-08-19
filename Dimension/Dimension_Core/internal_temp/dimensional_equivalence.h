#ifndef DIMENSIONAL_EQUIVALENCE_H
#define DIMENSIONAL_EQUIVALENCE_H

#include <concepts>
#include <type_traits>
#include <ratio>

#include "TemplateUtils/GenericUtils.h"

#include "TupleHandling.h"

#include "units/unit_decomposition.h"

#include "exponentiation/exponentiation.h"

#include "ratio/ratio_utils.h"
#include "utils.h"

#include "full_simplify.h"

#include "units/collapse_units.h"
#include "units/unit_exponent.h"

namespace dimension {
   namespace dimensional_equivalence
   {

      template<is_unit_exponent T, is_unit_exponent U>
      struct Convertibleunit_exponent
      {
         static constexpr bool value =
            is_same_dim<typename T::unit, typename U::unit>::value &&
            std::is_same_v<typename T::label, typename U::label> &&
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
      collapse_units_t<T1>,
      collapse_units_t<T2>
   >::value;

}

#endif // DIMENSIONAL_EQUIVALENCE_H
