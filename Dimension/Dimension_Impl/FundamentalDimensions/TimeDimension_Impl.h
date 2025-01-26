#ifndef STATIC_DIMENSION_Time_IMPL_H
#define STATIC_DIMENSION_Time_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Time type tag.
   struct TimeType {};

   /// @brief Represents the primary unit for Time.
   struct Seconds;

   /// @brief Concept to check if a type is a valid Time unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsTimeUnit = IsNonQuantityUnitDimension<T, TimeType> || IsQuantityUnitDimension<T, TimeType>;

   /// @brief Base class for Time units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct TimeUnit : public BaseUnit<Unit, Name, Abbreviation, "Time", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = TimeType;

      /// @brief The primary unit type for this dimension.
      using Primary = Seconds;
   };

   /// @brief Trait to check if a type is a Time dimension.
   /// @tparam T The type to check.
   template<typename T>
   struct is_Time : std::false_type {};

   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Time<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsTimeUnit<T>> {};

   /// @brief Helper variable template for is_Time.
   /// @tparam T The type to check.
   template<typename T>
   constexpr bool is_Time_v = is_Time<T>::value;

   /// @brief Concept to verify if a type is a valid Time type.
   /// @tparam T The type to check.
   template<typename T>
   concept Time_type = is_Time_v<T>;

   /// @brief Retrieves the value of a Time object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The Time object.
   /// @return The value in the specified unit.
   template<IsTimeUnit T>
   constexpr PrecisionType getTime(Time_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   /// @brief Represents a dimension type for Time.
   /// @tparam Unit The primary unit type.
   template<IsTimeUnit Unit>
   class Time : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Time() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      /// @brief Constructs a Time object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Time(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      /// @brief Constructs a Time object from another BaseDimension.
      /// @tparam T The unit type of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<IsTimeUnit T>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Time(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()){}

      /// @brief Deprecated method to retrieve the value of the dimension.
      /// @tparam T The unit type.
      /// @return The value in the specified unit.
      template<IsTimeUnit T>
      [[deprecated("Use the free function getTime() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetTime() const
      {
         return getTime<T>(*this);
      }
   };

   /// @brief Deduction guide for Time constructor.
   /// @tparam T The unit type.
   template<IsTimeUnit T>
   Time(T) -> Time<T>;

   /// @brief Deduction guide for Time constructor with BaseDimension.
   /// @tparam TimeUnit The unit type.
   template<IsTimeUnit TimeUnit>
   Time(BaseDimension<std::tuple<TimeUnit>, std::tuple<>>) -> Time<TimeUnit>;

   /// @brief Specialization for is_Time trait for Time types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Time<Time<T>> : std::bool_constant<IsTimeUnit<T>> {};
}

#endif //STATIC_DIMENSION_Time_IMPL_H