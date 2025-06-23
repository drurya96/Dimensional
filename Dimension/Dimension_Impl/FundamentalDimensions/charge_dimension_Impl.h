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

   template<is_charge_unit Unit = Primarycharge, bool isQuantity = false>
   class charge;

   /// @brief Represents a dimension type for charge.
   /// @tparam Unit The primary unit type.
   template<is_charge_unit Unit>
   class charge<Unit, false> : public base_dimension<unit_exponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr charge() : base_dimension<unit_exponent<Unit>>::base_dimension(0.0) {}

      /// @brief Constructs a charge object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr charge(double val) : base_dimension<unit_exponent<Unit>>::base_dimension(val) {}

      /// @brief Constructs a charge object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<base_dimension<unit_exponent<Unit>>, base_dimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr charge(const base_dimension<Ts...>& base) : base_dimension<unit_exponent<Unit>>::base_dimension(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   /// @brief Deduction guide for charge constructor with base_dimension.
   /// @tparam chargeUnit The unit type.
   template<is_charge Dim>
   charge(Dim) -> charge<DimExtractor<chargeType, Dim>>;
}

#endif // STATIC_DIMENSION_charge_IMPL_H