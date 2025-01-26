#ifndef STATIC_DIMENSION_Temperature_IMPL_H
#define STATIC_DIMENSION_Temperature_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Temperature type tag.
   struct TemperatureType {};

   /// @brief Represents the primary unit for Temperature.
   struct Kelvin;

   /// @brief Concept to check if a type is a valid Temperature unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsTemperatureUnit = IsNonQuantityUnitDimension<T, TemperatureType> || IsQuantityUnitDimension<T, TemperatureType>;

   /// @brief Base class for Temperature units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct TemperatureUnit : public BaseUnit<Unit, Name, Abbreviation, "Temperature", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = TemperatureType;

      /// @brief The primary unit type for this dimension.
      using Primary = Kelvin;
   };

   /// @brief Trait to check if a type is a Temperature dimension.
   /// @tparam T The type to check.
   template<typename T>
   struct is_Temperature : std::false_type {};

   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Temperature<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsTemperatureUnit<T>> {};

   /// @brief Helper variable template for is_Temperature.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Temperature_v = is_Temperature<T>::value;

   /// @brief Concept to verify if a type is a valid Temperature type.
   /// @tparam T The type to check.
   template<typename T>
   concept Temperature_type = is_Temperature_v<T>;

   /// @brief Retrieves the value of a Temperature object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Temperature object.
   /// @return The value in the specified unit.
   template<IsTemperatureUnit T>
   constexpr PrecisionType getTemperature(Temperature_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   /// @brief Represents a dimension type for Temperature.
   /// @tparam Unit The primary unit type.
   template<IsTemperatureUnit Unit>
   class Temperature : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Temperature() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      /// @brief Constructs a Temperature object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Temperature(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      /// @brief Constructs a Temperature object from another BaseDimension.
      /// @tparam T The unit type of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<IsTemperatureUnit T>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Temperature(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}

      /// @brief Deprecated method to retrieve the value of the dimension.
      /// @tparam T The unit type.
      /// @return The value in the specified unit.
      template<IsTemperatureUnit T>
      [[deprecated("Use the free function getTemperature() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetTemperature() const
      {
         return getTemperature<T>(*this);
      }
   };

   /// @brief Deduction guide for Temperature constructor.
   /// @tparam T The unit type.
   template<IsTemperatureUnit T>
   Temperature(T) -> Temperature<T>;

   /// @brief Deduction guide for Temperature constructor with BaseDimension.
   /// @tparam TemperatureUnit The unit type.
   template<IsTemperatureUnit TemperatureUnit>
   Temperature(BaseDimension<std::tuple<TemperatureUnit>, std::tuple<>>) -> Temperature<TemperatureUnit>;

   /// @brief Specialization for is_Temperature trait for Temperature types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Temperature<Temperature<T>> : std::bool_constant<IsTemperatureUnit<T>> {};
}

#endif //STATIC_DIMENSION_Temperature_IMPL_H