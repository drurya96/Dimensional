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

   template<typename... Ts>
   class timespan;

   /// @brief timespan dimension type.
   template<rep_type Rep, is_timespan_unit Unit, is_coefficient... Cs>
   class timespan<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {
      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructs to zero.
      constexpr timespan() : impl(0.0) {}

      /// @brief Construct with a numeric value.
      explicit constexpr timespan(double val) : impl(val) {}

      // perfect-forward ctor so factory can pass symbols
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr timespan(V&& v, Ds... ds)
         : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr timespan(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src)
         : impl(src) {}

      /// @brief Construct from another base_dimension of same timespan.
      template<typename... Ts>
      requires is_timespan<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between same-dimension types is intentional.
      // cppcheck-suppress noExplicitConstructor
      constexpr timespan(const base_dimension_impl<Rep, Ts...>& base)
         : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_timespan_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   [[maybe_unused]]
   constexpr auto make_timespan(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return timespan<Rep, U, Cs...>(value);
   }

   template<is_timespan_unit U, is_coefficient... Cs>
   [[maybe_unused]]
   constexpr auto make_timespan(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return timespan<double, U, Cs...>(1.0);   // 1 × coeffs
   }

   template<is_timespan_unit Unit, is_coefficient... Cs>
   class timespan<Unit, Cs...> : public timespan<double, Unit, Cs...>
   {
   public:
      using timespan<double, Unit, Cs...>::timespan;
   };

   /// @brief Deduction guides
   template<is_timespan Dim>
   timespan(Dim) -> timespan<
      typename Dim::rep,
      simplified_unit_filter<timespanType, typename Dim::units>>;

   template<rep_type Rep, is_timespan_unit Unit, is_coefficient... Cs>
   timespan(const timespan<Rep, Unit, Cs...>&)
      -> timespan<Rep, Unit, Cs...>;

   template<rep_type R, is_timespan_unit U, is_coefficient... Cs>
   timespan(base_dimension_impl<R, unit_exponent<U>, Cs...>)
      -> timespan<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_TIMESPAN_IMPL_H