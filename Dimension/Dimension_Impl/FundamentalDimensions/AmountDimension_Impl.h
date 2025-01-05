#ifndef STATIC_DIMENSION_Amount_IMPL_H
#define STATIC_DIMENSION_Amount_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Amount type tag.
   struct AmountType {};

   /// @brief Represents the primary unit for Amount.
   struct Moles;

   /// @brief Concept to check if a type is a valid Amount unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsAmountUnit = IsNonQuantityUnitDimension<T, AmountType> || IsQuantityUnitDimension<T, AmountType>;

   /// @brief Base class for Amount units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct AmountUnit : public BaseUnit<Unit, Name, Abbreviation, "Amount">
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = AmountType;

      /// @brief The primary unit type for this dimension.
      using Primary = Moles;
   };

   /// @brief Trait to check if a type is a Amount dimension.
   /// @tparam T The type to check.
   template<typename T>
   struct is_Amount : std::false_type {};

   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Amount<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsAmountUnit<T>> {};

   /// @brief Helper variable template for is_Amount.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Amount_v = is_Amount<T>::value;

   /// @brief Concept to verify if a type is a valid Amount type.
   /// @tparam T The type to check.
   template<typename T>
   concept Amount_type = is_Amount_v<T>;

   /// @brief Retrieves the value of a Amount object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Amount object.
   /// @return The value in the specified unit.
   template<IsAmountUnit T>
   constexpr PrecisionType getAmount(Amount_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   /// @brief Represents a dimension type for Amount.
   /// @tparam Unit The primary unit type.
   template<IsAmountUnit Unit>
   class Amount : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Amount() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      /// @brief Constructs a Amount object with a specific value.
      /// @param val The value to initialize with.
      constexpr Amount(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      /// @brief Constructs a Amount object from another BaseDimension.
      /// @tparam T The unit type of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<IsAmountUnit T>
      constexpr Amount(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}

      /// @brief Deprecated method to retrieve the value of the dimension.
      /// @tparam T The unit type.
      /// @return The value in the specified unit.
      template<IsAmountUnit T>
      [[deprecated("Use the free function getAmount() instead.")]]
      double GetAmount() const
      {
         return getAmount<T>(*this);
      }
   };

   /// @brief Deduction guide for Amount constructor.
   /// @tparam T The unit type.
   template<IsAmountUnit T>
   Amount(T) -> Amount<T>;

   /// @brief Deduction guide for Amount constructor with BaseDimension.
   /// @tparam AmountUnit The unit type.
   template<IsAmountUnit AmountUnit>
   Amount(BaseDimension<std::tuple<AmountUnit>, std::tuple<>>) -> Amount<AmountUnit>;

   /// @brief Specialization for is_Amount trait for Amount types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Amount<Amount<T>> : std::bool_constant<IsAmountUnit<T>> {};
}

#endif //STATIC_DIMENSION_Amount_IMPL_H