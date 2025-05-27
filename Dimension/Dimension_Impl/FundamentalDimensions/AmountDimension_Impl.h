#ifndef STATIC_DIMENSION_Amount_IMPL_H
#define STATIC_DIMENSION_Amount_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Amount type tag.
   struct AmountType {};

   /// @brief Represents the primary unit for Amount.
   struct Moles;

   using PrimaryAmount = Moles;

   /// @brief Concept to check if a type is a valid Amount unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsAmountUnit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, AmountType>;//IsNonQuantityUnitDimension<T, AmountType> || IsQuantityUnitDimension<T, AmountType>;

   /// @brief Base class for Amount units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct AmountUnit : public BaseUnit<Unit, Name, Abbreviation, "Amount", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = AmountType;

      /// @brief The primary unit type for this dimension.
      using Primary = PrimaryAmount;
   };

   /// @brief Trait to check if a type is a Amount dimension.
   /// @tparam T The type to check.
   //template<typename T>
   //struct is_Amount : std::false_type {};

   //template<typename Dim>
   //struct is_Amount_Impl
   //{
   //   constexpr bool value = IsAmountUnit<std::tuple_element_t<0, decltype(FullSimplify(Dim{}))>>;
   //};


   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   //template<typename T>
   //struct is_Amount<BaseDimension<T>> : std::bool_constant<IsAmountUnit<T>> {};
   template<typename Dim>
   struct is_Amount : std::bool_constant<IsAmountUnit<typename std::tuple_element_t<0, typename decltype(FullSimplify(Dim{}))::units>::unit>> {};

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
   constexpr PrecisionType get_amount_as(/*Amount_type*/ auto obj)
   {
      return get_dimension_as<UnitExponent<T>>(obj);
   }

   template<IsAmountUnit Unit = PrimaryAmount, bool isQuantity = false>
   class Amount;

   /// @brief Represents a dimension type for Amount.
   /// @tparam Unit The primary unit type.
   template<IsAmountUnit Unit>
   class Amount<Unit, false> : public BaseDimension<UnitExponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Amount() : BaseDimension<UnitExponent<Unit>>::BaseDimension(0.0) {}

      /// @brief Constructs a Amount object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Amount(double val) : BaseDimension<UnitExponent<Unit>>::BaseDimension(val) {}

      /// @brief Constructs a Amount object from another BaseDimension.
      /// @tparam Ts The units of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<BaseDimension<UnitExponent<Unit>>, BaseDimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Amount(const BaseDimension<Ts...>& base) : BaseDimension<UnitExponent<Unit>>::BaseDimension(get_dimension_as<UnitExponent<Unit>>(base)) {}
   };

   /// @brief Specialization for is_Amount trait for Amount types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Amount<Amount<T>> : std::bool_constant<IsAmountUnit<T>> {};

   /// @brief Deduction guide for Amount constructor.
   /// @tparam T The unit type.
   //template<IsAmountUnit T>
   //Amount(T) -> Amount<T>;

   /// @brief Deduction guide for Amount constructor with BaseDimension.
   /// @tparam AmountUnit The unit type.
   //template<IsAmountUnit AmountUnit>
   //Amount(BaseDimension<UnitExponent<AmountUnit>>) -> Amount<AmountUnit>;

   /// @brief Deduction guide for Amount constructor with BaseDimension.
   /// @tparam AmountUnit The unit type.
   template<Amount_type Dim>
   Amount(Dim) -> Amount<DimExtractor<AmountType, Dim>>;
}

#endif // STATIC_DIMENSION_Amount_IMPL_H