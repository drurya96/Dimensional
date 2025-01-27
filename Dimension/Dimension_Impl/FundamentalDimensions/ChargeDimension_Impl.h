#ifndef STATIC_DIMENSION_Charge_IMPL_H
#define STATIC_DIMENSION_Charge_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Charge type tag.
   struct ChargeType {};

   /// @brief Represents the primary unit for Charge.
   struct Coulombs;

   /// @brief Concept to check if a type is a valid Charge unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsChargeUnit = IsNonQuantityUnitDimension<T, ChargeType> || IsQuantityUnitDimension<T, ChargeType>;

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
      using Primary = Coulombs;
   };

   /// @brief Trait to check if a type is a Charge dimension.
   /// @tparam T The type to check.
   template<typename T>
   struct is_Charge : std::false_type {};

   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Charge<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsChargeUnit<T>> {};

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
   constexpr PrecisionType getCharge(Charge_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   /// @brief Represents a dimension type for Charge.
   /// @tparam Unit The primary unit type.
   template<IsChargeUnit Unit>
   class Charge : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Charge() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      /// @brief Constructs a Charge object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Charge(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      /// @brief Constructs a Charge object from another BaseDimension.
      /// @tparam T The unit type of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<IsChargeUnit T>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Charge(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}

      /// @brief Deprecated method to retrieve the value of the dimension.
      /// @tparam T The unit type.
      /// @return The value in the specified unit.
      template<IsChargeUnit T>
      [[deprecated("Use the free function getCharge() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetCharge() const
      {
         return getCharge<T>(*this);
      }
   };

   /// @brief Deduction guide for Charge constructor.
   /// @tparam T The unit type.
   template<IsChargeUnit T>
   Charge(T) -> Charge<T>;

   /// @brief Deduction guide for Charge constructor with BaseDimension.
   /// @tparam ChargeUnit The unit type.
   template<IsChargeUnit ChargeUnit>
   Charge(BaseDimension<std::tuple<ChargeUnit>, std::tuple<>>) -> Charge<ChargeUnit>;

   /// @brief Specialization for is_Charge trait for Charge types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Charge<Charge<T>> : std::bool_constant<IsChargeUnit<T>> {};
}

#endif //STATIC_DIMENSION_Charge_IMPL_H