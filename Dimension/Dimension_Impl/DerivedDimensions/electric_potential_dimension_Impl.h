#ifndef STATIC_DIMENSION_ELECTRIC_POTENTIAL_IMPL_H
#define STATIC_DIMENSION_ELECTRIC_POTENTIAL_IMPL_H

#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "../../Dimension_Core/internal_temp/dimension/base_dimension_from_tuple.h"

#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2,
         typename T3
   >
   concept are_electric_potential_units =
         (
               is_mass_unit<T0> && 
               is_length_unit<T1> && 
               is_timespan_unit<T2> && 
               is_charge_unit<T3>
         ) ||
         (
               is_mass_unit<T0> && 
               is_length_unit<T1> && 
               is_charge_unit<T2> && 
               is_timespan_unit<T3>
         ) ||
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1> && 
               is_length_unit<T2> && 
               is_charge_unit<T3>
         ) ||
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1> && 
               is_charge_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_mass_unit<T0> && 
               is_charge_unit<T1> && 
               is_length_unit<T2> && 
               is_timespan_unit<T3>
         ) ||
         (
               is_mass_unit<T0> && 
               is_charge_unit<T1> && 
               is_timespan_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_length_unit<T0> && 
               is_mass_unit<T1> && 
               is_timespan_unit<T2> && 
               is_charge_unit<T3>
         ) ||
         (
               is_length_unit<T0> && 
               is_mass_unit<T1> && 
               is_charge_unit<T2> && 
               is_timespan_unit<T3>
         ) ||
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1> && 
               is_mass_unit<T2> && 
               is_charge_unit<T3>
         ) ||
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1> && 
               is_charge_unit<T2> && 
               is_mass_unit<T3>
         ) ||
         (
               is_length_unit<T0> && 
               is_charge_unit<T1> && 
               is_mass_unit<T2> && 
               is_timespan_unit<T3>
         ) ||
         (
               is_length_unit<T0> && 
               is_charge_unit<T1> && 
               is_timespan_unit<T2> && 
               is_mass_unit<T3>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_mass_unit<T1> && 
               is_length_unit<T2> && 
               is_charge_unit<T3>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_mass_unit<T1> && 
               is_charge_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_length_unit<T1> && 
               is_mass_unit<T2> && 
               is_charge_unit<T3>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_length_unit<T1> && 
               is_charge_unit<T2> && 
               is_mass_unit<T3>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_charge_unit<T1> && 
               is_mass_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_charge_unit<T1> && 
               is_length_unit<T2> && 
               is_mass_unit<T3>
         ) ||
         (
               is_charge_unit<T0> && 
               is_mass_unit<T1> && 
               is_length_unit<T2> && 
               is_timespan_unit<T3>
         ) ||
         (
               is_charge_unit<T0> && 
               is_mass_unit<T1> && 
               is_timespan_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_charge_unit<T0> && 
               is_length_unit<T1> && 
               is_mass_unit<T2> && 
               is_timespan_unit<T3>
         ) ||
         (
               is_charge_unit<T0> && 
               is_length_unit<T1> && 
               is_timespan_unit<T2> && 
               is_mass_unit<T3>
         ) ||
         (
               is_charge_unit<T0> && 
               is_timespan_unit<T1> && 
               is_mass_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_charge_unit<T0> && 
               is_timespan_unit<T1> && 
               is_length_unit<T2> && 
               is_mass_unit<T3>
         )
   ;

   /// @brief Concept to verify a type can serve as a named electric_potential unit
   template<typename T>
   concept IsNamedelectric_potentialUnit =
      (std::tuple_size_v<unit_units_t<T>> == 4) &&
      is_mass_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_length_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      is_timespan_unit<typename std::tuple_element_t<2, unit_units_t<T>>::unit> &&
      is_charge_unit<typename std::tuple_element_t<3, unit_units_t<T>>::unit> &&
      !is_fundamental_unit_v<T>;

   /// @brief Concept to verify a dimension can be treated as a electric_potential type
   template<typename T, typename Rep>
   concept is_electric_potential_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_timespan, -2>, 
      unit_exponent<primary_charge, -1>
   >;

   template<typename T>
   concept is_electric_potential = is_electric_potential_as<T, double>;

   /// @brief Retrieves the value of a electric_potential object with specific units
   /// @tparam massUnit The mass unit used for all mass components of electric_potential
   /// @tparam lengthUnit The length unit used for all length components of electric_potential
   /// @tparam timespanUnit The timespan unit used for all timespan components of electric_potential
   /// @tparam chargeUnit The charge unit used for all charge components of electric_potential
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      is_electric_potential DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr typename DimType::rep get_electric_potential_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -2>,
         unit_exponent<chargeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named electric_potential object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedelectric_potentialUnit Named, is_electric_potential DimType>
   // TODO: Unit test this and remove suppression
   constexpr typename DimType::rep get_electric_potential_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class electric_potential;

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_electric_potential_units<
      T0,
      T1,
      T2,
      T3
   >
   class electric_potential<T0, T1, T2, T3, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1, T2, T3>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2, T3>::type, -2>,
      unit_exponent<typename unit_filter<chargeType, T0, T1, T2, T3>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1, T2, T3>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2, T3>::type, -2>,
         unit_exponent<typename unit_filter<chargeType, T0, T1, T2, T3>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_electric_potential<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr electric_potential(const T& base) : Base(base) {}
   };

   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_electric_potential_units<
      T0,
      T1,
      T2,
      T3
   >
   class electric_potential<Rep, T0, T1, T2, T3, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1, T2, T3>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2, T3>::type, -2>,
      unit_exponent<typename unit_filter<chargeType, T0, T1, T2, T3>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1, T2, T3>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2, T3>::type, -2>,
         unit_exponent<typename unit_filter<chargeType, T0, T1, T2, T3>::type, -1>,
         Cs...
      >;

      using Base::Base;

      template<typename T>
      requires is_electric_potential<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr electric_potential(const T& base) : Base(base) {}
   };

   /// @brief Template specialization for named electric_potential units
   /// @tparam Named The named unit this electric_potential type is in terms of
   template<IsNamedelectric_potentialUnit Named, is_coefficient... Cs>
   class electric_potential<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_electric_potential<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr electric_potential(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named electric_potential units
   /// @tparam Named The named unit this electric_potential type is in terms of
   template<rep_type Rep, IsNamedelectric_potentialUnit Named, is_coefficient... Cs>
   class electric_potential<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_electric_potential<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr electric_potential(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   // ───────────────────── factory helpers (make_*) ─────────────────────
   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_electric_potential_units<
      T0,
      T1,
      T2,
      T3
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_electric_potential(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return electric_potential<double, T0, T1, T2, T3, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_electric_potential_units<
      T0,
      T1,
      T2,
      T3
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_electric_potential(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return electric_potential<Rep, T0, T1, T2, T3, Cs...>(value);
   }

   /// @brief Template specialization for named electric_potential units
   /// @tparam Named The named unit this electric_potential type is in terms of
   template<IsNamedelectric_potentialUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_electric_potential(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return electric_potential<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named electric_potential units
   /// @tparam Named The named unit this electric_potential type is in terms of
   template<IsNamedelectric_potentialUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_electric_potential(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return electric_potential<Rep, Named, Cs...>(value);
   }

   // deduction guide
   template<is_electric_potential Dim>
   electric_potential(Dim) ->
   electric_potential<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<lengthType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>,
      simplified_unit_filter<chargeType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_ELECTRIC_POTENTIAL_IMPL_H