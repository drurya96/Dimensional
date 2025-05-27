#ifndef STATIC_DIMENSION_Time_IMPL_H
#define STATIC_DIMENSION_Time_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   /// @brief Represents the Time type tag.
   struct TimeType {};

   /// @brief Represents the primary unit for Time.
   struct Seconds;

   using PrimaryTime = Seconds;

   /// @brief Concept to check if a type is a valid Time unit.
   /// @tparam T The type to check.
   template<typename T>
   concept IsTimeUnit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, TimeType>;//IsNonQuantityUnitDimension<T, TimeType> || IsQuantityUnitDimension<T, TimeType>;

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
      using Primary = PrimaryTime;
   };

   /// @brief Trait to check if a type is a Time dimension.
   /// @tparam T The type to check.
   //template<typename T>
   //struct is_Time : std::false_type {};

   //template<typename Dim>
   //struct is_Time_Impl
   //{
   //   constexpr bool value = IsTimeUnit<std::tuple_element_t<0, decltype(FullSimplify(Dim{}))>>;
   //};


   /// @brief Specialization for BaseDimension types.
   /// @tparam T The unit type.
   //template<typename T>
   //struct is_Time<BaseDimension<T>> : std::bool_constant<IsTimeUnit<T>> {};
   template<typename Dim>
   struct is_Time : std::bool_constant<IsTimeUnit<typename std::tuple_element_t<0, typename decltype(FullSimplify(Dim{}))::units>::unit>> {};

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
   constexpr PrecisionType get_time_as(/*Time_type*/ auto obj)
   {
      return get_dimension_as<UnitExponent<T>>(obj);
   }

   template<IsTimeUnit Unit = PrimaryTime, bool isQuantity = false>
   class Time;

   /// @brief Represents a dimension type for Time.
   /// @tparam Unit The primary unit type.
   template<IsTimeUnit Unit>
   class Time<Unit, false> : public BaseDimension<UnitExponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr Time() : BaseDimension<UnitExponent<Unit>>::BaseDimension(0.0) {}

      /// @brief Constructs a Time object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr Time(double val) : BaseDimension<UnitExponent<Unit>>::BaseDimension(val) {}

      /// @brief Constructs a Time object from another BaseDimension.
      /// @tparam Ts The units of the BaseDimension.
      /// @param base The BaseDimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<BaseDimension<UnitExponent<Unit>>, BaseDimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Time(const BaseDimension<Ts...>& base) : BaseDimension<UnitExponent<Unit>>::BaseDimension(get_dimension_as<UnitExponent<Unit>>(base)) {}
   };

   /// @brief Specialization for is_Time trait for Time types.
   /// @tparam T The unit type.
   template<typename T>
   struct is_Time<Time<T>> : std::bool_constant<IsTimeUnit<T>> {};

   /// @brief Deduction guide for Time constructor.
   /// @tparam T The unit type.
   //template<IsTimeUnit T>
   //Time(T) -> Time<T>;

   /// @brief Deduction guide for Time constructor with BaseDimension.
   /// @tparam TimeUnit The unit type.
   //template<IsTimeUnit TimeUnit>
   //Time(BaseDimension<UnitExponent<TimeUnit>>) -> Time<TimeUnit>;

   /// @brief Deduction guide for Time constructor with BaseDimension.
   /// @tparam TimeUnit The unit type.
   template<Time_type Dim>
   Time(Dim) -> Time<DimExtractor<TimeType, Dim>>;
}

#endif // STATIC_DIMENSION_Time_IMPL_H