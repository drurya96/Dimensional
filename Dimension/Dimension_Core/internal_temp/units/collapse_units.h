#ifndef DIMENSIONAL_COLLAPSE_UNITS_H
#define DIMENSIONAL_COLLAPSE_UNITS_H

#include <type_traits>

#include "unit_exponent.h"
#include "../TemplateUtils/GenericUtils.h" // filter_tuple
#include "../ratio/ratio_utils.h"
#include "../TupleHandling.h"
#include "unit_decomposition.h"

namespace dimension
{
   namespace simplification
   {
      // ============================================================
      // ===================== Combine Power ========================
      // ============================================================

      template<typename IncomingUnit, typename... Units>
      struct CombinePower;

      template<typename IncomingUnit, typename... Units>
      struct CombinePower<IncomingUnit, std::tuple<Units...>>
      {
      private:
         // Fold over matching unit exponents
         using combined_exponent = ratio_add_t<
         std::conditional_t<
               (
                  std::is_same_v<typename IncomingUnit::unit, typename Units::unit> &&
                  std::is_same_v<typename IncomingUnit::label, typename Units::label>
               ),
               typename Units::exponent,
               std::ratio<0>
         >...
      >;

      public:
         using unit = unit_exponent<typename IncomingUnit::unit, combined_exponent::num, combined_exponent::den, typename IncomingUnit::label>;
      };
   

      // ============================================================
      // ====================== Unit Present ========================
      // ============================================================

      template<typename IncomingUnit, typename Tuple>
      struct UnitPresent;

      template<typename IncomingUnit, typename... Units>
      struct UnitPresent<IncomingUnit, std::tuple<Units...>>
         : std::bool_constant<(... || (
            std::is_same_v<typename IncomingUnit::unit, typename Units::unit> &&
            std::is_same_v<typename IncomingUnit::label, typename Units::label>
         ))> {};

    template<class Unit, class Tuple>
    inline constexpr bool UnitPresent_v = simplification::UnitPresent<Unit, Tuple>::value;

    // TODO: This probably doens't belong here...
    template<class Accum, class Unit, class FullTuple>
    using append_once_t =
    std::conditional_t<
        UnitPresent_v<Unit, Accum>,
        Accum,
        append_t<Accum, typename simplification::CombinePower<Unit, FullTuple>::unit>
    >;

   }

   // ============================================================
   // ===================== Remove Zeroes ========================
   // ============================================================

   template<typename E>
   struct is_nonzero_unit : std::bool_constant<E::exponent::num != 0> {};

   template<typename Tuple>
   struct RemoveZeros
   {
      using units = filter_tuple_t<is_nonzero_unit, Tuple>;
   };

   // ============================================================
   // ================== Initial Simplifier ======================
   // ============================================================

    template<class InputTuple>
    struct collapse_units;

    template<class... Units>
    struct collapse_units<std::tuple<Units...>> {
    private:
        using flattened = typename unit_decomposition<std::tuple<Units...>>::units;

        // Binary metafunction for the fold: F<Accum, Unit> -> ::type
        template<class Accum, class Unit>
        struct combine_unique_from_flattened {
            using type = simplification::append_once_t<Accum, Unit, flattened>;
        };



    public:
        // Make this private, only public to validate a quick test
        using combined =
            fold_over_tuple_with_state_t<
            flattened,
            std::tuple<>,
            combine_unique_from_flattened
            >;

        using type = typename RemoveZeros<combined>::units;
    };

   template<typename Tuple>
   using collapse_units_t = typename collapse_units<Tuple>::type;


} // end Dimension

#endif // DIMENSIONAL_COLLAPSE_UNITS_H
