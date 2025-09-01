#ifndef STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H
#define STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_diffusion_coefficient_units =
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_length_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named diffusion_coefficient unit
   template<typename T>
   concept IsNameddiffusion_coefficientUnit =
      (std::tuple_size_v<unit_units_t<T>> == 2) &&
      is_length_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_timespan_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a diffusion_coefficient type
   template<typename T, typename Rep>
   concept is_diffusion_coefficient_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_timespan, -1>
   >;

   template<typename T>
   concept is_diffusion_coefficient = is_diffusion_coefficient_as<T, double>;

   /// @brief Retrieves the value of a diffusion_coefficient object with specific units
   /// @tparam lengthUnit The length unit used for all length components of diffusion_coefficient
   /// @tparam timespanUnit The timespan unit used for all timespan components of diffusion_coefficient
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_diffusion_coefficient DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_diffusion_coefficient_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named diffusion_coefficient object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNameddiffusion_coefficientUnit Named, is_diffusion_coefficient DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_diffusion_coefficient_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class diffusion_coefficient;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_diffusion_coefficient_units<
      T0,
      T1
   >
   class diffusion_coefficient<T0, T1, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 2>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 2>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_diffusion_coefficient<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr diffusion_coefficient(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_diffusion_coefficient_units<
      T0,
      T1
   >
   class diffusion_coefficient<Rep, T0, T1, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 2>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<lengthType, T0, T1>::type, 2>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_diffusion_coefficient<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr diffusion_coefficient(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<IsNameddiffusion_coefficientUnit Named, is_coefficient... Cs>
   class diffusion_coefficient<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_diffusion_coefficient<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr diffusion_coefficient(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<rep_type Rep, IsNameddiffusion_coefficientUnit Named, is_coefficient... Cs>
   class diffusion_coefficient<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_diffusion_coefficient<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr diffusion_coefficient(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_diffusion_coefficient_units<
      T0,
      T1
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_diffusion_coefficient(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return diffusion_coefficient<double, T0, T1, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_diffusion_coefficient_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_diffusion_coefficient(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return diffusion_coefficient<Rep, T0, T1, Cs...>(value);
   }

   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<IsNameddiffusion_coefficientUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_diffusion_coefficient(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return diffusion_coefficient<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<IsNameddiffusion_coefficientUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_diffusion_coefficient(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return diffusion_coefficient<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_diffusion_coefficient Dim>
   diffusion_coefficient(Dim) ->
   diffusion_coefficient<
      simplified_unit_filter<lengthType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H