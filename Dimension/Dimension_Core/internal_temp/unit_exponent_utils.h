#ifndef DIMENSIONAL_UNIT_EXPONENT_UTILS_H
#define DIMENSIONAL_UNIT_EXPONENT_UTILS_H

#include <ratio>
#include <tuple>
#include <type_traits>

#include "unit_exponent.h"
#include "unit_filter.h" // find_unit_by_dimension

namespace dimension
{

   // ============================================================
   // ==================== Subtract Tuples =======================
   // ============================================================

   template<typename TupleA, typename TupleB>
   struct Subtractunit_exponents;

   // Empty base case
   template<>
   struct Subtractunit_exponents<std::tuple<>, std::tuple<>> {
      using type = std::tuple<>;
   };

   // General case: A and B are std::tuple<unit_exponent<...>...>
   template<typename... UnitsA, typename... UnitsB>
   struct Subtractunit_exponents<std::tuple<UnitsA...>, std::tuple<UnitsB...>> {
   private:
      template<typename UnitA>
      struct subtract_one {
         using matching = find_unit_by_dimension<UnitA, std::tuple<UnitsB...>>;
         
         using result = std::conditional_t<
               matching::found,
               unit_exponent<
                  typename UnitA::unit,
                  std::ratio_subtract<
                     typename UnitA::exponent,
                     typename matching::type::exponent
                  >::num,
                  std::ratio_subtract<
                     typename UnitA::exponent,
                     typename matching::type::exponent
                  >::den
               >,
               UnitA
         >;
      };

   public:
      using type = tuple_cat_t<
         std::tuple<typename subtract_one<UnitsA>::result>...
      >;
   };

   template<class T>
   struct is_unit_exponents_trait
   : std::bool_constant<are_unit_exponents<std::remove_cvref_t<T>>> {};

   template<class... Ts>
   using extract_units_t  = filter_pack_tuple_t<is_unit_exponents_trait, Ts...>;

}

#endif // DIMENSIONAL_UNIT_EXPONENT_UTILS_H
