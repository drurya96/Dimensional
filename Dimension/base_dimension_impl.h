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

#include "Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"
#include "Dimension_Core/internal_temp/dimension/dimensional_equivalence.h"
#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/UnitValidation.h"
#include "Dimension_Core/internal_temp/dimension/full_simplify.h"
#include "Dimension_Core/internal_temp/units/unit_filter.h"
#include "Dimension_Core/internal_temp/units/collapse_units.h"
#include "Dimension_Core/internal_temp/units/unit_decomposition.h"
#include "Dimension_Core/internal_temp/units/Conversion.h"
#include "Dimension_Core/streaming/Stream.h"
#include "Dimension_Core/serialization/Serialization.h"
#include "Dimension_Core/serialization/exact_tag_policy.h"
#include "Dimension_Core/serialization/raw_value_policy.h"

#include "Dimension_Core/internal_temp/units/get_factor.h"

#include "Dimension_Core/internal_temp/point/point.h"

namespace dimension
{

   // Consider moving this to some helper...
   // This relies on specific stuff from the simplification header.. probably really shouldn't be here.
   template<typename ToTuple, typename FromDim>
   static constexpr double convert_scalar_units(FromDim input) 
   {
      return 
         detail::ConvertSimplified<simplified_units_t<typename FromDim::units>, simplified_units_t<ToTuple>>::scalar * // conversion factor from-to
         simplification::SimplifiedDimension<typename FromDim::units>::convert_scalar( // No idea...
            input.get_raw() // From as a scalar
         );
   }

   // New struct that casts a dimension to another set of units, but retains as much compile-time knowledge as possible.
   // So the conversion factors should be applied and any coefficients should be simplified.
   // This means the scalar value will necessarily be the same, meaning this really isn't a function, its a struct.
   // This new struct probably doens't belong in this header, but writing here first might be simpler.
   template<class From, class TargetTuple>
   using find_equivalent_unit_t = typename MatchUnit<From, TargetTuple>::type;

   // Primary template
   // Expected tuple of unit_exponent
   // IMPORTANT: ASSUMES UNITS ARE SIMPLIFIED!
   template<class StartingTuple, class TargetTuple>
   struct apply_all_conversions;

   // Identity: same unit representation → no conversions
   template<class... U>
   struct apply_all_conversions<std::tuple<U...>, std::tuple<U...>> {
      using factor = factor_t<std::ratio<1>, std::tuple<>>;
      using ratio   = std::ratio<1>;
      using symbols = std::tuple<>;
   };

   // General case: dimensionally equivalent but different representations
   template<class... StartingUnits, class... TargetUnits>
   requires (
      dimensionally_equivalent<
         base_dimension_impl<double, StartingUnits...>,
         base_dimension_impl<double, TargetUnits...>> &&
      !same_unit_representation<std::tuple<StartingUnits...>, std::tuple<TargetUnits...>>
   )
   struct apply_all_conversions<std::tuple<StartingUnits...>, std::tuple<TargetUnits...>> {
   private:
      using target_tuple = typename unit_decomposition<std::tuple<TargetUnits...>>::units;
      
      // Map each S to its matching T in target tuple and compute its factor
      template<class UE>
      using matched_target_t = find_equivalent_unit_t<UE, target_tuple>;

      template<class UE>
      using factor_for_t = details::get_factor_t<typename UE::unit, typename matched_target_t<UE>::unit>;

   public:
         // Fold ratios and concatenate symbols from all per-pair factors
         using ratio = detail::ratio_impl::ratio_mul_t<
               std::ratio<1>,
               typename factor_for_t<StartingUnits>::ratio...>;

         using symbols = tuple_cat_t<
               typename factor_for_t<StartingUnits>::symbols...>;

         using factors =
         std::tuple<
            raise_factor_t<
               factor_for_t<StartingUnits>,
               typename StartingUnits::exponent
            >...
         >;

         using factor = multiply_and_reduce_tuple_t<factors>;
   };


   // ============== This section is to check if units are simplified =================
   // This should definitely be moved elsewhere
   template <typename Tuple>
   struct has_duplicate_dims;

   // empty / singleton: no duplicates
   template <>
   struct has_duplicate_dims<std::tuple<>> : std::false_type {};

   template <typename T>
   struct has_duplicate_dims<std::tuple<T>> : std::false_type {};

   // any_same_dim<H, Ts...> : does any Ts share dimension with H?
   template <typename H, typename... Ts> // Lets try this with assuming H and T are unit exponents
   struct any_same_dim : std::bool_constant<((is_same_dim_v<typename H::unit, typename Ts::unit> && std::is_same_v<typename H::label, typename Ts::label>) || ...)> {};

   template <typename H, typename... Ts>
   struct has_duplicate_dims<std::tuple<H, Ts...>>
      : std::bool_constant< any_same_dim<H, Ts...>::value
                           || has_duplicate_dims<std::tuple<Ts...>>::value > {};

   
   template <typename... Units> // Expect a pack of unit exponents
   concept are_simplified_units =
      !has_duplicate_dims<
         typename unit_decomposition<std::tuple<Units...>>::units // This will be a tuple of unit exponents
      >::value;

   // ================ End simplified units check ==================

   /// @brief Return the internal value as a double in terms of the provided units
   /// @tparam NumTuple tuple of Unit types to convert numerator to
   /// @tparam DenTuple tuple of Unit types to convert denominator to
   /// @return The value in terms of the given units
   template<are_unit_exponents... Units, typename Dim>
   requires (
      dimensionally_equivalent<base_dimension_impl<double, Units...>, Dim> &&
      !same_unit_representation<std::tuple<Units...>, typename Dim::units> &&
      are_simplified_units<Units...>)
   constexpr Dim::rep get_dimension_as(Dim obj)
   {
      // Disallowed if units are not simplified

      // TODO: See if we can repace the call to convert_scalar_units - we should be able to now.

      // Big picture, this needs to extract a double (or rep_type really) from the object.
      // This *necessarily* collapses all ratios, coefficients, exponents, etc, so we don't need to worry about retaining those.
      // We could either collapse before or after conversion. Collapsing after conversion gives one final chance for ratios to simplify.
      // I think this means the logical approach is to cast the type to the destination type, then just extract...
      // But ideally we'd want to keep the coefficients intact, so just base_dimension<Units...> wouldn't be quite right since that would collapse coefficients...

      // Maybe I should have a separate function that's an implementation detail that handles this part - cast to another set of units with max compile-time usage
      // Then, I could just call that function here then extract.

      //using conversions = apply_all_conversions<typename Dim::units, std::tuple<Units...>>;
      //using ratio = typename conversions::ratio;
      //using symbols = typename conversions::symbols;
      //using new_dim = base_dimension_impl<rep, Units..., ratio, symbols>; // not reduced at this point

      //constexpr new_obj = new_dim(obj.get_raw());


      // Finally putting it all together...
      //   We need to assume the requested units are simplified; that's already enforced by concepts
      //   We need to simplify the Dim input type
      //   We can then call apply_all_conversions to get the single factor_t object that represents the conversions needed
      //   Finally, we just need to evaluate the factor and apply that to the dimension.
      //     We could probably have a separate "apply_factor" struct...
      //     Would apply_factor yield a dimension, or a value?
      //     It would make sense to yield the dimension, then have the get_apply_coefficients yield the value when needed..
      //       To do this, I'd need the base_dimension class to handle ratio_exponent... which it really should, but doesn't yet.
      //       I think for now I should get things working by just yielding the value, but make a TODO for this...

      // This block is just to work it out, I can collapse this down.
      //using simplified_units = simplified_units_t<typename Dim::units>;

      // URGENT TODO: We don't just need the simplified units, we need to perform a full simplification operation.
      // Doing so should include the conversion factors used during simplification
      //   Currently, simplification almost certainly does not carry the conversion factors forward.. so more work is needed.

      //using simplified_dim = simplified_dimension_t<Dim>;
      //using conv_factor = typename apply_all_conversions<typename simplified_dim::units, std::tuple<Units...>>::factor; // Get the conversion factor for real conversions
      //constexpr auto conversion = factor::eval_factor<conv_factor, typename Dim::rep>(); // Evaluate the conversion factor to a numeric value

      //constexpr simplified_dim simplified_obj = obj;
      //return simplified_obj.get_apply_coefficients() * conversion;

      // 1) Simplify the source units at the type level
      using src_units  = collapse_units_t<typename Dim::units>;
      using simplify   = some_simplify_operation<src_units>;
      using S_units    = typename simplify::units;   // simplified units of Dim
      using S_factor   = typename simplify::factor;  // factor: (original -> simplified)

      // 2) Factor to go from simplified source units -> requested target units
      using U_factor = typename apply_all_conversions<S_units, std::tuple<Units...>>::factor;

      // 3) Compose in the correct order: (original -> simplified) then (simplified -> target)
      using F_total = multiply_factors_t<U_factor, S_factor>;

      // 4) Evaluate once and scale runtime value
      const auto k = factor::eval_factor<F_total, typename Dim::rep>();
      return obj.get_raw() * k;


      //constexpr double coefficients = ratio_v<typename Dim::ratio> * multiply_symbol_exponent_values_v<typename Dim::symbols>;
      //return convert_scalar_units<std::tuple<Units...>>(obj) * coefficients;
   }
   
   // Matching units
   template<are_unit_exponents... Units, typename Dim>
   requires same_unit_representation<std::tuple<Units...>, typename Dim::units>
   constexpr Dim::rep get_dimension_as(Dim obj)
   {
      return obj.template get_apply_coefficients(); // URGENT TODO: Need to apply coefficients
   }

   template<typename UnitTuple>
   constexpr base_dimension_from_tuple<UnitTuple>::dim::rep get_dimension_tuple(typename base_dimension_from_tuple<UnitTuple>::dim obj)
   {
      return call_unpack<UnitTuple>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
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
      // URGEN TODO: Find all uses of base_dimension_from_tuple and ensure ratio_exponents are handled
      using ratio_exponents = tuple_extract_ratio_exponents_t<std::tuple<Ts...>>;

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

      // This should **NEVER** be called by users. Doing so is considered undefined behavior!
      // All internal calls to this should be HEAVILY scrutinized... TODO
      [[nodiscard]] constexpr Rep get_raw() const
      {
         return scalar;
      }

      [[nodiscard]] constexpr Rep get_apply_coefficients() const
      {
         return static_cast<Rep>(get_raw() *
                                 ratio_v<ratio> * 
                                 multiply_symbol_exponent_values_v<symbols> *
                                 multiply_ratio_exponent_values_v<ratio_exponents>
                              );
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
      using ratio_exponents = divide_ratio_exponent_tuples_t<typename Lhs::ratio_exponents, typename Rhs::ratio_exponents>;

      using coeffs = tuple_cat_t<symbols, ratio_exponents>;

      using units_combined = tuple_cat_t<typename Lhs::units, typename FlipExponents<typename Rhs::units>::units>;
      using units = collapse_units_t<units_combined>;

      return typename base_dimension_from_tuple<Rep, ratio, units, coeffs>::dim(
         lhs.get_raw() /
         rhs.get_raw()
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
      using ratio_exponents = multiply_ratio_exponent_tuples_t<typename Lhs::ratio_exponents, typename Rhs::ratio_exponents>;

      using coeffs = tuple_cat_t<symbols, ratio_exponents>;

      using units_combined = tuple_cat_t<typename Lhs::units, typename Rhs::units>;
      using units = collapse_units_t<units_combined>;
      
      return typename base_dimension_from_tuple<Rep, ratio, units, coeffs>::dim(
         lhs.get_raw() *
         rhs.get_raw()
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
