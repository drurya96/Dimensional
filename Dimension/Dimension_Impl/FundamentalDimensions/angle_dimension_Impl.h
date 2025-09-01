#ifndef STATIC_DIMENSION_ANGLE_IMPL_H
#define STATIC_DIMENSION_ANGLE_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension_impl.h"

namespace dimension
{
   /// @brief Represents the primary unit for angle.
   struct radians;

   using primary_angle = radians;

   /// @brief angle type tag.
   struct angleType {
      using primary = primary_angle;
   };

   /// @brief Concept: a valid angle unit.
   template<typename T>
   concept is_angle_unit =
      is_unit<T> && std::is_same_v<unit_dimension_t<T>, angleType>;

   /// @brief Concept: a angle dimension (exactly one angle unit after simplification).
   template<typename T>
   concept is_angle =
      std::tuple_size_v<simplified_units_t<typename T::units>> == 1 &&
      is_angle_unit<typename std::tuple_element_t<0, simplified_units_t<typename T::units>>::unit>;

   /// @brief Get a angle object in unit T.
   template<is_angle_unit T, is_angle DimType>
   constexpr typename DimType::rep get_angle_as(DimType obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class angle;

   /// @brief angle dimension type.
   template<rep_type Rep, is_angle_unit Unit, is_coefficient... Cs>
   class angle<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {
      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructs to zero.
      constexpr angle() : impl(0.0) {}

      /// @brief Construct with a numeric value.
      explicit constexpr angle(double val) : impl(val) {}

      // perfect-forward ctor so factory can pass symbols
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr angle(V&& v, Ds... ds)
         : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr angle(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src)
         : impl(src) {}

      /// @brief Construct from another base_dimension of same angle.
      template<typename... Ts>
      requires is_angle<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between same-dimension types is intentional.
      // cppcheck-suppress noExplicitConstructor
      constexpr angle(const base_dimension_impl<Rep, Ts...>& base)
         : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_angle_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   [[maybe_unused]]
   constexpr auto make_angle(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return angle<Rep, U, Cs...>(value);
   }

   template<is_angle_unit U, is_coefficient... Cs>
   [[maybe_unused]]
   constexpr auto make_angle(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return angle<double, U, Cs...>(1.0);   // 1 × coeffs
   }

   template<is_angle_unit Unit, is_coefficient... Cs>
   class angle<Unit, Cs...> : public angle<double, Unit, Cs...>
   {
   public:
      using angle<double, Unit, Cs...>::angle;
   };

   /// @brief Deduction guides
   template<is_angle Dim>
   angle(Dim) -> angle<
      typename Dim::rep,
      simplified_unit_filter<angleType, typename Dim::units>>;

   template<rep_type Rep, is_angle_unit Unit, is_coefficient... Cs>
   angle(const angle<Rep, Unit, Cs...>&)
      -> angle<Rep, Unit, Cs...>;

   template<rep_type R, is_angle_unit U, is_coefficient... Cs>
   angle(base_dimension_impl<R, unit_exponent<U>, Cs...>)
      -> angle<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_ANGLE_IMPL_H