#ifndef STATIC_DIMENSION_BASE_H
#define STATIC_DIMENSION_BASE_H

#include <tuple> // For std::tuple and related functions
#include <cmath> // For std::hypot, std::modf, std::fmod // @todo move this to Utilities
#include <stdexcept> // For std::invalid_argument
#include <numbers> // For std::numbers::pi
#include <ratio>
#include <concepts>
#include <type_traits>
#include <limits>
#include <utility>

#include "Dimension_Core/PrecisionType.h"
#include "Dimension_Core/UnitValidation.h"
#include "Dimension_Core/UnitSimplifier.h"
#include "Dimension_Core/FundamentalUnitExtractor.h"
#include "Dimension_Core/Conversion.h"
#include "Dimension_Core/SI_Macro.h"
#include "Dimension_Core/Hashing.h"
#include "Dimension_Core/StringLiteral.h"
#include "Dimension_Core/Stream.h"
#include "Dimension_Core/Serialization.h"
#include "Dimension_Core/base_dimension_signature.h"
#include "Dimension_Core/Coefficient.h"

#include "Dimension_Core/Point.h"

namespace dimension
{

   static constexpr StringLiteral<3> delim = "::"; // Size three due to null terminator

   struct FundamentalUnitTag {};

   /// @brief A base class representing a unit
   /// @details This abstract class represents a Unit,
   ///    such as meters, seconds, Grams, etc.
   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation, StringLiteral DimName, int UnitID = 0>
   struct BaseUnit : FundamentalUnitTag
   {
   public:
      /// @brief No constructor
      BaseUnit() = delete;

      // @TODO: Figure out where these are used and remove them.
      using NumTuple = std::tuple<Unit>;
      using DenTuple = std::tuple<>;

      using units = std::tuple<unit_exponent<Unit>>;
      using unit = Unit;

      /// @brief Used to handle subscripting
      /// @details Units only cancel if this value is the same.
      ///    This means, creating units with different IDs and combining them
      ///    into one dimension will prevent them from canelling out.
      constexpr static int ID = UnitID;

      static constexpr StringLiteral<Name.size> name = Name;
      static constexpr StringLiteral<Abbreviation.size> abbr = Abbreviation;
      static constexpr StringLiteral<DimName.size> dimName = DimName;

      static constexpr StringLiteral<DimName.size + delim.size - 1> intermediate = concat(dimName, delim); // size - 1 to account for removed null terminator from first param
      static constexpr StringLiteral<intermediate.size + name.size - 1> qualifiedName = concat(intermediate, name); // size - 1 to account for removed null terminator from first param

      using name_type = decltype(Name);
      using abbr_type = decltype(Abbreviation);
      using dimName_type = decltype(DimName);
   };

   /// @brief Return the internal value as a double in terms of the provided units
   /// @tparam NumTuple tuple of Unit types to convert numerator to
   /// @tparam DenTuple tuple of Unit types to convert denominator to
   /// @return A PrecisionType representing the value in terms of the given units
   template<are_unit_exponents... Units, typename Dim>
   requires (matching_dimensions<base_dimension_impl<double, Units...>, Dim> && !same_units<std::tuple<Units...>, typename Dim::units>)
   constexpr Dim::rep get_dimension_as(Dim obj)
   {
      return ConvertDim<typename Dim::units, std::tuple<Units...>>::Convert(
         call_unpack<typename Dim::units>([&]<typename... OrigUnits> { return get_dimension_as<OrigUnits...>(static_cast<const base_dimension_impl<double, OrigUnits...>&>(obj)); })
      );
   }
   
   template<are_unit_exponents... Units, typename Dim>
   requires same_units<std::tuple<Units...>, typename Dim::units>
   constexpr Dim::rep get_dimension_as(Dim obj)
   {
      return obj.template get<Units...>();
   }

   template<typename UnitTuple>
   constexpr PrecisionType get_dimension_tuple(typename base_dimensionFromTuple<UnitTuple>::dim obj)
   {
      return call_unpack<UnitTuple>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<are_unit_exponents... Units, typename Dim>
   requires (matching_dimensions<base_dimension_impl<double, Units...>, Dim> && !same_units<std::tuple<Units...>, typename Dim::units>)
   constexpr Dim::rep get_scalar_as(Dim obj)
   {
      return ConvertDim<typename Dim::units, std::tuple<Units...>>::Convert(
         call_unpack<typename Dim::units>([&]<typename... OrigUnits> { return get_scalar_as<OrigUnits...>(static_cast<const base_dimension_impl<double, OrigUnits...>&>(obj)); })
      );
   }
   
   template<are_unit_exponents... Units, typename Dim>
   requires same_units<std::tuple<Units...>, typename Dim::units>
   constexpr Dim::rep get_scalar_as(Dim obj)
   {
      return obj.template get_scalar<Units...>();
   }

   template<typename UnitTuple, typename Dim>
   constexpr PrecisionType get_scalar_tuple(Dim obj)
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
   private:
      using ts_split = partition_coeffs<Ts...>;

      template<typename... Us>
      static constexpr void ignore_unused(Us&&...) noexcept {}

   public:

      using units = typename ts_split::units;
      using coeffs = typename ts_split::coeffs;
      using symbols = typename handle_coefficients_tuple<coeffs>::symbols;
      using ratio = typename handle_coefficients_tuple<coeffs>::ratio;

      using simplified = FullSimplifyType<units>::final_units;
      using rep = Rep;
      
      //------------------------------------------------------------------
      // 1. default / “coefficients-only” constructor
      //------------------------------------------------------------------
      template<is_coefficient... Cs>
      constexpr base_dimension_impl(Cs... incoming_coeffs) noexcept
         : scalar(Rep{0})
      {
         static_assert(sizeof...(Cs) == 0 ||               // plain default
                        (std::is_empty_v<Cs> && ...),        // all tags are EBO
                        "Run-time coefficient arguments must be empty types");
         ignore_unused(incoming_coeffs...);                          // discard at run time
      }

      //------------------------------------------------------------------
      // 2. value (+ optional coefficients) constructor
      //------------------------------------------------------------------
      template<is_coefficient... Cs>
      explicit constexpr base_dimension_impl(Rep v,
                                             Cs... incoming_coeffs) noexcept
         : scalar(v)
      {
         static_assert((std::is_empty_v<Cs> && ...),
                        "Run-time coefficient arguments must be empty types");
         ignore_unused(incoming_coeffs...);
      }
      
      template<typename... OtherUnits>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, OtherUnits...>>
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
      //requires MatchingDimensionsNew<base_dimension_impl<NumTuple, DenTuple>, base_dimension_impl<NumTuple2, DenTuple2>>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
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
      //requires MatchingDimensionsNew<base_dimension_impl<NumTuple, DenTuple>, base_dimension_impl<NumTuple2, DenTuple2>>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr base_dimension_impl<Rep, Ts...>& operator-=(const base_dimension_impl<Rep, Units2...>& rhs)
      {
         scalar -= get_dimension_as<Ts...>(rhs);
         return *this;
      }

      /// @brief *= operator overload for a scalar
      /// @param[in] rhs scalar value to multiply by
      constexpr base_dimension_impl<Rep, Ts...>& operator*=(PrecisionType rhs)
      {
         scalar *= rhs;
         return *this;
      }

      /// @brief /= operator overload for a scalar
      /// @param[in] rhs scalar value to divide by
      constexpr base_dimension_impl<Rep, Ts...>& operator/=(PrecisionType rhs)
      {
         scalar /= rhs;
         return *this;
      }
      
      // The following operators are explicitly deleted
      base_dimension_impl<Rep, Ts...>& operator*=(const base_dimension_impl<Rep, Ts...>& rhs) = delete; // Multiplication results in a different type
      base_dimension_impl<Rep, Ts...>& operator/=(const base_dimension_impl<Rep, Ts...>& rhs) = delete; // Division results in a different type
      base_dimension_impl<Rep, Ts...>& operator+=(PrecisionType rhs) = delete; // Addition cannot be performed between a dimension and a scalar
      base_dimension_impl<Rep, Ts...>& operator-=(PrecisionType rhs) = delete; // Subtraction cannot be performed between a dimension and a scalar

      template<typename... Units2>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator<(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar < get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator>(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar > get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator<=(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar <= get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator>=(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar >= get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator==(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return scalar == get_dimension_as<Ts...>(rhs);
      }

      template<typename... Units2>
      requires matching_dimensions<base_dimension_impl<Rep, Ts...>, base_dimension_impl<Rep, Units2...>>
      constexpr bool operator!=(const base_dimension_impl<Rep, Units2...>& rhs) const {
         return !(*this == rhs);
      }

      template<typename... Units2>
      [[nodiscard]] constexpr Rep get() const
      {
         static_assert(same_units<units, std::tuple<Units2...>>,
            "get is an implementation detail of Dimensional and is not meant to be called externally! Prefer get_dimension_as. When using get directly, template parameter units must exactly match units of the object."
         );

         return static_cast<Rep>(scalar *
                                 ratio_value<ratio>() * 
                                 eval_symbol_tuple<symbols>());
      }

      template<typename... Units2>
      [[nodiscard]] constexpr Rep get_scalar() const
      {
         static_assert(same_units<units, std::tuple<Units2...>>,
            "get_scalar is an implementation detail of Dimensional and is not meant to be called externally! Prefer get_dimension_as. When using get directly, template parameter units must exactly match units of the object."
         );

         return static_cast<Rep>(scalar);
      }

      template<typename Tuple>
      [[nodiscard]] constexpr Rep get_tuple_scalar() const
      {
         static_assert(same_units<units, Tuple>,
            "get_tuple_scalar is an implementation detail of Dimensional and is not meant to be called externally! Prefer get_dimension_as. When using get directly, template parameter units must exactly match units of the object."
         );

         return static_cast<Rep>(scalar);
      }

   private:
      /// @brief The scalar value of this dimension
      Rep scalar;
   };

   // @TODO: Re-evaluate this
   // ────────────────────────────────────────────────────────────────
   //  base_dimension_wrapper  — revised specialisations
   // ────────────────────────────────────────────────────────────────

   // 1.  Rep specified  +  at least ONE unit  + optional coeffs/units
   template<rep_type Rep,
            are_unit_exponents First,
            is_coefficient_or_unit... Rest>
   struct base_dimension_wrapper<Rep, First, Rest...>
   {
      using type = base_dimension_impl<Rep, First, Rest...>;
   };

   // 2.  Rep omitted (defaults to double)  +  at least ONE unit  + optional coeffs/units
   template<are_unit_exponents First,
            is_coefficient_or_unit... Rest>
   struct base_dimension_wrapper<First, Rest...>
   {
      using type = base_dimension_impl<double, First, Rest...>;
   };

   // 3.  Explicit Rep only      (unchanged)
   template<rep_type Rep>
   struct base_dimension_wrapper<Rep>
   {
      using type = base_dimension_impl<Rep>;
   };

   // 4.  No arguments            (unchanged)
   template<>
   struct base_dimension_wrapper<>
   {
      using type = base_dimension_impl<double>;
   };

   //--------------------------------------------------------------------------
   // 5.  Rep *and* at least one coefficient (no unit exponents)
   //--------------------------------------------------------------------------
   template<rep_type Rep,
            is_coefficient FirstCoeff,
            is_coefficient... RestCoeffs>
   struct base_dimension_wrapper<Rep, FirstCoeff, RestCoeffs...>
   {
      using type = base_dimension_impl<Rep, FirstCoeff, RestCoeffs...>;
   };

   //--------------------------------------------------------------------------
   // 6.  No Rep given   +   coefficient pack only  →  default Rep = double
   //--------------------------------------------------------------------------
   template<is_coefficient FirstCoeff,
            is_coefficient... RestCoeffs>
   struct base_dimension_wrapper<FirstCoeff, RestCoeffs...>
   {
      using type = base_dimension_impl<double, FirstCoeff, RestCoeffs...>;
   };

   // ─── generic factory ──────────────────────────────────────────────
   template<are_unit_exponents U,
            typename Rep,                          // deduced
            is_coefficient... Cs>                  // deduced
   // TODO: Unit test this and remove suppression
   // cppcheck-suppress unusedFunction
   constexpr auto make_dimension(Rep value, Cs... coeffs)
   {
      // compile-time guard: Rep must work with base_dimension’s ctor
      static_assert(requires(Rep v)
                     { base_dimension<Rep, U, Cs...>(v, coeffs...); },
                     "make_dimension: provided value type cannot be used as Rep");

      return base_dimension<Rep, U, Cs...>(value, coeffs...);
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
      using symbols = typename divide_symbol_tuples<typename Lhs::symbols, typename Rhs::symbols>::type;
      using units_combined = tuple_cat_t<typename Lhs::units, typename FlipExponents<typename Rhs::units>::units>;
      using units = typename InitialSimplifier<units_combined>::units;

      return typename base_dimensionFromTuple<Rep, ratio, units, symbols>::dim(
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
      using symbols = typename multiply_symbol_tuples<typename Lhs::symbols, typename Rhs::symbols>::type;
      using units_combined = tuple_cat_t<typename Lhs::units, typename Rhs::units>;
      using units = typename InitialSimplifier<units_combined>::units;
      
      return typename base_dimensionFromTuple<Rep, ratio, units, symbols>::dim(
         get_scalar_tuple<typename Lhs::units>(lhs) *
         get_scalar_tuple<typename Rhs::units>(rhs)
      );
      
   }
 
   // Scalar Math

   // Multiply base_dimension * scalar
   template<typename Lhs>
   constexpr auto operator*(const Lhs& lhs, double scalar)
   {
      return typename base_dimensionFromTuple<typename Lhs::units>::dim(
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
      return typename base_dimensionFromTuple<typename Lhs::units>::dim(
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(lhs); }) / scalar
      );
   }

   // Divide scalar / base_dimension --> flip units
   template<typename Rhs>
   constexpr auto operator/(double scalar, const Rhs& rhs)
   {
      return typename base_dimensionFromTuple<typename FlipExponents<typename Rhs::units>::units>::dim(
         scalar / call_unpack<typename Rhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(rhs); })
      );
   }


   // ==============================
   //   Addition
   // ==============================
   template<is_base_dimension Lhs, is_base_dimension Rhs>
   constexpr auto operator+(const Lhs& lhs, const Rhs& rhs)
   {
      return typename base_dimensionFromTuple<typename Lhs::units>::dim(
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
      return typename base_dimensionFromTuple<typename Lhs::units>::dim(
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(lhs); }) -
         call_unpack<typename Lhs::units>([&]<typename... Units> { return get_dimension_as<Units...>(rhs); })
      );
   }

   /// @brief Implementation for Pow (exponential)
   /// @details Uses exponentiation by squares method
   /// @tparam exponent the exponent to raise to. This must be a positive integer
   /// @tparam Dim The input dimension type
   /// @tparam TransitiveDim The dimension type returned from this recursive call of PowImpl
   /// @param[in] obj The input dimension object
   /// @param[in] transitiveDim The transitive object from the previous recursive call
   /// @return transitiveDim multiplied by obj either one or two times
   ///    for even or odd exponents, respectively.
   template<unsigned int exponent, is_base_dimension T>
   constexpr auto PowImpl(const T& base)
   {
      if constexpr (exponent == 0)
      {
         // 0th power: returns dimensionless with value 1.0
         return base_dimension<>{1.0};
      }
      else if constexpr (exponent == 1)
      {
         return base;
      }
      else if constexpr (exponent % 2 == 0)
      {
         auto half = PowImpl<exponent / 2>(base);
         return half * half;
      }
      else
      {
         auto half = PowImpl<exponent / 2>(base);
         return half * half * base;
      }
   }

   /// @brief Implementation for Pow (exponential)
   /// @tparam exponent the exponent to raise to. This must be a positive integer
   /// @tparam Dim The input dimension type
   /// @param[in] obj The object to raise to exponent power
   /// @return A dimension object of units raised to exponent power,
   ///    and a value raised to exponent power
   template<unsigned int exponent, typename Dim>
   [[nodiscard]] constexpr auto Pow(Dim obj)
   {
      return PowImpl<exponent>(obj);
   }

   /// @brief Calculate hypotenuse from two sides of a right triangle
   /// @tparam T Side type, must be a base_dimension
   /// @param obj1 Side of right triangle
   /// @param obj2 Side of right triangle
   /// @return Hypotenuse of right triangle
   template<is_base_dimension T>
   [[nodiscard]] constexpr T hypot(T obj1, T obj2)
   {
      return T(std::hypot(
         call_unpack<typename T::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj1); }),
         call_unpack<typename T::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj2); })
      ));
   }

   /// @brief Calculate absolute value of a dimension
   /// @tparam T dimension type
   /// @param obj dimension object
   /// @return Absolute value of the dimension
   template<is_base_dimension T>
   [[nodiscard]] constexpr T abs(T obj)
   {
      return obj < T{0} ? -obj : obj;
   }

   /// @brief Round dimension down to nearest whole number
   /// @tparam T dimension type
   /// @param obj dimension object
   /// @return dimension object rounded down to nearest whole number
   template<is_base_dimension T>
   [[nodiscard]] constexpr T floor(const T& obj)
   {
      return T(static_cast<PrecisionType>(static_cast<int>(get_dimension_tuple<typename T::units>(obj)) - 
              (get_dimension_tuple<typename T::units>(obj) < static_cast<int>(get_dimension_tuple<typename T::units>(obj)))));
   }

   /// @brief Round dimension up to nearest whole number
   /// @tparam T dimension type
   /// @param obj dimension object
   /// @return dimension object rounded up to nearest whole number
   template<is_base_dimension T>
   [[nodiscard]] constexpr T ceil(const T& obj)
   {
      return T(static_cast<PrecisionType>(static_cast<int>(get_dimension_tuple<typename T::units>(obj)) +
               (get_dimension_tuple<typename T::units>(obj) > static_cast<int>(get_dimension_tuple<typename T::units>(obj)))));
   }

   /// @brief Round dimension to nearest whole number
   /// @tparam T dimension type
   /// @param obj dimension object
   /// @return dimension object rounded to nearest whole number
   template<is_base_dimension T>
   [[nodiscard]] constexpr T round(const T& obj)
   {
      const auto val = get_dimension_tuple<typename T::units>(obj);
      const auto floor_val = static_cast<PrecisionType>(static_cast<int>(val));
      return T((val - floor_val < 0.5) ? floor_val : floor_val + 1.0);
   }

   /// @brief Decompose dimension into integer and floating point type
   /// @tparam T dimension type
   /// @param obj dimension object to decompose
   /// @param[out] intPart A pointer to a dimension object which will store the integer part
   /// @return The fractional component of the dimension
   /// @todo Consider marking this function constexpr in C++23 (modf is constexpr in C++23)
   template<is_base_dimension T>
   [[nodiscard]] constexpr T modf(const T& obj, T* intPart = nullptr)
   {
      const auto val = get_dimension_tuple<typename T::units>(obj);
      const auto int_val = static_cast<PrecisionType>(static_cast<long long>(val));
      if (intPart) {
         *intPart = T(int_val);
      }
      return T(val - int_val);
   }

   /// @brief Floating point remainder division of dividend / divisor
   /// @tparam T dimension type
   /// @param dividend dimension object to be divided
   /// @param divisor dimension object to divide by
   /// @return dimension object of floating point remainder division (modulus)
   /// @todo Consider marking this function constexpr in C++23 (fmod is constexpr in C++23)
   template<is_base_dimension T>
   [[nodiscard]] constexpr T fmod(const T& dividend, const T& divisor)
   {
      const auto num = get_dimension_tuple<typename T::units>(dividend);
      const auto denom = get_dimension_tuple<typename T::units>(divisor);
      return T(denom == 0 ? throw std::invalid_argument("Divisor cannot be zero.")
                          : num - static_cast<PrecisionType>(static_cast<long long>(num / denom)) * denom);
   }
   
}

#endif // STATIC_DIMENSION_BASE_H
