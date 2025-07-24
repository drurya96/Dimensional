#ifndef STATIC_DIMENSION_CAPACITANCE_IMPL_H
#define STATIC_DIMENSION_CAPACITANCE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2,
         typename T3
   >
   concept are_capacitance_units =
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
         ) ||
         (
               is_charge_unit<T0> && 
               is_mass_unit<T1> && 
               is_timespan_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_charge_unit<T0> && 
               is_mass_unit<T1> && 
               is_length_unit<T2> && 
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
               is_length_unit<T1> && 
               is_mass_unit<T2> && 
               is_timespan_unit<T3>
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
               is_timespan_unit<T0> && 
               is_mass_unit<T1> && 
               is_charge_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_mass_unit<T1> && 
               is_length_unit<T2> && 
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
               is_length_unit<T1> && 
               is_mass_unit<T2> && 
               is_charge_unit<T3>
         ) ||
         (
               is_mass_unit<T0> && 
               is_charge_unit<T1> && 
               is_timespan_unit<T2> && 
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
               is_timespan_unit<T1> && 
               is_charge_unit<T2> && 
               is_length_unit<T3>
         ) ||
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1> && 
               is_length_unit<T2> && 
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
               is_length_unit<T1> && 
               is_timespan_unit<T2> && 
               is_charge_unit<T3>
         ) ||
         (
               is_length_unit<T0> && 
               is_charge_unit<T1> && 
               is_timespan_unit<T2> && 
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
               is_timespan_unit<T1> && 
               is_charge_unit<T2> && 
               is_mass_unit<T3>
         ) ||
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1> && 
               is_mass_unit<T2> && 
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
               is_mass_unit<T1> && 
               is_timespan_unit<T2> && 
               is_charge_unit<T3>
         )
   ;

   /// @brief Concept to verify a type can serve as a named capacitance unit
   template<typename T>
   concept IsNamedcapacitanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         is_charge_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_mass_unit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a capacitance type
   template<typename T>
   concept is_capacitance = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_charge, 2>, 
      unit_exponent<primary_timespan, 2>, 
      unit_exponent<primary_mass, -1>, 
      unit_exponent<primary_length, -2>
   >>;

   /// @brief Retrieves the value of a capacitance object with specific units
   /// @tparam chargeUnit The charge unit used for all charge components of capacitance
   /// @tparam timespanUnit The timespan unit used for all timespan components of capacitance
   /// @tparam massUnit The mass unit used for all mass components of capacitance
   /// @tparam lengthUnit The length unit used for all length components of capacitance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_charge_unit chargeUnit,
      is_timespan_unit timespanUnit,
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_capacitance DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr PrecisionType get_capacitance_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<chargeUnit, 2>,
         unit_exponent<timespanUnit, 2>,
         unit_exponent<massUnit, -1>,
         unit_exponent<lengthUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named capacitance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedcapacitanceUnit Named, is_capacitance DimType>
   // TODO: Unit test this and remove suppression
   constexpr PrecisionType get_capacitance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class capacitance;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_capacitance_units<
      T0,
      T1,
      T2,
      T3
   >
   class capacitance<T0, T1, T2, T3, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, -1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, -1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_capacitance<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr capacitance(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_capacitance_units<
      T0,
      T1,
      T2,
      T3
   >
   class capacitance<Rep, T0, T1, T2, T3, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, -1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, -1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_capacitance<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr capacitance(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named capacitance units
   /// @tparam Named The named unit this capacitance type is in terms of
   template<IsNamedcapacitanceUnit Named, is_coefficient... Cs>
   class capacitance<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_capacitance<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr capacitance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named capacitance units
   /// @tparam Named The named unit this capacitance type is in terms of
   template<rep_type Rep, IsNamedcapacitanceUnit Named, is_coefficient... Cs>
   class capacitance<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_capacitance<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr capacitance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_capacitance_units<
      T0,
      T1,
      T2,
      T3
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_capacitance(Cs... coeffs)
   {
      return capacitance<double, T0, T1, T2, T3, Cs...>(1.0, coeffs...);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_capacitance_units<
      T0,
      T1,
      T2,
      T3
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_capacitance(Rep value, Cs... coeffs)
   {
      return capacitance<Rep, T0, T1, T2, T3, Cs...>(value, coeffs...);
   }

   /// @brief Template specialization for named capacitance units
   /// @tparam Named The named unit this capacitance type is in terms of
   template<IsNamedcapacitanceUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_capacitance(Cs... coeffs)
   {
      return capacitance<double, Named, Cs...>(1.0, coeffs...);
   }

   /// @brief Template specialization for named capacitance units
   /// @tparam Named The named unit this capacitance type is in terms of
   template<IsNamedcapacitanceUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_capacitance(Rep value, Cs... coeffs)
   {
      return capacitance<Rep, Named, Cs...>(value, coeffs...);
   }

   template<is_capacitance Dim>
   capacitance(Dim) -> 
   capacitance<
      DimExtractor<chargeType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_CAPACITANCE_IMPL_H