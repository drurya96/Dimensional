#ifndef DIMENSION_BASE_DIMENSION_IMPL_H
#define DIMENSION_BASE_DIMENSION_IMPL_H

#include <tuple> // For std::tuple and related functions
#include <cmath> // For std::hypot, std::modf, std::fmod // @todo move this to Utilities
#include <stdexcept> // For std::invalid_argument
#include <numbers> // For std::numbers::pi
#include <ratio>
#include <concepts>
#include <type_traits>
#include <limits>
#include <utility>

#include "Dimension_Core/internal_temp/uuid.h"
#include "Dimension_Core/internal_temp/units/UnitValidation.h"
#include "Dimension_Core/internal_temp/full_simplify.h"
#include "Dimension_Core/internal_temp/units/unit_filter.h"
#include "Dimension_Core/internal_temp/dimensional_equivalence.h"
#include "Dimension_Core/internal_temp/units/collapse_units.h"
#include "Dimension_Core/internal_temp/units/unit_decomposition.h"
#include "Dimension_Core/internal_temp/Conversion.h"
#include "Dimension_Core/internal_temp/SI_Macro.h"
#include "Dimension_Core/streaming/Stream.h"
#include "Dimension_Core/serialization/Serialization.h"
#include "Dimension_Core/serialization/exact_tag_policy.h"
#include "Dimension_Core/serialization/raw_value_policy.h"

#include "Dimension_Core/internal_temp/point/point.h"

namespace dimension
{


   template<are_unit_exponents... Units, typename Dim>
   requires (dimensionally_equivalent<base_dimension_impl<double, Units...>, Dim> && !same_unit_representation<std::tuple<Units...>, typename Dim::units>)
   constexpr Dim::rep get_scalar_as(Dim obj)
   {
      return detail::convert_scalar_units<std::tuple<Units...>>(obj);
   }
   
   template<are_unit_exponents... Units, typename Dim>
   requires same_unit_representation<std::tuple<Units...>, typename Dim::units>
   constexpr Dim::rep get_scalar_as(Dim obj)
   {
      return obj.template get_scalar<Units...>();
   }

   /// @brief Return the internal value as a double in terms of the provided units
   /// @tparam NumTuple tuple of Unit types to convert numerator to
   /// @tparam DenTuple tuple of Unit types to convert denominator to
   /// @return The value in terms of the given units
   template<are_unit_exponents... Units, typename Dim>
   requires (dimensionally_equivalent<base_dimension_impl<double, Units...>, Dim> && !same_unit_representation<std::tuple<Units...>, typename Dim::units>)
   constexpr Dim::rep get_dimension_as(Dim obj)
   {
      // TODO: URGENT: Need to "apply coefficients"
      constexpr double coefficients = ratio_v<typename Dim::ratio> * detail::multiply_symbol_exponent_values_v<typename Dim::symbols>;
      return get_scalar_as<Units...>(obj) * coefficients;
   }
   
   template<are_unit_exponents... Units, typename Dim>
   requires same_unit_representation<std::tuple<Units...>, typename Dim::units>
   constexpr Dim::rep get_dimension_as(Dim obj)
   {
      return obj.template get<Units...>();
   }

   template<typename UnitTuple>
   constexpr base_dimension_from_tuple<UnitTuple>::dim::rep get_dimension_tuple(typename base_dimension_from_tuple<UnitTuple>::dim obj)
   {
      return call_unpack<UnitTuple>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename UnitTuple, typename Dim>
   constexpr Dim::rep get_scalar_tuple(Dim obj)
   {
      return call_unpack<typename Dim::units>([&]<typename... Units> { return get_scalar_as<Units...>(obj); });
   }

   class base_dimension_marker{};

   /// @brief A generic dimension class
   /// @details This class represents a dimension,
   ///    such as length, Time, speed, etc.
   ///    Its templates are used to set the dimensions of the numerator
   ///    and denominator, respectively. For example, a base_dimension
   ///    may be templated on a tuple<lengthUnit> and a tuple<TimeUnit>.
   ///    This would still be a base_dimension, but can be treated as "speed".
   ///    This library may be used with base_dimension alone, but Dimensions
   ///    may also be derived from base_dimension for more readible code,
   ///    for example the length, Time, and speed classes provided.
   ///    Finally, the dimension itself contains a scalar value which is typically
   ///    used when constructing an object without existing units, or when simplifying.
   /// @tparam NumTuple A tuple of BaseUnits describing the dimension's numerator.
   ///    Note all types in NumTuple must derive from BaseUnit
   /// @tparam DenTuple A tuple of BaseUnits describing the dimension's denominator.
   ///    Note all types in DenTuple must derive from BaseUnit
   template<rep_type Rep, is_coefficient_or_unit... Ts>
   class base_dimension_impl : public base_dimension_marker
   {
   public:
      using units = extract_units_t<Ts...>;
      using symbols = extract_symbols_t<Ts...>;
      using ratio = extract_ratio_t<Ts...>;

      using simplified = simplified_units_t<units>;
      using rep = Rep;
      
      //------------------------------------------------------------------
      // 1. default ctor – no run-time coeffs
      //------------------------------------------------------------------
      constexpr base_dimension_impl() noexcept : scalar(Rep{}) {}

      //------------------------------------------------------------------
      // 2. numeric value ctor – no run-time coeffs
      //------------------------------------------------------------------
      explicit constexpr base_dimension_impl(Rep v) noexcept : scalar(v) {}

      //------------------------------------------------------------------
      // 3.  catch-all that triggers a hard error if *any* coeff tags are given
      //------------------------------------------------------------------
      template<is_coefficient... Cs>
      explicit constexpr base_dimension_impl(Rep, Cs...)
      {
         static_assert(sizeof...(Cs) == 0,
                        "run-time coefficient tags (e.g. symbols::pi{}) are disallowed");
      }


      template<typename... OtherUnits>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, OtherUnits...>>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr base_dimension_impl(base_dimension_impl<Rep, OtherUnits...> obj) :
         base_dimension_impl(get_dimension_as<Ts...>(obj))
      {
      }
      
      /// @brief Cast to double operator overload for Scalar types
      /// @details Cast the dimension to a double if unitless (i.e. scalar type) 
      template<typename U = simplified>
      requires (std::tuple_size_v<U> == 0)
      constexpr operator double() const
      {
         return scalar;
      }
      

      /// @brief Negative unary operator
      /// @return base_dimension of the same type with opposite sign
      constexpr base_dimension_impl<Rep, Ts...> operator-() const
      {
         return base_dimension_impl<Rep, Ts...>(-scalar);
      }

      /// @brief += operator overload for another dimension
      /// @tparam NumTuple2 Tuple of numerator types of object being added
      /// @tparam DenTuple2 Tuple of denominator types of object being added
      /// @param[in] rhs The object being added
      template<are_unit_exponents... Units2>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr base_dimension_impl<Rep, Ts...>& operator+=(const base_dimension_impl<Rep, Units2...>& rhs)
      {
         scalar += get_dimension_as<Ts...>(rhs);
         return *this;
      }
      
      /// @brief -= operator overload for another dimension
      /// @tparam NumTuple2 Tuple of numerator types of object being subtracted
      /// @tparam DenTuple2 Tuple of denominator types of object being substracted
      /// @param[in] rhs The object being substracted
      template<are_unit_exponents... Units2>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr base_dimension_impl<Rep, Ts...>& operator-=(const base_dimension_impl<Rep, Units2...>& rhs)
      {
         scalar -= get_dimension_as<Ts...>(rhs);
         return *this;
      }

      /// @brief *= operator overload for a scalar
      /// @param[in] rhs scalar value to multiply by
      constexpr base_dimension_impl<Rep, Ts...>& operator*=(Rep rhs)
      {
         scalar *= rhs;
         return *this;
      }

      /// @brief /= operator overload for a scalar
      /// @param[in] rhs scalar value to divide by
      constexpr base_dimension_impl<Rep, Ts...>& operator/=(Rep rhs)
      {
         scalar /= rhs;
         return *this;
      }
      
      // The following operators are explicitly deleted
      base_dimension_impl<Rep, Ts...>& operator*=(const base_dimension_impl<Rep, Ts...>& rhs) = delete; // Multiplication results in a different type
      base_dimension_impl<Rep, Ts...>& operator/=(const base_dimension_impl<Rep, Ts...>& rhs) = delete; // Division results in a different type
      base_dimension_impl<Rep, Ts...>& operator+=(Rep rhs) = delete; // Addition cannot be performed between a dimension and a scalar
      base_dimension_impl<Rep, Ts...>& operator-=(Rep rhs) = delete; // Subtraction cannot be performed between a dimension and a scalar

      template<typename... Units2>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator<(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar < get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator>(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar > get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator<=(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar <= get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator>=(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar >= get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator==(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar == get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires dimensionally_equivalent<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator!=(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return !(*this == rhs);
      }

      template<typename... Units2>
      [[nodiscard]] constexpr Rep get() const
      {
         static_assert(same_unit_representation<units, std::tuple<Units2...>>,
            "get is an implementation detail of Dimensional and is not meant to be called externally! Prefer get_dimension_as. When using get directly, template parameter units must exactly match units of the object."
         );

         return static_cast<Rep>(scalar *
                                 ratio_v<ratio> * 
                                 detail::multiply_symbol_exponent_values_v<symbols>);
      }

      template<typename Tuple>
      [[nodiscard]] constexpr Rep get_tuple() const
      {
         static_assert(same_unit_representation<units, Tuple>,
            "get_tuple is an implementation detail of Dimensional and is not meant to be called externally! Prefer get_dimension_as. When using get directly, template parameter units must exactly match units of the object."
         );

         return static_cast<Rep>(scalar *
                                 ratio_v<ratio> * 
                                 detail::multiply_symbol_exponent_values_v<symbols>);
      }

      template<typename... Units2>
      [[nodiscard]] constexpr Rep get_scalar() const
      {
         static_assert(same_unit_representation<units, std::tuple<Units2...>>,
            "get_scalar is an implementation detail of Dimensional and is not meant to be called externally! Prefer get_dimension_as. When using get directly, template parameter units must exactly match units of the object."
         );

         return static_cast<Rep>(scalar);
      }

      template<typename Tuple>
      [[nodiscard]] constexpr Rep get_tuple_scalar() const
      {
         static_assert(same_unit_representation<units, Tuple>,
            "get_tuple_scalar is an implementation detail of Dimensional and is not meant to be called externally! Prefer get_dimension_as. When using get directly, template parameter units must exactly match units of the object."
         );

         return static_cast<Rep>(scalar);
      }

   private:
      /// @brief The scalar value of this dimension
      Rep scalar;
   };

   // ─── generic factory ──────────────────────────────────────────────
   template<are_unit_exponents... Us,
            typename Rep,                          // deduced
            is_coefficient... Cs>                  // deduced
   // TODO: Unit test this and remove suppression
   // cppcheck-suppress unusedFunction
   constexpr auto make_dimension(Rep value, Cs... coeffs)
   {
      // compile-time guard: Rep must work with base_dimension’s ctor
      static_assert(requires(Rep v)
                     { base_dimension_impl<Rep, Us..., Cs...>(v, coeffs...); },
                     "make_dimension: provided value type cannot be used as Rep");

      return base_dimension_impl<Rep, Us..., Cs...>(value);
   }

   /// @brief Division operator for two Dimensions
   /// @tparam NumTuple1 Tuple of numerator units of obj1
   /// @tparam DenTuple1 Tuple of denominator units of obj1
   /// @tparam NumTuple2 Tuple of numerator units of obj2
   /// @tparam DenTuple2 Tuple of denominator units of obj2
   /// @param[in] obj1 The numerator base_dimension object
   /// @param[in] obj2 The denominator base_dimension object
   /// @return A base dimension object templated on the numerator types and
   ///    the denominator types, then simplified.
   template<is_base_dimension Lhs, is_base_dimension Rhs>
   constexpr auto operator/(const Lhs& lhs, const Rhs& rhs)
   {
      using Rep = std::common_type_t<typename Lhs::rep, typename Rhs::rep>;
      using ratio = std::ratio_divide<typename Lhs::ratio, typename Rhs::ratio>;
      using symbols = detail::divide_symbol_tuples_t<typename Lhs::symbols, typename Rhs::symbols>;
      //using symbols = std::tuple<>;
      using units_combined = tuple_cat_t<typename Lhs::units, typename FlipExponents<typename Rhs::units>::units>;
      using units = collapse_units_t<units_combined>;

      return typename base_dimension_from_tuple<Rep, ratio, units, symbols>::dim(
         get_scalar_tuple<typename Lhs::units>(lhs) /
         get_scalar_tuple<typename Rhs::units>(rhs)
      );
   }

   /// @brief Multiplication operator for two Dimensions
   /// @tparam NumTuple1 Tuple of numerator units of obj1
   /// @tparam DenTuple1 Tuple of denominator units of obj1
   /// @tparam NumTuple2 Tuple of numerator units of obj2
   /// @tparam DenTuple2 Tuple of denominator units of obj2
   /// @param[in] obj1 The first base_dimension object
   /// @param[in] obj2 The second base_dimension object
   /// @return A base dimension object templated on the types of both
   ///    input objects, then simplified.
   template<is_base_dimension Lhs, is_base_dimension Rhs>
   constexpr auto operator*(const Lhs& lhs, const Rhs& rhs)
   {
      using Rep = std::common_type_t<typename Lhs::rep, typename Rhs::rep>;
      using ratio = std::ratio_multiply<typename Lhs::ratio, typename Rhs::ratio>;
      using symbols = detail::multiply_symbol_tuples_t<typename Lhs::symbols, typename Rhs::symbols>;
      //using symbols = std::tuple<>;
      using units_combined = tuple_cat_t<typename Lhs::units, typename Rhs::units>;
      using units = collapse_units_t<units_combined>;
      
      return typename base_dimension_from_tuple<Rep, ratio, units, symbols>::dim(
         get_scalar_tuple<typename Lhs::units>(lhs) *
         get_scalar_tuple<typename Rhs::units>(rhs)
      );
      
   }
 
   // Scalar Math

   // Multiply base_dimension * scalar
   template<typename Lhs>
   constexpr auto operator*(const Lhs& lhs, double scalar)
   {
      return typename base_dimension_from_tuple<typename Lhs::units>::dim(
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(lhs); }) * scalar
      );
   }

   // Multiply scalar * base_dimension
   template<typename Rhs>
   constexpr auto operator*(double scalar, const Rhs& rhs)
   {
      return rhs * scalar; // Just reuse the other overload
   }

   // Divide base_dimension / scalar
   template<typename Lhs>
   constexpr auto operator/(const Lhs& lhs, double scalar)
   {
      return typename base_dimension_from_tuple<typename Lhs::units>::dim(
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(lhs); }) / scalar
      );
   }

   // Divide scalar / base_dimension --> flip units
   template<typename Rhs>
   constexpr auto operator/(double scalar, const Rhs& rhs)
   {
      return typename base_dimension_from_tuple<typename FlipExponents<typename Rhs::units>::units>::dim(
         scalar / call_unpack<typename Rhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(rhs); })
      );
   }


   // ==============================
   //   Addition
   // ==============================
   template<is_base_dimension Lhs, is_base_dimension Rhs>
   constexpr auto operator+(const Lhs& lhs, const Rhs& rhs)
   {
      return typename base_dimension_from_tuple<typename Lhs::units>::dim(
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(lhs); }) +
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(rhs); })
      );
   }

   // ==============================
   //   Subtraction
   // ==============================
   template<is_base_dimension Lhs, is_base_dimension Rhs>
   constexpr auto operator-(const Lhs& lhs, const Rhs& rhs)
   {
      return typename base_dimension_from_tuple<typename Lhs::units>::dim(
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(lhs); }) -
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(rhs); })
      );
   }
}

#endif // DIMENSION_BASE_DIMENSION_IMPL_H
