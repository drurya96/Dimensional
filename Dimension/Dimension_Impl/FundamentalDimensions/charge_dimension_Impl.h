#ifndef STATIC_DIMENSION_CHARGE_IMPL_H
#define STATIC_DIMENSION_CHARGE_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension_impl.h"

namespace dimension
{
   /// @brief Represents the primary unit for charge.
   struct coulombs;

   using primary_charge = coulombs;

   /// @brief charge type tag.
   struct chargeType {
      using primary = primary_charge;
   };

   /// @brief Concept: a valid charge unit.
   template<typename T>
   concept is_charge_unit =
      is_unit<T> && std::is_same_v<unit_dimension_t<T>, chargeType>;

   /// @brief Concept: a charge dimension (exactly one charge unit after simplification).
   template<typename T>
   concept is_charge =
      std::tuple_size_v<simplified_units_t<typename T::units>> == 1 &&
      is_charge_unit<typename std::tuple_element_t<0, simplified_units_t<typename T::units>>::unit>;

   /// @brief Get a charge object in unit T.
   template<is_charge_unit T, is_charge DimType>
   constexpr typename DimType::rep get_charge_as(DimType obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class charge;

   /// @brief charge dimension type.
   template<rep_type Rep, is_charge_unit Unit, is_coefficient... Cs>
   class charge<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {
      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructs to zero.
      constexpr charge() : impl(0.0) {}

      /// @brief Construct with a numeric value.
      explicit constexpr charge(double val) : impl(val) {}

      // perfect-forward ctor so factory can pass symbols
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr charge(V&& v, Ds... ds)
         : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr charge(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src)
         : impl(src) {}

      /// @brief Construct from another base_dimension of same charge.
      template<typename... Ts>
      requires is_charge<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between same-dimension types is intentional.
      // cppcheck-suppress noExplicitConstructor
      constexpr charge(const base_dimension_impl<Rep, Ts...>& base)
         : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_charge_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   [[maybe_unused]]
   constexpr auto make_charge(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return charge<Rep, U, Cs...>(value);
   }

   template<is_charge_unit U, is_coefficient... Cs>
   [[maybe_unused]]
   constexpr auto make_charge(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return charge<double, U, Cs...>(1.0);   // 1 × coeffs
   }

   template<is_charge_unit Unit, is_coefficient... Cs>
   class charge<Unit, Cs...> : public charge<double, Unit, Cs...>
   {
   public:
      using charge<double, Unit, Cs...>::charge;
   };

   /// @brief Deduction guides
   template<is_charge Dim>
   charge(Dim) -> charge<
      typename Dim::rep,
      simplified_unit_filter<chargeType, typename Dim::units>>;

   template<rep_type Rep, is_charge_unit Unit, is_coefficient... Cs>
   charge(const charge<Rep, Unit, Cs...>&)
      -> charge<Rep, Unit, Cs...>;

   template<rep_type R, is_charge_unit U, is_coefficient... Cs>
   charge(base_dimension_impl<R, unit_exponent<U>, Cs...>)
      -> charge<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_CHARGE_IMPL_H