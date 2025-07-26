#ifndef STATIC_DIMENSION_LENGTH_IMPL_H
#define STATIC_DIMENSION_LENGTH_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension.h"

namespace dimension
{
   /// @brief Represents the length type tag.
   struct lengthType {};

   /// @brief Represents the primary unit for length.
   struct meters;

   using primary_length = meters;

   /// @brief Concept to check if a type is a valid length unit.
   /// @tparam T The type to check.
   template<typename T>
   concept is_length_unit = std::is_base_of_v<FundamentalUnitTag, T> && std::is_same_v<typename T::Dim, lengthType>;

   /// @brief Base class for length units.
   /// @tparam Unit The unit type.
   /// @tparam Name The name of the unit.
   /// @tparam Abbreviation The abbreviation of the unit.
   template<typename Unit, string_literal Name, string_literal Abbreviation, int ID = 0>
   struct lengthUnit : public BaseUnit<Unit, Name, Abbreviation, "length", ID>
   {
   public:
      /// @brief The dimension type associated with the unit.
      using Dim = lengthType;

      /// @brief The primary unit type for this dimension.
      using Primary = primary_length;
   };

   /// @brief Concept to check if a type is a length dimension.
   /// @tparam T The type to check.
   template<typename T>
   concept is_length = requires {
      typename decltype(FullSimplify(T{}))::units;
   } &&
   std::tuple_size_v<typename decltype(FullSimplify(T{}))::units> == 1 &&
   is_length_unit<typename std::tuple_element_t<0, typename decltype(FullSimplify(T{}))::units>::unit>;

   /// @brief Retrieves the value of a length object in the specified unit.
   /// @tparam T The unit type.
   /// @param obj The length object.
   /// @return The value in the specified unit.
   template<is_length_unit T>
   constexpr PrecisionType get_length_as(/*length_type*/ auto obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class length;

   /// @brief Represents a dimension type for length.
   /// @tparam Unit The primary unit type.
   template<rep_type Rep, is_length_unit Unit, is_coefficient... Cs>
   class length<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {

      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructor initializing to zero.
      constexpr length() : impl(0.0) {}

      /// @brief Constructs a length object with a specific value.
      /// @param val The value to initialize with.
      explicit constexpr length(double val) : impl(val) {}

      /* perfect-forward ctor so factory can pass symbols --------------------- */
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr length(V&& v, Ds... ds) : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr length(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src) : impl(src) {}

      /// @brief Constructs a length object from another base_dimension.
      /// @tparam Ts The units of the base_dimension.
      /// @param base The base_dimension object to construct from.
      template<typename... Ts>
      requires matching_dimensions<impl, base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr length(const base_dimension_impl<Rep, Ts...>& base) : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_length_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_length(Rep value, Cs... coeffs)
   {
      return length<Rep, U, Cs...>(value, coeffs...);
   }

   template<is_length_unit U, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_length(Cs... coeffs)
   {
      return length<double, U, Cs...>(1.0, coeffs...);   // 1 × coeffs
   }

   template<is_length_unit Unit, is_coefficient... Cs>
   class length<Unit, Cs...> : public length<double, Unit, Cs...> {
   public:
      using length<double, Unit, Cs...>::length;
   };

   /// @brief Deduction guide for length constructor with base_dimension.
   /// @tparam lengthUnit The unit type.
   template<is_length Dim>
   length(Dim) -> length<typename Dim::rep, DimExtractor<lengthType, Dim>>;

   template<rep_type Rep, is_length_unit Unit, is_coefficient... Cs>
   length(const length<Rep, Unit, Cs...>&) -> length<Rep, Unit, Cs...>;

   template<rep_type R, is_length_unit U, is_coefficient... Cs>
   length(base_dimension_impl<R, unit_exponent<U>, Cs...>) -> length<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_LENGTH_IMPL_H