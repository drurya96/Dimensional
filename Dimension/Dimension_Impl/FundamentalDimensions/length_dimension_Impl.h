#ifndef STATIC_DIMENSION_LENGTH_IMPL_H
#define STATIC_DIMENSION_LENGTH_IMPL_H

#include <type_traits>
#include <tuple>

#include "../../base_dimension_impl.h"

namespace dimension
{
   /// @brief Represents the primary unit for length.
   struct meters;

   using primary_length = meters;

   /// @brief length type tag.
   struct lengthType {
      using primary = primary_length;
   };

   /// @brief Concept: a valid length unit.
   template<typename T>
   concept is_length_unit =
      is_unit<T> && std::is_same_v<unit_dimension_t<T>, lengthType>;

   /// @brief Concept: a length dimension (exactly one length unit after simplification).
   template<typename T>
   concept is_length =
      std::tuple_size_v<simplified_units_t<typename T::units>> == 1 &&
      is_length_unit<typename std::tuple_element_t<0, simplified_units_t<typename T::units>>::unit>;

   /// @brief Get a length object in unit T.
   template<is_length_unit T, is_length DimType>
   constexpr typename DimType::rep get_length_as(DimType obj)
   {
      return get_dimension_as<unit_exponent<T>>(obj);
   }

   template<typename... Ts>
   class length;

   /// @brief length dimension type.
   template<rep_type Rep, is_length_unit Unit, is_coefficient... Cs>
   class length<Rep, Unit, Cs...> : public base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>
   {
      using impl = base_dimension_impl<Rep, unit_exponent<Unit>, Cs...>;

   public:
      /// @brief Default constructs to zero.
      constexpr length() : impl(0.0) {}

      /// @brief Construct with a numeric value.
      explicit constexpr length(double val) : impl(val) {}

      // perfect-forward ctor so factory can pass symbols
      template<typename V, is_coefficient... Ds>
      requires std::is_constructible_v<Rep, V>
      explicit constexpr length(V&& v, Ds... ds)
         : impl(static_cast<Rep>(std::forward<V>(v)), ds...) {}

      template<is_coefficient... Ds>
      requires std::same_as<std::tuple<Cs...>, std::tuple<Ds...>>
      // cppcheck-suppress noExplicitConstructor
      constexpr length(const base_dimension_impl<Rep, unit_exponent<Unit>, Ds...>& src)
         : impl(src) {}

      /// @brief Construct from another base_dimension of same length.
      template<typename... Ts>
      requires is_length<base_dimension_impl<Rep, Ts...>>
      // Implicit conversion between same-dimension types is intentional.
      // cppcheck-suppress noExplicitConstructor
      constexpr length(const base_dimension_impl<Rep, Ts...>& base)
         : impl(get_dimension_as<unit_exponent<Unit>>(base)) {}
   };

   template<is_length_unit U, typename Rep, is_coefficient... Cs>
   requires (!is_coefficient<Rep>)
   [[maybe_unused]]
   constexpr auto make_length(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return length<Rep, U, Cs...>(value);
   }

   template<is_length_unit U, is_coefficient... Cs>
   [[maybe_unused]]
   constexpr auto make_length(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return length<double, U, Cs...>(1.0);   // 1 × coeffs
   }

   template<is_length_unit Unit, is_coefficient... Cs>
   class length<Unit, Cs...> : public length<double, Unit, Cs...>
   {
   public:
      using length<double, Unit, Cs...>::length;
   };

   /// @brief Deduction guides
   template<is_length Dim>
   length(Dim) -> length<
      typename Dim::rep,
      simplified_unit_filter<lengthType, typename Dim::units>>;

   template<rep_type Rep, is_length_unit Unit, is_coefficient... Cs>
   length(const length<Rep, Unit, Cs...>&)
      -> length<Rep, Unit, Cs...>;

   template<rep_type R, is_length_unit U, is_coefficient... Cs>
   length(base_dimension_impl<R, unit_exponent<U>, Cs...>)
      -> length<R, U, Cs...>;
}

#endif // STATIC_DIMENSION_LENGTH_IMPL_H