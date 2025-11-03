#ifndef DIMENSIONAL_FULL_SIMPLIFY_H
#define DIMENSIONAL_FULL_SIMPLIFY_H

#include <type_traits>

#include "../TupleHandling.h"
#include "../units/get_factor.h" // TODO: Only needed for do_conversion — may want to extract that
#include "../units/collapse_units.h"
#include "../ratio/ratio_utils.h"
#include "../utils.h"
#include "base_dimension_from_tuple.h"

namespace dimension
{

   // Forward declaration, consider dependency order
   template<typename From, typename To>
   struct Conversion;


   namespace simplification
   {



      // This should definitely live elsewhere...
      // ==================== Primary ====================
      // Default: no relationship -> identity ratio
      template<typename From, typename To>
      struct conversion_ratio {
      private:
         template<typename F, typename T>
         static consteval auto impl() {
            if constexpr (std::is_same_v<F,T>){
               return std::ratio<1>{};
            }
            else if constexpr (!is_same_dim_v<F,T>) {
               return std::ratio<1>{};
            }
            else if constexpr (HasConversion<F,T>) {
               return typename Conversion<F,T>::scale::ratio{};
            }
            else if constexpr (HasConversion<T,F>) {
               return inverse_ratio_t<typename Conversion<T,F>::scale::ratio>{};
            }
            else {
               return std::ratio_multiply<
                  typename conversion_ratio<F, unit_primary_t<F>>::type,
                  typename conversion_ratio<unit_primary_t<F>, T>::type
               >{};
            }
         }

      public:
         using type = decltype(impl<From,To>());
      };

      template<typename From, typename To>
      using conversion_ratio_t = typename conversion_ratio<From, To>::type;












      // ============================================================
      // =================== Convert All Units ======================
      // ============================================================

      template<typename TargetUnit, typename Tuple>
      struct Convert_All_Units;
      
      template<typename TargetUnit>
      struct Convert_All_Units<TargetUnit, std::tuple<>> {
         using units = std::tuple<>;
      
         using ratio = std::ratio<1>;

         using ratio_coefficients = std::tuple<>;

         static constexpr double Convert(double val) {
            return val;
         }
      };
      
      template<typename TargetUnit, typename Unit, typename... Rest>
      struct Convert_All_Units<TargetUnit, std::tuple<Unit, Rest...>> {
      private:
         static constexpr bool same_dim = is_same_dim<typename Unit::unit, typename TargetUnit::unit>::value &&
            std::is_same_v<typename Unit::label, typename TargetUnit::label>;
      
         using converted_unit = std::conditional_t<
            same_dim,
            unit_exponent<typename TargetUnit::unit, Unit::exponent::num, Unit::exponent::den>,
            Unit
         >;
      
         using tail_result = Convert_All_Units<TargetUnit, std::tuple<Rest...>>;
      
      public:
         using units = tuple_cat_t<
            std::tuple<converted_unit>,
            typename tail_result::units
         >;
      
         using TEST = conversion_ratio_t<typename Unit::unit, typename TargetUnit::unit>;

         // WRONG!!!
         // There's a problem in this logic, we don't account for whether Unit and Target have same/opposite signed exponents
         // We also don't account for the expont itself.. m^2 -> ft^2 != m -> ft

         // Added call to raise_ratio... but `N` might itself be a ratio...
         // In either case, this will be type explosion
         // But if I collapse here, then all simplification will collapse ratios...
         // I think I need to allow the explosion...
         //using ratio = std::ratio_multiply<
         //   raise_ratio_t<conversion_ratio_t<typename Unit::unit, typename TargetUnit::unit>, 1>,
         //   typename tail_result::ratio
         //>;
         
         static constexpr bool unit_ratio_integer = (Unit::exponent::den == 1);
         static constexpr int unit_ratio_int = unit_ratio_integer ? Unit::exponent::num : 1;

         // This is closer.. but we aren't there yet. This works when the power is an integer, but if its fractional we'll need to handle that in Convert...
         // We also still aren't handling symbols that are part of unit converions (i.e. pi for angles)

         // Instead, we'll use ratio_exponent which can be a coefficient - this keeps everything compile time. With this, Convert actually shouldn't be needed.
         // URGENT TODO: We'll need a way to evalautate ratio_exponents too.. that's not built yet.
         using ratio = std::ratio_multiply<
            std::conditional_t<
               unit_ratio_integer,
               raise_ratio_t<conversion_ratio_t<typename Unit::unit, typename TargetUnit::unit>, unit_ratio_int>,
               std::ratio<1>
            >,
            typename tail_result::ratio
         >;

         using ratio_coefficients = tuple_cat_t<
            std::conditional_t<
               unit_ratio_integer,
               std::tuple<>,
               std::tuple<ratio_exponent<conversion_ratio_t<typename Unit::unit, typename TargetUnit::unit>, Unit::exponent::num, Unit::exponent::den>>
            >,
            typename tail_result::ratio_coefficients
         >;

         // These shouldn't be needed now
         //static constexpr double evaluated_ratio = unit_ratio_integer ? 1.0 : 1.0/*evaluate here*/;

         //static constexpr double placeholder_ratio_double = ratio_v<conversion_ratio_t<typename Unit::unit, typename TargetUnit::unit>>;
         //static constexpr double placeholder_evaluated_ratio = 1.0;

         // End experimental region... ============

         // I think when all's done I actually won't need `Convert` anymore... the ratio should capture it all...
         // But because the ratio could be irrational (raised to a ratio power), I'll still need a way to modify `val`... tbd

         // Ok, I think a `ratio_exponent` is going to be strictly necessary...
         // This would have to be treated as a "coefficient" since it can't necessarily collapse with regular ratios.
         // So I need a way to check "can this collapse", and just apply the ratio if so - using the ratio_exponent should only be if all else fails
         // It might also be worth adding a final check to see if any other ratio_exponents can collapse together... maybe. Maybe not, I'm not sure yet.
         // The "base" really should never be the same since we do a first-pass simplification of like-types, so a final pass to simplify may not be helpful
         // This is gonna be quite an effort to build out, but I think its unavoidable.
         static constexpr double Convert(double val) {
            if constexpr (same_dim) {
                  val = details::do_conversion<typename TargetUnit::unit, Unit>(val);
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
      
         using ratio = std::ratio<1>;

         static constexpr double Convert(double val) {
            return val;
         }
      };
      
      template<typename Unit, typename... Units>
      struct Convert_All_Dims<std::tuple<Unit, Units...>> {
      private:
         using tail = std::tuple<Units...>;
      
         // Convert all subsequent units to match Unit's unit
         using converted = Convert_All_Units<Unit, tail>;
      
         // Recurse on the updated tail
         using next = Convert_All_Dims<typename converted::units>;
      
      public:
         using units = tuple_cat_t<
            std::tuple<Unit>,
            typename next::units
         >;
      

         using a = typename converted::ratio;
         using b = typename next::ratio;

         using ratio = std::ratio_multiply<
            typename converted::ratio,
            typename next::ratio
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
         using initial = collapse_units_t<std::tuple<Units...>>;
         
         

      public:
      using after_conversion = Convert_All_Dims<initial>;
      using ratio = typename after_conversion::ratio;
         static constexpr double convert_scalar(double val)
         {
            return after_conversion::Convert(val);
         }

         using units = collapse_units_t<typename after_conversion::units>;

         // TODO: Now I think I just need to get `ratio` into `dim`...
         //using dim   = typename base_dimension_from_tuple<units>::dim;
         using dim   = typename base_dimension_from_tuple<
            double,
            ratio,
            units,
            std::tuple<>
         >::dim;
      };

   }

   template<typename UnitTuple>
   using simplified_units_t = typename simplification::SimplifiedDimension<UnitTuple>::units;

   template<typename Dim>
   constexpr auto full_simplify(Dim input) {
      using Simplified = simplification::SimplifiedDimension<typename Dim::units>;
      return typename Simplified::dim{
         input.get_raw()
         //Simplified::convert_scalar(input.get_raw())
      };
   }

} // end Dimension

#endif // DIMENSIONAL_FULL_SIMPLIFY_H
