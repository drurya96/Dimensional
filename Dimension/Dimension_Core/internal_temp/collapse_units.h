#ifndef DIMENSIONAL_COLLAPSE_UNITS_H
#define DIMENSIONAL_COLLAPSE_UNITS_H

#include <type_traits>

#include "unit_exponent.h"
#include "TemplateUtils/GenericUtils.h" // filter_tuple
#include "TemplateUtils/RatioUtils.h" // filter_tuple
#include "TupleHandling.h"
#include "FundamentalUnitExtractor.h"

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
         using combined_exponent = ratio_add_fold_t<
         std::conditional_t<
               std::is_same_v<IncomingUnit, typename Units::unit>,
               typename Units::exponent,
               std::ratio<0>
         >...
      >;

      public:
         using unit = unit_exponent<IncomingUnit, combined_exponent::num, combined_exponent::den>;
      };
   

      // ============================================================
      // ====================== Unit Present ========================
      // ============================================================

      template<typename IncomingUnit, typename Tuple>
      struct UnitPresent;

      template<typename IncomingUnit, typename... Units>
      struct UnitPresent<IncomingUnit, std::tuple<Units...>>
         : std::bool_constant<(... || std::is_same_v<IncomingUnit, typename Units::unit>)> {};

   }

   // ============================================================
   // ===================== Remove Zeroes ========================
   // ============================================================

   template<typename E>
   struct is_nonzero_unit : std::bool_constant<E::exponent::num != 0> {};

   template<typename Tuple>
   struct RemoveZeros
   {
      using units = typename filter_tuple<Tuple, is_nonzero_unit>::type;
   };

   // ============================================================
   // ================== Initial Simplifier ======================
   // ============================================================

   template<typename InputTuple>
   struct collapse_units;
   
   template<typename... Units>
   struct collapse_units<std::tuple<Units...>>
   {
   private:
      using flattened = typename FundamentalUnitExtractor<std::tuple<Units...>>::units;
      
      template<typename FullTuple>
      struct CombineUniqueFrom {
          template<typename Accum, typename Unit>
          struct apply {      
              using type = std::conditional_t<
                  simplification::UnitPresent<typename Unit::unit, Accum>::value,
                  Accum,
                  tuple_cat_t<
                      Accum,
                      std::tuple<typename simplification::CombinePower<typename Unit::unit, FullTuple>::unit>
                  >
              >;
          };
      };

       using combined = fold_over_tuple_with_state<flattened,std::tuple<>, CombineUniqueFrom<flattened>::template apply>::type;
      
   public:
       using units = typename RemoveZeros<combined>::units;
   };

} // end Dimension

#endif // DIMENSIONAL_COLLAPSE_UNITS_H
