#ifndef STATIC_DIMENSION_charge_IMPL_H
#define STATIC_DIMENSION_charge_IMPL_H

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the charge type tag.
   struct chargeType {};

   /// @brief Represents the primary unit for charge.
   struct coulombs;

   using Primarycharge = coulombs;

   /// @brief Concept to check if a type is a valid charge unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_charge_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, chargeType>;

   /// @brief Base class for charge units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct chargeUnit : public BaseUnit<Unit, Name, Abbreviation, "charge", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = chargeType;

      /// @brief The primary unit type for this dimension.
      using Primary = Primarycharge;
   };

   /// @brief Concept to check if a type is a charge dimension.
   /// @tparam T The type to check.
   template<typename T>
   concept is_charge = requires {
      typename decltype(FullSimplify(T{}))::units;
   } &&
   std::tuple_size_v<typename decltype(FullSimplify(T{}))::units> == 1 &&
   is_charge_unit<typename std::tuple_element_t<0, typename decltype(FullSimplify(T{}))::units>::unit>;

   /// @brief Retrieves the value of a charge object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The charge object.
   /// @return The value in the specified unit.
   template<is_charge_unit T>
   constexpr PrecisionType get_charge_as(/*charge_type*/ auto obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename RepOrUnit = double, typename MaybeUnit = void, is_coefficient... Cs>
   class charge;

   /// @brief Represents a dimension type for charge.
   /// @tparam Unit The primary unit type.
   template<rep_type Rep, is_charge_unit Unit, is_coefficient... Cs>
   class charge<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {

      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructor initializing to zero.
      constexpr charge() : impl(0.0) {}

      /// @brief Constructs a charge object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr charge(double val) : impl(val) {}

      /* perfect-forward ctor so factory can pass symbols --------------------- */
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr charge(V&& v, Ds... ds) : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      constexpr charge(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src) : impl(src) {}

      /// @brief Constructs a charge object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<impl, base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr charge(const base_dimension_impl<Rep, Ts...>& base) : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_charge_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   constexpr auto make_charge(Rep value, Cs... coeffs)
   {
      return charge<Rep, U, Cs...>(value, coeffs...);
   }

   template<is_charge_unit U, is_coefficient... Cs>
   constexpr auto make_charge(Cs... coeffs)
   {
      return charge<double, U, Cs...>(1.0, coeffs...);   // 1 × coeffs
   }

   template<is_charge_unit Unit, is_coefficient... Cs>
   class charge<Unit, void, Cs...> : public charge<double, Unit, Cs...> {
   public:
      using charge<double, Unit, Cs...>::charge;
   };

   /// @brief Deduction guide for charge constructor with base_dimension.
   /// @tparam chargeUnit The unit type.
   template<is_charge Dim>
   charge(Dim) -> charge<typename Dim::rep, DimExtractor<chargeType, Dim>>;

   template<rep_type Rep, is_charge_unit Unit, is_coefficient... Cs>
   charge(const charge<Rep, Unit, Cs...>&) -> charge<Rep, Unit, Cs...>;

   template<rep_type R, is_charge_unit U, is_coefficient... Cs>
   charge(base_dimension_impl<R, unit_exponent<U>, Cs...>) -> charge<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_charge_IMPL_H