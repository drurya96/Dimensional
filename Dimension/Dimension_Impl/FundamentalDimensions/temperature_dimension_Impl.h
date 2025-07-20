#ifndef STATIC_DIMENSION_TEMPERATURE_IMPL_H
#define STATIC_DIMENSION_TEMPERATURE_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the temperature type tag.
   struct temperatureType {};

   /// @brief Represents the primary unit for temperature.
   struct kelvin;

   using primary_temperature = kelvin;

   /// @brief Concept to check if a type is a valid temperature unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_temperature_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, temperatureType>;

   /// @brief Base class for temperature units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct temperatureUnit : public BaseUnit<Unit, Name, Abbreviation, "temperature", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = temperatureType;

      /// @brief The primary unit type for this dimension.
      using Primary = primary_temperature;
   };

   /// @brief Concept to check if a type is a temperature dimension.
   /// @tparam T The type to check.
   template<typename T>
   concept is_temperature = requires {
      typename decltype(FullSimplify(T{}))::units;
   } &&
   std::tuple_size_v<typename decltype(FullSimplify(T{}))::units> == 1 &&
   is_temperature_unit<typename std::tuple_element_t<0, typename decltype(FullSimplify(T{}))::units>::unit>;

   /// @brief Retrieves the value of a temperature object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The temperature object.
   /// @return The value in the specified unit.
   template<is_temperature_unit T>
   constexpr PrecisionType get_temperature_as(/*temperature_type*/ auto obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename RepOrUnit = double, typename MaybeUnit = void, is_coefficient... Cs>
   class temperature;

   /// @brief Represents a dimension type for temperature.
   /// @tparam Unit The primary unit type.
   template<rep_type Rep, is_temperature_unit Unit, is_coefficient... Cs>
   class temperature<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {

      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructor initializing to zero.
      constexpr temperature() : impl(0.0) {}

      /// @brief Constructs a temperature object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr temperature(double val) : impl(val) {}

      /* perfect-forward ctor so factory can pass symbols --------------------- */
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr temperature(V&& v, Ds... ds) : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      constexpr temperature(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src) : impl(src) {}

      /// @brief Constructs a temperature object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<impl, base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr temperature(const base_dimension_impl<Rep, Ts...>& base) : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_temperature_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   constexpr auto make_temperature(Rep value, Cs... coeffs)
   {
      return temperature<Rep, U, Cs...>(value, coeffs...);
   }

   template<is_temperature_unit U, is_coefficient... Cs>
   constexpr auto make_temperature(Cs... coeffs)
   {
      return temperature<double, U, Cs...>(1.0, coeffs...);   // 1 × coeffs
   }

   template<is_temperature_unit Unit, is_coefficient... Cs>
   class temperature<Unit, void, Cs...> : public temperature<double, Unit, Cs...> {
   public:
      using temperature<double, Unit, Cs...>::temperature;
   };

   /// @brief Deduction guide for temperature constructor with base_dimension.
   /// @tparam temperatureUnit The unit type.
   template<is_temperature Dim>
   temperature(Dim) -> temperature<typename Dim::rep, DimExtractor<temperatureType, Dim>>;

   template<rep_type Rep, is_temperature_unit Unit, is_coefficient... Cs>
   temperature(const temperature<Rep, Unit, Cs...>&) -> temperature<Rep, Unit, Cs...>;

   template<rep_type R, is_temperature_unit U, is_coefficient... Cs>
   temperature(base_dimension_impl<R, unit_exponent<U>, Cs...>) -> temperature<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_TEMPERATURE_IMPL_H