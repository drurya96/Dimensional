#ifndef STATIC_DIMENSION_Angle_IMPL_H
#define STATIC_DIMENSION_Angle_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Angle type tag.
   struct AngleType {};

   /// @brief Represents the primary unit for Angle.
   struct Radians;

   using PrimaryAngle = Radians;

   /// @brief Concept to check if a type is a valid Angle unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsAngleUnit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, AngleType>;//IsNonQuantityUnitDimension<T, AngleType> || IsQuantityUnitDimension<T, AngleType>;

   /// @brief Base class for Angle units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct AngleUnit : public BaseUnit<Unit, Name, Abbreviation, "Angle", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = AngleType;

      /// @brief The primary unit type for this dimension.
      using Primary = PrimaryAngle;
   };

   /// @brief Trait to check if a type is a Angle dimension.
   /// @tparam T The type to check.
   //template<typename T>
   //struct is_Angle : std::false_type {};

   //template<typename Dim>
   //struct is_Angle_Impl
   //{
   //   constexpr bool value = IsAngleUnit<std::tuple_element_t<0, decltype(FullSimplify(Dim{}))>>;
   //};


   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   //template<typename T>
   //struct is_Angle<BaseDimension<T>> : std::bool_constant<IsAngleUnit<T>> {};
   template<typename Dim>
   struct is_Angle : std::bool_constant<IsAngleUnit<typename std::tuple_element_t<0, typename decltype(FullSimplify(Dim{}))::units>::unit>> {};

   /// @brief Helper variable template for is_Angle.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Angle_v = is_Angle<T>::value;

   /// @brief Concept to verify if a type is a valid Angle type.
   /// @tparam T The type to check.
   template<typename T>
   concept Angle_type = is_Angle_v<T>;

   /// @brief Retrieves the value of a Angle object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Angle object.
   /// @return The value in the specified unit.
   template<IsAngleUnit T>
   constexpr PrecisionType get_angle_as(/*Angle_type*/ auto obj)
   {
      return get_dimension_as<UnitExponent<T>>(obj);
   }

   template<IsAngleUnit Unit = PrimaryAngle, bool isQuantity = false>
   class Angle;

   /// @brief Represents a dimension type for Angle.
   /// @tparam Unit The primary unit type.
   template<IsAngleUnit Unit>
   class Angle<Unit, false> : public BaseDimension<UnitExponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Angle() : BaseDimension<UnitExponent<Unit>>::BaseDimension(0.0) {}

      /// @brief Constructs a Angle object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Angle(double val) : BaseDimension<UnitExponent<Unit>>::BaseDimension(val) {}

      /// @brief Constructs a Angle object from another BaseDimension.
      /// @tparam Ts The units of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<BaseDimension<UnitExponent<Unit>>, BaseDimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Angle(const BaseDimension<Ts...>& base) : BaseDimension<UnitExponent<Unit>>::BaseDimension(get_dimension_as<UnitExponent<Unit>>(base)) {}
   };

   /// @brief Specialization for is_Angle trait for Angle types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Angle<Angle<T>> : std::bool_constant<IsAngleUnit<T>> {};

   /// @brief Deduction guide for Angle constructor.
   /// @tparam T The unit type.
   //template<IsAngleUnit T>
   //Angle(T) -> Angle<T>;

   /// @brief Deduction guide for Angle constructor with BaseDimension.
   /// @tparam AngleUnit The unit type.
   //template<IsAngleUnit AngleUnit>
   //Angle(BaseDimension<UnitExponent<AngleUnit>>) -> Angle<AngleUnit>;

   /// @brief Deduction guide for Angle constructor with BaseDimension.
   /// @tparam AngleUnit The unit type.
   template<Angle_type Dim>
   Angle(Dim) -> Angle<DimExtractor<AngleType, Dim>>;
}

#endif // STATIC_DIMENSION_Angle_IMPL_H