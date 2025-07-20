#ifndef STATIC_DIMENSION_AMOUNT_IMPL_H
#define STATIC_DIMENSION_AMOUNT_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the amount type tag.
   struct amountType {};

   /// @brief Represents the primary unit for amount.
   struct moles;

   using primary_amount = moles;

   /// @brief Concept to check if a type is a valid amount unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_amount_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, amountType>;

   /// @brief Base class for amount units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct amountUnit : public BaseUnit<Unit, Name, Abbreviation, "amount", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = amountType;

      /// @brief The primary unit type for this dimension.
      using Primary = primary_amount;
   };

   /// @brief Concept to check if a type is a amount dimension.
   /// @tparam T The type to check.
   template<typename T>
   concept is_amount = requires {
      typename decltype(FullSimplify(T{}))::units;
   } &&
   std::tuple_size_v<typename decltype(FullSimplify(T{}))::units> == 1 &&
   is_amount_unit<typename std::tuple_element_t<0, typename decltype(FullSimplify(T{}))::units>::unit>;

   /// @brief Retrieves the value of a amount object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The amount object.
   /// @return The value in the specified unit.
   template<is_amount_unit T>
   constexpr PrecisionType get_amount_as(/*amount_type*/ auto obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class amount;

   /// @brief Represents a dimension type for amount.
   /// @tparam Unit The primary unit type.
   template<rep_type Rep, is_amount_unit Unit, is_coefficient... Cs>
   class amount<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {

      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructor initializing to zero.
      constexpr amount() : impl(0.0) {}

      /// @brief Constructs a amount object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr amount(double val) : impl(val) {}

      /* perfect-forward ctor so factory can pass symbols --------------------- */
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr amount(V&& v, Ds... ds) : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr amount(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src) : impl(src) {}

      /// @brief Constructs a amount object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<impl, base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr amount(const base_dimension_impl<Rep, Ts...>& base) : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_amount_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_amount(Rep value, Cs... coeffs)
   {
      return amount<Rep, U, Cs...>(value, coeffs...);
   }

   template<is_amount_unit U, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_amount(Cs... coeffs)
   {
      return amount<double, U, Cs...>(1.0, coeffs...);   // 1 × coeffs
   }

   template<is_amount_unit Unit, is_coefficient... Cs>
   class amount<Unit, Cs...> : public amount<double, Unit, Cs...> {
   public:
      using amount<double, Unit, Cs...>::amount;
   };

   /// @brief Deduction guide for amount constructor with base_dimension.
   /// @tparam amountUnit The unit type.
   template<is_amount Dim>
   amount(Dim) -> amount<typename Dim::rep, DimExtractor<amountType, Dim>>;

   template<rep_type Rep, is_amount_unit Unit, is_coefficient... Cs>
   amount(const amount<Rep, Unit, Cs...>&) -> amount<Rep, Unit, Cs...>;

   template<rep_type R, is_amount_unit U, is_coefficient... Cs>
   amount(base_dimension_impl<R, unit_exponent<U>, Cs...>) -> amount<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_AMOUNT_IMPL_H