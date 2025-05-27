#ifndef STATIC_DIMENSION_Charge_IMPL_H
#define STATIC_DIMENSION_Charge_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Charge type tag.
   struct ChargeType {};

   /// @brief Represents the primary unit for Charge.
   struct Coulombs;

   using PrimaryCharge = Coulombs;

   /// @brief Concept to check if a type is a valid Charge unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsChargeUnit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, ChargeType>;//IsNonQuantityUnitDimension<T, ChargeType> || IsQuantityUnitDimension<T, ChargeType>;

   /// @brief Base class for Charge units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct ChargeUnit : public BaseUnit<Unit, Name, Abbreviation, "Charge", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = ChargeType;

      /// @brief The primary unit type for this dimension.
      using Primary = PrimaryCharge;
   };

   /// @brief Trait to check if a type is a Charge dimension.
   /// @tparam T The type to check.
   //template<typename T>
   //struct is_Charge : std::false_type {};

   //template<typename Dim>
   //struct is_Charge_Impl
   //{
   //   constexpr bool value = IsChargeUnit<std::tuple_element_t<0, decltype(FullSimplify(Dim{}))>>;
   //};


   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   //template<typename T>
   //struct is_Charge<BaseDimension<T>> : std::bool_constant<IsChargeUnit<T>> {};
   template<typename Dim>
   struct is_Charge : std::bool_constant<IsChargeUnit<typename std::tuple_element_t<0, typename decltype(FullSimplify(Dim{}))::units>::unit>> {};

   /// @brief Helper variable template for is_Charge.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Charge_v = is_Charge<T>::value;

   /// @brief Concept to verify if a type is a valid Charge type.
   /// @tparam T The type to check.
   template<typename T>
   concept Charge_type = is_Charge_v<T>;

   /// @brief Retrieves the value of a Charge object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Charge object.
   /// @return The value in the specified unit.
   template<IsChargeUnit T>
   constexpr PrecisionType get_charge_as(/*Charge_type*/ auto obj)
   {
      return get_dimension_as<UnitExponent<T>>(obj);
   }

   template<IsChargeUnit Unit = PrimaryCharge, bool isQuantity = false>
   class Charge;

   /// @brief Represents a dimension type for Charge.
   /// @tparam Unit The primary unit type.
   template<IsChargeUnit Unit>
   class Charge<Unit, false> : public BaseDimension<UnitExponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Charge() : BaseDimension<UnitExponent<Unit>>::BaseDimension(0.0) {}

      /// @brief Constructs a Charge object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Charge(double val) : BaseDimension<UnitExponent<Unit>>::BaseDimension(val) {}

      /// @brief Constructs a Charge object from another BaseDimension.
      /// @tparam Ts The units of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<BaseDimension<UnitExponent<Unit>>, BaseDimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Charge(const BaseDimension<Ts...>& base) : BaseDimension<UnitExponent<Unit>>::BaseDimension(get_dimension_as<UnitExponent<Unit>>(base)) {}
   };

   /// @brief Specialization for is_Charge trait for Charge types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Charge<Charge<T>> : std::bool_constant<IsChargeUnit<T>> {};

   /// @brief Deduction guide for Charge constructor.
   /// @tparam T The unit type.
   //template<IsChargeUnit T>
   //Charge(T) -> Charge<T>;

   /// @brief Deduction guide for Charge constructor with BaseDimension.
   /// @tparam ChargeUnit The unit type.
   //template<IsChargeUnit ChargeUnit>
   //Charge(BaseDimension<UnitExponent<ChargeUnit>>) -> Charge<ChargeUnit>;

   /// @brief Deduction guide for Charge constructor with BaseDimension.
   /// @tparam ChargeUnit The unit type.
   template<Charge_type Dim>
   Charge(Dim) -> Charge<DimExtractor<ChargeType, Dim>>;
}

#endif // STATIC_DIMENSION_Charge_IMPL_H