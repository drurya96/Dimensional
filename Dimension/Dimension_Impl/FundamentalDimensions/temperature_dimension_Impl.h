#ifndef STATIC_DIMENSION_TEMPERATURE_IMPL_H
#define STATIC_DIMENSION_TEMPERATURE_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension_impl.h"

namespace dimension
{
   /// @brief Represents the primary unit for temperature.
   struct kelvin;

   using primary_temperature = kelvin;

   /// @brief temperature type tag.
   struct temperatureType {
      using primary = primary_temperature;
   };

   /// @brief Concept: a valid temperature unit.
   template<typename T>
   concept is_temperature_unit =
      is_unit<T> && std::is_same_v<unit_dimension_t<T>, temperatureType>;

   /// @brief Concept: a temperature dimension (exactly one temperature unit after simplification).
   template<typename T>
   concept is_temperature =
      std::tuple_size_v<simplified_units_t<typename T::units>> == 1 &&
      is_temperature_unit<typename std::tuple_element_t<0, simplified_units_t<typename T::units>>::unit>;

   /// @brief Get a temperature object in unit T.
   template<is_temperature_unit T, is_temperature DimType>
   constexpr typename DimType::rep get_temperature_as(DimType obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class temperature;

   /// @brief temperature dimension type.
   template<rep_type Rep, is_temperature_unit Unit, is_coefficient... Cs>
   class temperature<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {
      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructs to zero.
      constexpr temperature() : impl(0.0) {}

      /// @brief Construct with a numeric value.
      explicit constexpr temperature(double val) : impl(val) {}

      // perfect-forward ctor so factory can pass symbols
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr temperature(V&& v, Ds... ds)
         : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr temperature(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src)
         : impl(src) {}

      /// @brief Construct from another base_dimension of same temperature.
      template<typename... Ts>
      requires is_temperature<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between same-dimension types is intentional.
      // cppcheck-suppress noExplicitConstructor
      constexpr temperature(const base_dimension_impl<Rep, Ts...>& base)
         : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_temperature_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   [[maybe_unused]]
   constexpr auto make_temperature(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return temperature<Rep, U, Cs...>(value);
   }

   template<is_temperature_unit U, is_coefficient... Cs>
   [[maybe_unused]]
   constexpr auto make_temperature(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return temperature<double, U, Cs...>(1.0);   // 1 × coeffs
   }

   template<is_temperature_unit Unit, is_coefficient... Cs>
   class temperature<Unit, Cs...> : public temperature<double, Unit, Cs...>
   {
   public:
      using temperature<double, Unit, Cs...>::temperature;
   };

   /// @brief Deduction guides
   template<is_temperature Dim>
   temperature(Dim) -> temperature<
      typename Dim::rep,
      simplified_unit_filter<temperatureType, typename Dim::units>>;

   template<rep_type Rep, is_temperature_unit Unit, is_coefficient... Cs>
   temperature(const temperature<Rep, Unit, Cs...>&)
      -> temperature<Rep, Unit, Cs...>;

   template<rep_type R, is_temperature_unit U, is_coefficient... Cs>
   temperature(base_dimension_impl<R, unit_exponent<U>, Cs...>)
      -> temperature<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_TEMPERATURE_IMPL_H