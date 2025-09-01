#ifndef STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H
#define STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2
   >
   concept are_magnetic_field_units =
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1> && 
               is_charge_unit<T2>
         ) ||
         (
               is_mass_unit<T0> && 
               is_charge_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_mass_unit<T1> && 
               is_charge_unit<T2>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_charge_unit<T1> && 
               is_mass_unit<T2>
         ) ||
         (
               is_charge_unit<T0> && 
               is_mass_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_charge_unit<T0> && 
               is_timespan_unit<T1> && 
               is_mass_unit<T2>
         )
   ;

   /// @brief Concept to verify a type can serve as a named magnetic_field unit
   template<typename T>
   concept IsNamedmagnetic_fieldUnit =
      (std::tuple_size_v<unit_units_t<T>> == 3) &&
      is_mass_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_timespan_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      is_charge_unit<typename std::tuple_element_t<2, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a magnetic_field type
   template<typename T, typename Rep>
   concept is_magnetic_field_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_timespan, -1>, 
      unit_exponent<primary_charge, -1>
   >;

   template<typename T>
   concept is_magnetic_field = is_magnetic_field_as<T, double>;

   /// @brief Retrieves the value of a magnetic_field object with specific units
   /// @tparam massUnit The mass unit used for all mass components of magnetic_field
   /// @tparam timespanUnit The timespan unit used for all timespan components of magnetic_field
   /// @tparam chargeUnit The charge unit used for all charge components of magnetic_field
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      is_magnetic_field DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_magnetic_field_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<timespanUnit, -1>,
         unit_exponent<chargeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named magnetic_field object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedmagnetic_fieldUnit Named, is_magnetic_field DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_magnetic_field_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class magnetic_field;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_magnetic_field_units<
      T0,
      T1,
      T2
   >
   class magnetic_field<T0, T1, T2, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -1>,
      unit_exponent<typename unit_filter<chargeType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -1>,
         unit_exponent<typename unit_filter<chargeType, T0, T1, T2>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_magnetic_field<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr magnetic_field(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_magnetic_field_units<
      T0,
      T1,
      T2
   >
   class magnetic_field<Rep, T0, T1, T2, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -1>,
      unit_exponent<typename unit_filter<chargeType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -1>,
         unit_exponent<typename unit_filter<chargeType, T0, T1, T2>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_magnetic_field<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr magnetic_field(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<IsNamedmagnetic_fieldUnit Named, is_coefficient... Cs>
   class magnetic_field<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_magnetic_field<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr magnetic_field(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<rep_type Rep, IsNamedmagnetic_fieldUnit Named, is_coefficient... Cs>
   class magnetic_field<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_magnetic_field<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr magnetic_field(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_magnetic_field_units<
      T0,
      T1,
      T2
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_magnetic_field(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return magnetic_field<double, T0, T1, T2, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_magnetic_field_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_magnetic_field(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return magnetic_field<Rep, T0, T1, T2, Cs...>(value);
   }

   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<IsNamedmagnetic_fieldUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_magnetic_field(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return magnetic_field<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<IsNamedmagnetic_fieldUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_magnetic_field(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return magnetic_field<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_magnetic_field Dim>
   magnetic_field(Dim) ->
   magnetic_field<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>,
      simplified_unit_filter<chargeType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H