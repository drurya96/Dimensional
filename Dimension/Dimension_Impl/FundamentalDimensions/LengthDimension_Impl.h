#ifndef STATIC_DIMENSION_Length_IMPL_H
#define STATIC_DIMENSION_Length_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Length type tag.
   struct LengthType {};

   /// @brief Represents the primary unit for Length.
   struct Meters;

   /// @brief Concept to check if a type is a valid Length unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsLengthUnit = IsNonQuantityUnitDimension<T, LengthType> || IsQuantityUnitDimension<T, LengthType>;

   /// @brief Base class for Length units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct LengthUnit : public BaseUnit<Unit, Name, Abbreviation, "Length">
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = LengthType;

      /// @brief The primary unit type for this dimension.
      using Primary = Meters;
   };

   /// @brief Trait to check if a type is a Length dimension.
   /// @tparam T The type to check.
   template<typename T>
   struct is_Length : std::false_type {};

   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Length<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsLengthUnit<T>> {};

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
   constexpr PrecisionType getLength(Length_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   /// @brief Represents a dimension type for Length.
   /// @tparam Unit The primary unit type.
   template<IsLengthUnit Unit>
   class Length : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Length() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      /// @brief Constructs a Length object with a specific value.
      /// @param val The value to initialize with.
      constexpr Length(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      /// @brief Constructs a Length object from another BaseDimension.
      /// @tparam T The unit type of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<IsLengthUnit T>
      constexpr Length(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}

      /// @brief Deprecated method to retrieve the value of the dimension.
      /// @tparam T The unit type.
      /// @return The value in the specified unit.
      template<IsLengthUnit T>
      [[deprecated("Use the free function getLength() instead.")]]
      double GetLength() const
      {
         return getLength<T>(*this);
      }
   };

   /// @brief Deduction guide for Length constructor.
   /// @tparam T The unit type.
   template<IsLengthUnit T>
   Length(T) -> Length<T>;

   /// @brief Deduction guide for Length constructor with BaseDimension.
   /// @tparam LengthUnit The unit type.
   template<IsLengthUnit LengthUnit>
   Length(BaseDimension<std::tuple<LengthUnit>, std::tuple<>>) -> Length<LengthUnit>;

   /// @brief Specialization for is_Length trait for Length types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Length<Length<T>> : std::bool_constant<IsLengthUnit<T>> {};
}

#endif //STATIC_DIMENSION_Length_IMPL_H