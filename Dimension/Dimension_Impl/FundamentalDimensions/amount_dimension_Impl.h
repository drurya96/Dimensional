#ifndef STATIC_DIMENSION_AMOUNT_IMPL_H
#define STATIC_DIMENSION_AMOUNT_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension_impl.h"

namespace dimension
{
   /// @brief Represents the primary unit for amount.
   struct moles;

   using primary_amount = moles;

   /// @brief amount type tag.
   struct amountType {
      using primary = primary_amount;
   };

   /// @brief Concept: a valid amount unit.
   template<typename T>
   concept is_amount_unit =
      is_unit<T> && std::is_same_v<unit_dimension_t<T>, amountType>;

   /// @brief Concept: a amount dimension (exactly one amount unit after simplification).
   template<typename T>
   concept is_amount =
      std::tuple_size_v<simplified_units_t<typename T::units>> == 1 &&
      is_amount_unit<typename std::tuple_element_t<0, simplified_units_t<typename T::units>>::unit>;

   /// @brief Get a amount object in unit T.
   template<is_amount_unit T, is_amount DimType>
   constexpr typename DimType::rep get_amount_as(DimType obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class amount;

   /// @brief amount dimension type.
   template<rep_type Rep, is_amount_unit Unit, is_coefficient... Cs>
   class amount<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {
      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructs to zero.
      constexpr amount() : impl(0.0) {}

      /// @brief Construct with a numeric value.
      explicit constexpr amount(double val) : impl(val) {}

      // perfect-forward ctor so factory can pass symbols
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr amount(V&& v, Ds... ds)
         : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr amount(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src)
         : impl(src) {}

      /// @brief Construct from another base_dimension of same amount.
      template<typename... Ts>
      requires is_amount<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between same-dimension types is intentional.
      // cppcheck-suppress noExplicitConstructor
      constexpr amount(const base_dimension_impl<Rep, Ts...>& base)
         : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_amount_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   [[maybe_unused]]
   constexpr auto make_amount(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return amount<Rep, U, Cs...>(value);
   }

   template<is_amount_unit U, is_coefficient... Cs>
   [[maybe_unused]]
   constexpr auto make_amount(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return amount<double, U, Cs...>(1.0);   // 1 × coeffs
   }

   template<is_amount_unit Unit, is_coefficient... Cs>
   class amount<Unit, Cs...> : public amount<double, Unit, Cs...>
   {
   public:
      using amount<double, Unit, Cs...>::amount;
   };

   /// @brief Deduction guides
   template<is_amount Dim>
   amount(Dim) -> amount<
      typename Dim::rep,
      simplified_unit_filter<amountType, typename Dim::units>>;

   template<rep_type Rep, is_amount_unit Unit, is_coefficient... Cs>
   amount(const amount<Rep, Unit, Cs...>&)
      -> amount<Rep, Unit, Cs...>;

   template<rep_type R, is_amount_unit U, is_coefficient... Cs>
   amount(base_dimension_impl<R, unit_exponent<U>, Cs...>)
      -> amount<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_AMOUNT_IMPL_H