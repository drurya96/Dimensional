#ifndef STATIC_DIMENSION_TIMESPAN_IMPL_H
#define STATIC_DIMENSION_TIMESPAN_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension_impl.h"

namespace dimension
{
   /// @brief Represents the primary unit for timespan.
   struct seconds;

   using primary_timespan = seconds;

   /// @brief timespan type tag.
   struct timespanType {
      using primary = primary_timespan;
   };

   /// @brief Concept: a valid timespan unit.
   template<typename T>
   concept is_timespan_unit =
      is_unit<T> && std::is_same_v<unit_dimension_t<T>, timespanType>;

   /// @brief Concept: a timespan dimension (exactly one timespan unit after simplification).
   template<typename T>
   concept is_timespan =
      std::tuple_size_v<simplified_units_t<typename T::units>> == 1 &&
      is_timespan_unit<typename std::tuple_element_t<0, simplified_units_t<typename T::units>>::unit>;

   /// @brief Get a timespan object in unit T.
   template<is_timespan_unit T, is_timespan DimType>
   constexpr typename DimType::rep get_timespan_as(DimType obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   // Primary template
   template<typename... Ts>
   class timespan;

   // Helper: map (Rep, Unit, CoeffTuple) → base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   template<rep_type Rep, is_timespan_unit Unit, class CoeffTuple>
   struct timespan_impl_from_tuple;

   template<rep_type Rep, is_timespan_unit Unit, class... Cs>
   struct timespan_impl_from_tuple<Rep, Unit, std::tuple<Cs...>>
   {
      using type = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;
   };

   /// @brief Primary specialization: Rep, Unit, CoeffTuple (symbolic-capable form).
   template<rep_type Rep, is_timespan_unit Unit, class CoeffTuple>
   class timespan<Rep, Unit, CoeffTuple>
      : public timespan_impl_from_tuple<Rep, Unit, CoeffTuple>::type
   {
      using impl        = typename timespan_impl_from_tuple<Rep, Unit, CoeffTuple>::type;
      using coeff_tuple = CoeffTuple;

   public:
      using rep          = Rep;
      using unit         = Unit;
      using coefficients = coeff_tuple;

      /// @brief Default constructs to zero.
      constexpr timespan() : impl(0.0) {}

      /// @brief Construct with a numeric value.
      explicit constexpr timespan(double val) : impl(val) {}

      // perfect-forward ctor so factory can pass symbols / coefficients
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V> &&
               std::same_as<std::tuple<Ds...>, coeff_tuple>
      explicit constexpr timespan(V&& v, Ds... ds)
         : impl(static_cast<Rep>(std::forward<V>(v)), ds...)
      {}

      /// @brief Construct from matching impl (preserves coefficients).
      constexpr timespan(const impl& src)
         : impl(src)
      {}

      /// @brief CTAD / symbolic ctor:
      /// Takes any timespan dimension Dim whose simplified dimension exactly matches impl,
      /// and copies the raw value without applying any conversion factor.
      template<class Dim>
      requires is_timespan<Dim> &&
               std::is_same_v<simplify_dimension_t<Dim>, impl>
      // Implicit on purpose for CTAD-based construction.
      // cppcheck-suppress noExplicitConstructor
      constexpr timespan(const Dim& dim)
         : impl(dim.get_raw())
      {}
   };

   /// @brief Partial specialization: Rep, Unit → canonical mode (no coeff tuple in the type).
   template<rep_type Rep, is_timespan_unit Unit>
   class timespan<Rep, Unit> : public timespan<Rep, Unit, std::tuple<>>
   {
      using base = timespan<Rep, Unit, std::tuple<>>;

   public:
      using base::base;
      using rep  = Rep;
      using unit = Unit;

      /// @brief Canonicalizing ctor from any timespan dimension.
      /// Applies get_dimension_as<unit_exponent<Unit>> and loses symbolic factors.
      template<typename... Ts>
      requires is_timespan<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between same-dimension types is intentional.
      // cppcheck-suppress noExplicitConstructor
      constexpr timespan(const base_dimension_impl<Rep, Ts...>& base_dim)
         : base(get_dimension_as<unit_exponent<Unit>>(base_dim))
      {}
   };

   /// @brief Partial specialization: Unit only → default Rep = double.
   template<is_timespan_unit Unit>
   class timespan<Unit> : public timespan<double, Unit>
   {
   public:
      using timespan<double, Unit>::timespan;
      using rep  = double;
      using unit = Unit;
   };

   // --- Factories ---------------------------------------------------------

   template<is_timespan_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   [[maybe_unused]]
   constexpr auto make_timespan(Rep value, Cs... coeffs)
   {
      using coeff_tuple = std::tuple<Cs...>;
      ignore_unused(coeffs...); // coeffs are still primarily type-level here
      return timespan<Rep, U, coeff_tuple>(value, coeffs...);
   }

   template<is_timespan_unit U, is_coefficient... Cs>
   [[maybe_unused]]
   constexpr auto make_timespan(Cs... coeffs)
   {
      using coeff_tuple = std::tuple<Cs...>;
      ignore_unused(coeffs...);
      // 1 × coeffs
      return timespan<double, U, coeff_tuple>(1.0, coeffs...);
   }

   // --- Simplification → timespan mapping for CTAD ------------------------

   template<class Dim>
   struct simplify_timespan_dim; // primary

   template<typename R, class UExp, class... Cs>
   struct simplify_timespan_dim<base_dimension_impl<R, UExp, Cs...>> {
      static_assert(is_timespan_unit<typename UExp::unit>);
      using rep         = R;
      using unit        = typename UExp::unit;
      using coeff_tuple = std::tuple<Cs...>;
   };

   template<class Dim>
   using simplify_timespan_dim_t =
      simplify_timespan_dim<simplify_dimension_t<Dim>>;

   // --- Deduction guides --------------------------------------------------

   // 1) General "expression" CTAD:
   //    - Simplify the dimension type
   //    - Map to timespan<Rep, Unit, CoeffTuple>
   template<class Dim>
   requires is_timespan<Dim>
   timespan(Dim) -> timespan<
      typename simplify_timespan_dim_t<Dim>::rep,
      typename simplify_timespan_dim_t<Dim>::unit,
      typename simplify_timespan_dim_t<Dim>::coeff_tuple>;

   // 2) Identity CTAD from an existing timespan<Rep, Unit, CoeffTuple>.
   template<rep_type Rep, is_timespan_unit Unit, class CoeffTuple>
   timespan(const timespan<Rep, Unit, CoeffTuple>&)
      -> timespan<Rep, Unit, CoeffTuple>;

   // 3) CTAD from a base_dimension_impl that already has simplified units + coefficients.
   template<rep_type R, is_timespan_unit U, is_coefficient... Cs>
   timespan(base_dimension_impl<R, unit_exponent<U>, Cs...>)
      -> timespan<R, U, std::tuple<Cs...>>;
}

#endif // STATIC_DIMENSION_TIMESPAN_IMPL_H
