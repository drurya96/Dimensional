#ifndef STATIC_DIMENSION_MASS_IMPL_H
#define STATIC_DIMENSION_MASS_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_unit.h"
#include "../../base_dimension_impl.h"

namespace dimension
{
   /// @brief Represents the mass type tag.
   struct massType {};

   /// @brief Represents the primary unit for mass.
   struct grams;

   using primary_mass = grams;

   /// @brief Concept to check if a type is a valid mass unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_mass_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<unit_dim_t<T>, massType>;

   /// @brief Base class for mass units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, int ID = 0>
   struct massUnit : public BaseUnit<Unit, ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = massType;

      /// @brief The primary unit type for this dimension.
      using Primary = primary_mass;
   };

   /// @brief Concept to check if a type is a mass dimension.
   /// @tparam T The type to check.
   template<typename T>
   concept is_mass = 
      std::tuple_size_v<simplified_units_t<typename T::units>> == 1 &&
      is_mass_unit<typename std::tuple_element_t<0, simplified_units_t<typename T::units>>::unit>;

   /// @brief Retrieves the value of a mass object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The mass object.
   /// @return The value in the specified unit.
   template<is_mass_unit T, is_mass DimType>
   constexpr DimType::rep get_mass_as(DimType obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class mass;

   /// @brief Represents a dimension type for mass.
   /// @tparam Unit The primary unit type.
   template<rep_type Rep, is_mass_unit Unit, is_coefficient... Cs>
   class mass<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {

      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructor initializing to zero.
      constexpr mass() : impl(0.0) {}

      /// @brief Constructs a mass object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr mass(double val) : impl(val) {}

      /* perfect-forward ctor so factory can pass symbols --------------------- */
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr mass(V&& v, Ds... ds) : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr mass(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src) : impl(src) {}

      /// @brief Constructs a mass object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires is_mass<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr mass(const base_dimension_impl<Rep, Ts...>& base) : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_mass_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_mass(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return mass<Rep, U, Cs...>(value);
   }

   template<is_mass_unit U, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_mass(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return mass<double, U, Cs...>(1.0);   // 1 × coeffs
   }

   template<is_mass_unit Unit, is_coefficient... Cs>
   class mass<Unit, Cs...> : public mass<double, Unit, Cs...> {
   public:
      using mass<double, Unit, Cs...>::mass;
   };

   /// @brief Deduction guide for mass constructor with base_dimension.
   /// @tparam massUnit The unit type.
   template<is_mass Dim>
   mass(Dim) -> mass<typename Dim::rep, simplified_unit_filter<massType, typename Dim::units>>;

   template<rep_type Rep, is_mass_unit Unit, is_coefficient... Cs>
   mass(const mass<Rep, Unit, Cs...>&) -> mass<Rep, Unit, Cs...>;

   template<rep_type R, is_mass_unit U, is_coefficient... Cs>
   mass(base_dimension_impl<R, unit_exponent<U>, Cs...>) -> mass<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_MASS_IMPL_H