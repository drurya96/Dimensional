#ifndef DIMENSIONAL_FULL_SIMPLIFY_H
#define DIMENSIONAL_FULL_SIMPLIFY_H

#include <type_traits>

#include "TupleHandling.h"
#include "get_factor.h" // TODO: Only needed for do_conversion — may want to extract that
#include "collapse_units.h"
#include "base_dimension_from_tuple.h"

namespace dimension
{
   namespace simplification
   {
      // ============================================================
      // =================== Convert All Units ======================
      // ============================================================

      template<typename TargetUnit, typename Tuple>
      struct Convert_All_Units;
      
      template<typename TargetUnit>
      struct Convert_All_Units<TargetUnit, std::tuple<>> {
         using units = std::tuple<>;
      
         static constexpr double Convert(double val) {
            return val;
         }
      };
      
      template<typename TargetUnit, typename Unit, typename... Rest>
      struct Convert_All_Units<TargetUnit, std::tuple<Unit, Rest...>> {
      private:
         static constexpr bool same_dim = is_same_dim<typename Unit::unit, TargetUnit>::value;
      
         using converted_unit = std::conditional_t<
            same_dim,
            unit_exponent<TargetUnit, Unit::exponent::num, Unit::exponent::den>,
            Unit
         >;
      
         using tail_result = Convert_All_Units<TargetUnit, std::tuple<Rest...>>;
      
      public:
         using units = tuple_cat_t<
            std::tuple<converted_unit>,
            typename tail_result::units
         >;
      
         static constexpr double Convert(double val) {
            if constexpr (same_dim) {
                  val = details::do_conversion<TargetUnit, Unit>(val);
            }
            return tail_result::Convert(val);
         }
      };

      // ============================================================
      // ==================== Convert All Dims ======================
      // ============================================================

      template<typename Tuple>
      struct Convert_All_Dims;
      
      template<>
      struct Convert_All_Dims<std::tuple<>> {
         using units = std::tuple<>;
      
         static constexpr double Convert(double val) {
            return val;
         }
      };
      
      template<typename Unit, typename... Units>
      struct Convert_All_Dims<std::tuple<Unit, Units...>> {
      private:
         using tail = std::tuple<Units...>;
      
         // Convert all subsequent units to match Unit's unit
         using converted = Convert_All_Units<typename Unit::unit, tail>;
      
         // Recurse on the updated tail
         using next = Convert_All_Dims<typename converted::units>;
      
      public:
         using units = tuple_cat_t<
            std::tuple<Unit>,
            typename next::units
         >;
      
         static constexpr double Convert(double val) {
            double after_conversion = converted::Convert(val);
            return next::Convert(after_conversion);
         }
      };


      template<typename Tuple>
      struct SimplifiedDimension;

      template<typename... Units>
      struct SimplifiedDimension<std::tuple<Units...>>
      {
      private:
         using initial = typename collapse_units<std::tuple<Units...>>::units;
         using after_conversion = Convert_All_Dims<initial>;

      public:
         static constexpr double convert_scalar(double val)
         {
            return after_conversion::Convert(val);
         }

         using units = typename collapse_units<typename after_conversion::units>::units;
         using dim   = typename base_dimension_from_tuple<units>::dim;
      };

   }

   template<typename UnitTuple>
   using simplified_units_t = typename simplification::SimplifiedDimension<UnitTuple>::units;

   template<typename Dim>
   constexpr auto full_simplify(Dim input) {
      using Simplified = simplification::SimplifiedDimension<typename Dim::units>;
      return typename Simplified::dim{
         Simplified::convert_scalar(input.template get_tuple_scalar<typename Dim::units>())
      };
   }

} // end Dimension

#endif // DIMENSIONAL_FULL_SIMPLIFY_H
