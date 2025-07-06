#ifndef STATIC_DIMENSION_mass_IMPL_H
#define STATIC_DIMENSION_mass_IMPL_H

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the mass type tag.
   struct massType {};

   /// @brief Represents the primary unit for mass.
   struct grams;

   using Primarymass = grams;

   /// @brief Concept to check if a type is a valid mass unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_mass_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, massType>;

   /// @brief Base class for mass units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct massUnit : public BaseUnit<Unit, Name, Abbreviation, "mass", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = massType;

      /// @brief The primary unit type for this dimension.
      using Primary = Primarymass;
   };

   /// @brief Concept to check if a type is a mass dimension.
   /// @tparam T The type to check.
   template<typename T>
   concept is_mass = requires {
      typename decltype(FullSimplify(T{}))::units;
   } &&
   std::tuple_size_v<typename decltype(FullSimplify(T{}))::units> == 1 &&
   is_mass_unit<typename std::tuple_element_t<0, typename decltype(FullSimplify(T{}))::units>::unit>;

   /// @brief Retrieves the value of a mass object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The mass object.
   /// @return The value in the specified unit.
   template<is_mass_unit T>
   constexpr PrecisionType get_mass_as(/*mass_type*/ auto obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename RepOrUnit = double, typename MaybeUnit = void, is_coefficient... Cs>
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
      constexpr mass(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src) : impl(src) {}

      /// @brief Constructs a mass object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<impl, base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr mass(const base_dimension_impl<Rep, Ts...>& base) : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_mass_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   constexpr auto make_mass(Rep value, Cs... coeffs)
   {
      return mass<Rep, U, Cs...>(value, coeffs...);
   }

   template<is_mass_unit U, is_coefficient... Cs>
   constexpr auto make_mass(Cs... coeffs)
   {
      return mass<double, U, Cs...>(1.0, coeffs...);   // 1 × coeffs
   }

   template<is_mass_unit Unit, is_coefficient... Cs>
   class mass<Unit, void, Cs...> : public mass<double, Unit, Cs...> {
   public:
      using mass<double, Unit, Cs...>::mass;
   };

   /// @brief Deduction guide for mass constructor with base_dimension.
   /// @tparam massUnit The unit type.
   template<is_mass Dim>
   mass(Dim) -> mass<typename Dim::rep, DimExtractor<massType, Dim>>;

   template<rep_type Rep, is_mass_unit Unit, is_coefficient... Cs>
   mass(const mass<Rep, Unit, Cs...>&) -> mass<Rep, Unit, Cs...>;

   template<rep_type R, is_mass_unit U, is_coefficient... Cs>
   mass(base_dimension_impl<R, unit_exponent<U>, Cs...>) -> mass<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_mass_IMPL_H