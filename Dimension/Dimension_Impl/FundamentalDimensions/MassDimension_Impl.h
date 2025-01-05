#ifndef STATIC_DIMENSION_Mass_IMPL_H
#define STATIC_DIMENSION_Mass_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Mass type tag.
   struct MassType {};

   /// @brief Represents the primary unit for Mass.
   struct Grams;

   /// @brief Concept to check if a type is a valid Mass unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsMassUnit = IsNonQuantityUnitDimension<T, MassType> || IsQuantityUnitDimension<T, MassType>;

   /// @brief Base class for Mass units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct MassUnit : public BaseUnit<Unit, Name, Abbreviation, "Mass">
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = MassType;

      /// @brief The primary unit type for this dimension.
      using Primary = Grams;
   };

   /// @brief Trait to check if a type is a Mass dimension.
   /// @tparam T The type to check.
   template<typename T>
   struct is_Mass : std::false_type {};

   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Mass<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsMassUnit<T>> {};

   /// @brief Helper variable template for is_Mass.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Mass_v = is_Mass<T>::value;

   /// @brief Concept to verify if a type is a valid Mass type.
   /// @tparam T The type to check.
   template<typename T>
   concept Mass_type = is_Mass_v<T>;

   /// @brief Retrieves the value of a Mass object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Mass object.
   /// @return The value in the specified unit.
   template<IsMassUnit T>
   constexpr PrecisionType getMass(Mass_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   /// @brief Represents a dimension type for Mass.
   /// @tparam Unit The primary unit type.
   template<IsMassUnit Unit>
   class Mass : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Mass() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      /// @brief Constructs a Mass object with a specific value.
      /// @param val The value to initialize with.
      constexpr Mass(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      /// @brief Constructs a Mass object from another BaseDimension.
      /// @tparam T The unit type of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<IsMassUnit T>
      constexpr Mass(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}

      /// @brief Deprecated method to retrieve the value of the dimension.
      /// @tparam T The unit type.
      /// @return The value in the specified unit.
      template<IsMassUnit T>
      [[deprecated("Use the free function getMass() instead.")]]
      double GetMass() const
      {
         return getMass<T>(*this);
      }
   };

   /// @brief Deduction guide for Mass constructor.
   /// @tparam T The unit type.
   template<IsMassUnit T>
   Mass(T) -> Mass<T>;

   /// @brief Deduction guide for Mass constructor with BaseDimension.
   /// @tparam MassUnit The unit type.
   template<IsMassUnit MassUnit>
   Mass(BaseDimension<std::tuple<MassUnit>, std::tuple<>>) -> Mass<MassUnit>;

   /// @brief Specialization for is_Mass trait for Mass types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Mass<Mass<T>> : std::bool_constant<IsMassUnit<T>> {};
}

#endif //STATIC_DIMENSION_Mass_IMPL_H