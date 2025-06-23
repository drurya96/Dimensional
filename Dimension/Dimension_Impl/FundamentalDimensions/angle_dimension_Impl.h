#ifndef STATIC_DIMENSION_angle_IMPL_H
#define STATIC_DIMENSION_angle_IMPL_H

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the angle type tag.
   struct angleType {};

   /// @brief Represents the primary unit for angle.
   struct radians;

   using Primaryangle = radians;

   /// @brief Concept to check if a type is a valid angle unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_angle_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, angleType>;

   /// @brief Base class for angle units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, int ID = 0>
   struct angleUnit : public BaseUnit<Unit, Name, Abbreviation, "angle", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = angleType;

      /// @brief The primary unit type for this dimension.
      using Primary = Primaryangle;
   };

   /// @brief Concept to check if a type is a angle dimension.
   /// @tparam T The type to check.
   template<typename T>
   concept is_angle = requires {
      typename decltype(FullSimplify(T{}))::units;
   } &&
   std::tuple_size_v<typename decltype(FullSimplify(T{}))::units> == 1 &&
   is_angle_unit<typename std::tuple_element_t<0, typename decltype(FullSimplify(T{}))::units>::unit>;

   /// @brief Retrieves the value of a angle object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The angle object.
   /// @return The value in the specified unit.
   template<is_angle_unit T>
   constexpr PrecisionType get_angle_as(/*angle_type*/ auto obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<is_angle_unit Unit = Primaryangle, bool isQuantity = false>
   class angle;

   /// @brief Represents a dimension type for angle.
   /// @tparam Unit The primary unit type.
   template<is_angle_unit Unit>
   class angle<Unit, false> : public base_dimension<unit_exponent<Unit>>
   {
   public:
      /// @brief Default constructor initializing to zero.
      constexpr angle() : base_dimension<unit_exponent<Unit>>::base_dimension(0.0) {}

      /// @brief Constructs a angle object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr angle(double val) : base_dimension<unit_exponent<Unit>>::base_dimension(val) {}

      /// @brief Constructs a angle object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<base_dimension<unit_exponent<Unit>>, base_dimension<Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr angle(const base_dimension<Ts...>& base) : base_dimension<unit_exponent<Unit>>::base_dimension(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   /// @brief Deduction guide for angle constructor with base_dimension.
   /// @tparam angleUnit The unit type.
   template<is_angle Dim>
   angle(Dim) -> angle<DimExtractor<angleType, Dim>>;
}

#endif // STATIC_DIMENSION_angle_IMPL_H