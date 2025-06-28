#ifndef STATIC_DIMENSION_temperature_IMPL_H
#define STATIC_DIMENSION_temperature_IMPL_H

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the temperature type tag.
   struct temperatureType {};

   /// @brief Represents the primary unit for temperature.
   struct kelvin;

   using Primarytemperature = kelvin;

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
      using Primary = Primarytemperature;
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

   template<typename RepOrUnit = double, typename MaybeUnit = void>
   class temperature;

   /// @brief Represents a dimension type for temperature.
   /// @tparam Unit The primary unit type.
   template<rep_type Rep, is_temperature_unit Unit>
   class temperature<Rep, Unit> : public base_dimension_impl<Rep, unit_exponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr temperature() : base_dimension_impl<Rep, unit_exponent<Unit>>::base_dimension_impl(0.0) {}

      /// @brief Constructs a temperature object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr temperature(double val) : base_dimension_impl<Rep, unit_exponent<Unit>>::base_dimension_impl(val) {}

      /// @brief Constructs a temperature object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<base_dimension_impl<Rep, unit_exponent<Unit>>, base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr temperature(const base_dimension_impl<Rep, Ts...>& base) : base_dimension_impl<Rep, unit_exponent<Unit>>::base_dimension_impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_temperature_unit Unit>
   class temperature<Unit, void> : public temperature<double, Unit> {
   public:
      using temperature<double, Unit>::temperature;
   };

   /// @brief Deduction guide for temperature constructor with base_dimension.
   /// @tparam temperatureUnit The unit type.
   template<is_temperature Dim>
   temperature(Dim) -> temperature<typename Dim::rep, DimExtractor<temperatureType, Dim>>;
}

#endif // STATIC_DIMENSION_temperature_IMPL_H