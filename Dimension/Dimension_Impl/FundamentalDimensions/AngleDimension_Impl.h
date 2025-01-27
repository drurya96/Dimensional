#ifndef STATIC_DIMENSION_Angle_IMPL_H
#define STATIC_DIMENSION_Angle_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Angle type tag.
   struct AngleType {};

   /// @brief Represents the primary unit for Angle.
   struct Radians;

   /// @brief Concept to check if a type is a valid Angle unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsAngleUnit = IsNonQuantityUnitDimension<T, AngleType> || IsQuantityUnitDimension<T, AngleType>;

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
      using Primary = Radians;
   };

   /// @brief Trait to check if a type is a Angle dimension.
   /// @tparam T The type to check.
   template<typename T>
   struct is_Angle : std::false_type {};

   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Angle<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsAngleUnit<T>> {};

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
   constexpr PrecisionType getAngle(Angle_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   /// @brief Represents a dimension type for Angle.
   /// @tparam Unit The primary unit type.
   template<IsAngleUnit Unit>
   class Angle : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Angle() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      /// @brief Constructs a Angle object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Angle(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      /// @brief Constructs a Angle object from another BaseDimension.
      /// @tparam T The unit type of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<IsAngleUnit T>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Angle(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}

      /// @brief Deprecated method to retrieve the value of the dimension.
      /// @tparam T The unit type.
      /// @return The value in the specified unit.
      template<IsAngleUnit T>
      [[deprecated("Use the free function getAngle() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetAngle() const
      {
         return getAngle<T>(*this);
      }
   };

   /// @brief Deduction guide for Angle constructor.
   /// @tparam T The unit type.
   template<IsAngleUnit T>
   Angle(T) -> Angle<T>;

   /// @brief Deduction guide for Angle constructor with BaseDimension.
   /// @tparam AngleUnit The unit type.
   template<IsAngleUnit AngleUnit>
   Angle(BaseDimension<std::tuple<AngleUnit>, std::tuple<>>) -> Angle<AngleUnit>;

   /// @brief Specialization for is_Angle trait for Angle types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Angle<Angle<T>> : std::bool_constant<IsAngleUnit<T>> {};
}

#endif //STATIC_DIMENSION_Angle_IMPL_H