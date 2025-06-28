#ifndef STATIC_DIMENSION_amount_IMPL_H
#define STATIC_DIMENSION_amount_IMPL_H

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the amount type tag.
   struct amountType {};

   /// @brief Represents the primary unit for amount.
   struct moles;

   using Primaryamount = moles;

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
      using Primary = Primaryamount;
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

   template<typename RepOrUnit = double, typename MaybeUnit = void>
   class amount;

   /// @brief Represents a dimension type for amount.
   /// @tparam Unit The primary unit type.
   template<rep_type Rep, is_amount_unit Unit>
   class amount<Rep, Unit> : public base_dimension_impl<Rep, unit_exponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr amount() : base_dimension_impl<Rep, unit_exponent<Unit>>::base_dimension_impl(0.0) {}

      /// @brief Constructs a amount object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr amount(double val) : base_dimension_impl<Rep, unit_exponent<Unit>>::base_dimension_impl(val) {}

      /// @brief Constructs a amount object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<base_dimension_impl<Rep, unit_exponent<Unit>>, base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr amount(const base_dimension_impl<Rep, Ts...>& base) : base_dimension_impl<Rep, unit_exponent<Unit>>::base_dimension_impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_amount_unit Unit>
   class amount<Unit, void> : public amount<double, Unit> {
   public:
      using amount<double, Unit>::amount;
   };

   /// @brief Deduction guide for amount constructor with base_dimension.
   /// @tparam amountUnit The unit type.
   template<is_amount Dim>
   amount(Dim) -> amount<typename Dim::rep, DimExtractor<amountType, Dim>>;
}

#endif // STATIC_DIMENSION_amount_IMPL_H