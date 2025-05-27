#ifndef STATIC_DIMENSION_Length_IMPL_H
#define STATIC_DIMENSION_Length_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Length type tag.
   struct LengthType {};

   /// @brief Represents the primary unit for Length.
   struct Meters;

   using PrimaryLength = Meters;

   /// @brief Concept to check if a type is a valid Length unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsLengthUnit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, LengthType>;//IsNonQuantityUnitDimension<T, LengthType> || IsQuantityUnitDimension<T, LengthType>;

   /// @brief Base class for Length units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct LengthUnit : public BaseUnit<Unit, Name, Abbreviation, "Length", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = LengthType;

      /// @brief The primary unit type for this dimension.
      using Primary = PrimaryLength;
   };

   /// @brief Trait to check if a type is a Length dimension.
   /// @tparam T The type to check.
   //template<typename T>
   //struct is_Length : std::false_type {};

   //template<typename Dim>
   //struct is_Length_Impl
   //{
   //   constexpr bool value = IsLengthUnit<std::tuple_element_t<0, decltype(FullSimplify(Dim{}))>>;
   //};


   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   //template<typename T>
   //struct is_Length<BaseDimension<T>> : std::bool_constant<IsLengthUnit<T>> {};
   template<typename Dim>
   struct is_Length : std::bool_constant<IsLengthUnit<typename std::tuple_element_t<0, typename decltype(FullSimplify(Dim{}))::units>::unit>> {};

   /// @brief Helper variable template for is_Length.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Length_v = is_Length<T>::value;

   /// @brief Concept to verify if a type is a valid Length type.
   /// @tparam T The type to check.
   template<typename T>
   concept Length_type = is_Length_v<T>;

   /// @brief Retrieves the value of a Length object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Length object.
   /// @return The value in the specified unit.
   template<IsLengthUnit T>
   constexpr PrecisionType get_length_as(/*Length_type*/ auto obj)
   {
      return get_dimension_as<UnitExponent<T>>(obj);
   }

   template<IsLengthUnit Unit = PrimaryLength, bool isQuantity = false>
   class Length;

   /// @brief Represents a dimension type for Length.
   /// @tparam Unit The primary unit type.
   template<IsLengthUnit Unit>
   class Length<Unit, false> : public BaseDimension<UnitExponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Length() : BaseDimension<UnitExponent<Unit>>::BaseDimension(0.0) {}

      /// @brief Constructs a Length object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Length(double val) : BaseDimension<UnitExponent<Unit>>::BaseDimension(val) {}

      /// @brief Constructs a Length object from another BaseDimension.
      /// @tparam Ts The units of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<BaseDimension<UnitExponent<Unit>>, BaseDimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Length(const BaseDimension<Ts...>& base) : BaseDimension<UnitExponent<Unit>>::BaseDimension(get_dimension_as<UnitExponent<Unit>>(base)) {}
   };

   /// @brief Specialization for is_Length trait for Length types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Length<Length<T>> : std::bool_constant<IsLengthUnit<T>> {};

   /// @brief Deduction guide for Length constructor.
   /// @tparam T The unit type.
   //template<IsLengthUnit T>
   //Length(T) -> Length<T>;

   /// @brief Deduction guide for Length constructor with BaseDimension.
   /// @tparam LengthUnit The unit type.
   //template<IsLengthUnit LengthUnit>
   //Length(BaseDimension<UnitExponent<LengthUnit>>) -> Length<LengthUnit>;

   /// @brief Deduction guide for Length constructor with BaseDimension.
   /// @tparam LengthUnit The unit type.
   template<Length_type Dim>
   Length(Dim) -> Length<DimExtractor<LengthType, Dim>>;
}

#endif // STATIC_DIMENSION_Length_IMPL_H