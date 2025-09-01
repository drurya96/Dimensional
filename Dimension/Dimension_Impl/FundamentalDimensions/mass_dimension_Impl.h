#ifndef STATIC_DIMENSION_MASS_IMPL_H
#define STATIC_DIMENSION_MASS_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension_impl.h"

namespace dimension
{
   /// @brief Represents the primary unit for mass.
   struct grams;

   using primary_mass = grams;

   /// @brief mass type tag.
   struct massType {
      using primary = primary_mass;
   };

   /// @brief Concept: a valid mass unit.
   template<typename T>
   concept is_mass_unit =
      is_unit<T> && std::is_same_v<unit_dimension_t<T>, massType>;

   /// @brief Concept: a mass dimension (exactly one mass unit after simplification).
   template<typename T>
   concept is_mass =
      std::tuple_size_v<simplified_units_t<typename T::units>> == 1 &&
      is_mass_unit<typename std::tuple_element_t<0, simplified_units_t<typename T::units>>::unit>;

   /// @brief Get a mass object in unit T.
   template<is_mass_unit T, is_mass DimType>
   constexpr typename DimType::rep get_mass_as(DimType obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class mass;

   /// @brief mass dimension type.
   template<rep_type Rep, is_mass_unit Unit, is_coefficient... Cs>
   class mass<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {
      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructs to zero.
      constexpr mass() : impl(0.0) {}

      /// @brief Construct with a numeric value.
      explicit constexpr mass(double val) : impl(val) {}

      // perfect-forward ctor so factory can pass symbols
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr mass(V&& v, Ds... ds)
         : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr mass(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src)
         : impl(src) {}

      /// @brief Construct from another base_dimension of same mass.
      template<typename... Ts>
      requires is_mass<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between same-dimension types is intentional.
      // cppcheck-suppress noExplicitConstructor
      constexpr mass(const base_dimension_impl<Rep, Ts...>& base)
         : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_mass_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   [[maybe_unused]]
   constexpr auto make_mass(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return mass<Rep, U, Cs...>(value);
   }

   template<is_mass_unit U, is_coefficient... Cs>
   [[maybe_unused]]
   constexpr auto make_mass(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return mass<double, U, Cs...>(1.0);   // 1 × coeffs
   }

   template<is_mass_unit Unit, is_coefficient... Cs>
   class mass<Unit, Cs...> : public mass<double, Unit, Cs...>
   {
   public:
      using mass<double, Unit, Cs...>::mass;
   };

   /// @brief Deduction guides
   template<is_mass Dim>
   mass(Dim) -> mass<
      typename Dim::rep,
      simplified_unit_filter<massType, typename Dim::units>>;

   template<rep_type Rep, is_mass_unit Unit, is_coefficient... Cs>
   mass(const mass<Rep, Unit, Cs...>&)
      -> mass<Rep, Unit, Cs...>;

   template<rep_type R, is_mass_unit U, is_coefficient... Cs>
   mass(base_dimension_impl<R, unit_exponent<U>, Cs...>)
      -> mass<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_MASS_IMPL_H