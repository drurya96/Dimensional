#ifndef STATIC_DIMENSION_timespan_IMPL_H
#define STATIC_DIMENSION_timespan_IMPL_H

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the timespan type tag.
   struct timespanType {};

   /// @brief Represents the primary unit for timespan.
   struct seconds;

   using Primarytimespan = seconds;

   /// @brief Concept to check if a type is a valid timespan unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_timespan_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, timespanType>;

   /// @brief Base class for timespan units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct timespanUnit : public BaseUnit<Unit, Name, Abbreviation, "timespan", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = timespanType;

      /// @brief The primary unit type for this dimension.
      using Primary = Primarytimespan;
   };

   /// @brief Concept to check if a type is a timespan dimension.
   /// @tparam T The type to check.
   template<typename T>
   concept is_timespan = requires {
      typename decltype(FullSimplify(T{}))::units;
   } &&
   std::tuple_size_v<typename decltype(FullSimplify(T{}))::units> == 1 &&
   is_timespan_unit<typename std::tuple_element_t<0, typename decltype(FullSimplify(T{}))::units>::unit>;

   /// @brief Retrieves the value of a timespan object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The timespan object.
   /// @return The value in the specified unit.
   template<is_timespan_unit T>
   constexpr PrecisionType get_timespan_as(/*timespan_type*/ auto obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<is_timespan_unit Unit = Primarytimespan, bool isQuantity = false>
   class timespan;

   /// @brief Represents a dimension type for timespan.
   /// @tparam Unit The primary unit type.
   template<is_timespan_unit Unit>
   class timespan<Unit, false> : public base_dimension<unit_exponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr timespan() : base_dimension<unit_exponent<Unit>>::base_dimension(0.0) {}

      /// @brief Constructs a timespan object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr timespan(double val) : base_dimension<unit_exponent<Unit>>::base_dimension(val) {}

      /// @brief Constructs a timespan object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<base_dimension<unit_exponent<Unit>>, base_dimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr timespan(const base_dimension<Ts...>& base) : base_dimension<unit_exponent<Unit>>::base_dimension(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   /// @brief Deduction guide for timespan constructor with base_dimension.
   /// @tparam timespanUnit The unit type.
   template<is_timespan Dim>
   timespan(Dim) -> timespan<DimExtractor<timespanType, Dim>>;
}

#endif // STATIC_DIMENSION_timespan_IMPL_H