#ifndef STATIC_DIMENSION_DYNAMIC_VISCOSITY_IMPL_H
#define STATIC_DIMENSION_DYNAMIC_VISCOSITY_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2
   >
   concept are_dynamic_viscosity_units =
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1> && 
               is_length_unit<T2>
         ) ||
         (
               is_mass_unit<T0> && 
               is_length_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_mass_unit<T1> && 
               is_length_unit<T2>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_length_unit<T1> && 
               is_mass_unit<T2>
         ) ||
         (
               is_length_unit<T0> && 
               is_mass_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1> && 
               is_mass_unit<T2>
         )
   ;

   /// @brief Concept to verify a type can serve as a named dynamic_viscosity unit
   template<typename T>
   concept IsNameddynamic_viscosityUnit =
      (std::tuple_size_v<unit_units_t<T>> == 3) &&
      is_mass_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_timespan_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      is_length_unit<typename std::tuple_element_t<2, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a dynamic_viscosity type
   template<typename T, typename Rep>
   concept is_dynamic_viscosity_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_timespan, -1>, 
      unit_exponent<primary_length, -1>
   >;

   template<typename T>
   concept is_dynamic_viscosity = is_dynamic_viscosity_as<T, double>;

   /// @brief Retrieves the value of a dynamic_viscosity object with specific units
   /// @tparam massUnit The mass unit used for all mass components of dynamic_viscosity
   /// @tparam timespanUnit The timespan unit used for all timespan components of dynamic_viscosity
   /// @tparam lengthUnit The length unit used for all length components of dynamic_viscosity
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      is_length_unit lengthUnit,
      is_dynamic_viscosity DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_dynamic_viscosity_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<timespanUnit, -1>,
         unit_exponent<lengthUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named dynamic_viscosity object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNameddynamic_viscosityUnit Named, is_dynamic_viscosity DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_dynamic_viscosity_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class dynamic_viscosity;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_dynamic_viscosity_units<
      T0,
      T1,
      T2
   >
   class dynamic_viscosity<T0, T1, T2, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_dynamic_viscosity<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr dynamic_viscosity(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_dynamic_viscosity_units<
      T0,
      T1,
      T2
   >
   class dynamic_viscosity<Rep, T0, T1, T2, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_dynamic_viscosity<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr dynamic_viscosity(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named dynamic_viscosity units
   /// @tparam Named The named unit this dynamic_viscosity type is in terms of
   template<IsNameddynamic_viscosityUnit Named, is_coefficient... Cs>
   class dynamic_viscosity<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_dynamic_viscosity<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr dynamic_viscosity(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named dynamic_viscosity units
   /// @tparam Named The named unit this dynamic_viscosity type is in terms of
   template<rep_type Rep, IsNameddynamic_viscosityUnit Named, is_coefficient... Cs>
   class dynamic_viscosity<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_dynamic_viscosity<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr dynamic_viscosity(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_dynamic_viscosity_units<
      T0,
      T1,
      T2
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_dynamic_viscosity(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return dynamic_viscosity<double, T0, T1, T2, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_dynamic_viscosity_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_dynamic_viscosity(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return dynamic_viscosity<Rep, T0, T1, T2, Cs...>(value);
   }

   /// @brief Template specialization for named dynamic_viscosity units
   /// @tparam Named The named unit this dynamic_viscosity type is in terms of
   template<IsNameddynamic_viscosityUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_dynamic_viscosity(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return dynamic_viscosity<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named dynamic_viscosity units
   /// @tparam Named The named unit this dynamic_viscosity type is in terms of
   template<IsNameddynamic_viscosityUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_dynamic_viscosity(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return dynamic_viscosity<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_dynamic_viscosity Dim>
   dynamic_viscosity(Dim) ->
   dynamic_viscosity<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>,
      simplified_unit_filter<lengthType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_DYNAMIC_VISCOSITY_IMPL_H