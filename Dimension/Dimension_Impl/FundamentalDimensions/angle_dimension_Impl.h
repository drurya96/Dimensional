#ifndef STATIC_DIMENSION_ANGLE_IMPL_H
#define STATIC_DIMENSION_ANGLE_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the angle type tag.
   struct angleType {};

   /// @brief Represents the primary unit for angle.
   struct radians;

   using primary_angle = radians;

   /// @brief Concept to check if a type is a valid angle unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_angle_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, angleType>;

   /// @brief Base class for angle units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, string_literal Name, string_literal Abbreviation, int ID = 0>
   struct angleUnit : public BaseUnit<Unit, Name, Abbreviation, "angle", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = angleType;

      /// @brief The primary unit type for this dimension.
      using Primary = primary_angle;
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

   template<typename... Ts>
   class angle;

   /// @brief Represents a dimension type for angle.
   /// @tparam Unit The primary unit type.
   template<rep_type Rep, is_angle_unit Unit, is_coefficient... Cs>
   class angle<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {

      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructor initializing to zero.
      constexpr angle() : impl(0.0) {}

      /// @brief Constructs a angle object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr angle(double val) : impl(val) {}

      /* perfect-forward ctor so factory can pass symbols --------------------- */
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr angle(V&& v, Ds... ds) : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr angle(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src) : impl(src) {}

      /// @brief Constructs a angle object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<impl, base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr angle(const base_dimension_impl<Rep, Ts...>& base) : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_angle_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_angle(Rep value, Cs... coeffs)
   {
      return angle<Rep, U, Cs...>(value, coeffs...);
   }

   template<is_angle_unit U, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_angle(Cs... coeffs)
   {
      return angle<double, U, Cs...>(1.0, coeffs...);   // 1 × coeffs
   }

   template<is_angle_unit Unit, is_coefficient... Cs>
   class angle<Unit, Cs...> : public angle<double, Unit, Cs...> {
   public:
      using angle<double, Unit, Cs...>::angle;
   };

   /// @brief Deduction guide for angle constructor with base_dimension.
   /// @tparam angleUnit The unit type.
   template<is_angle Dim>
   angle(Dim) -> angle<typename Dim::rep, DimExtractor<angleType, Dim>>;

   template<rep_type Rep, is_angle_unit Unit, is_coefficient... Cs>
   angle(const angle<Rep, Unit, Cs...>&) -> angle<Rep, Unit, Cs...>;

   template<rep_type R, is_angle_unit U, is_coefficient... Cs>
   angle(base_dimension_impl<R, unit_exponent<U>, Cs...>) -> angle<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_ANGLE_IMPL_H