#ifndef STATIC_DIMENSION_RESISTANCE_IMPL_H
#define STATIC_DIMENSION_RESISTANCE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2,
         typename T3
   >
   concept are_resistance_units =
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

   /// @brief Concept to verify a type can serve as a named resistance unit
   template<typename T>
   concept IsNamedresistanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a resistance type
   template<typename T>
   concept is_resistance = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_timespan, -1>, 
      unit_exponent<primary_charge, -2>
   >>;

   /// @brief Retrieves the value of a resistance object with specific units
   /// @tparam massUnit The mass unit used for all mass components of resistance
   /// @tparam lengthUnit The length unit used for all length components of resistance
   /// @tparam timespanUnit The timespan unit used for all timespan components of resistance
   /// @tparam chargeUnit The charge unit used for all charge components of resistance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      is_resistance DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr PrecisionType get_resistance_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -1>,
         unit_exponent<chargeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named resistance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedresistanceUnit Named, is_resistance DimType>
   // TODO: Unit test this and remove suppression
   constexpr PrecisionType get_resistance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class resistance;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_resistance_units<
      T0,
      T1,
      T2,
      T3
   >
   class resistance<T0, T1, T2, T3, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, -1>,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, -1>,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_resistance<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr resistance(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_resistance_units<
      T0,
      T1,
      T2,
      T3
   >
   class resistance<Rep, T0, T1, T2, T3, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, -1>,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, -1>,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_resistance<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr resistance(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named resistance units
   /// @tparam Named The named unit this resistance type is in terms of
   template<IsNamedresistanceUnit Named, is_coefficient... Cs>
   class resistance<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_resistance<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr resistance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named resistance units
   /// @tparam Named The named unit this resistance type is in terms of
   template<rep_type Rep, IsNamedresistanceUnit Named, is_coefficient... Cs>
   class resistance<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_resistance<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr resistance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_resistance_units<
      T0,
      T1,
      T2,
      T3
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_resistance(Cs... coeffs)
   {
      return resistance<double, T0, T1, T2, T3, Cs...>(1.0, coeffs...);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_resistance_units<
      T0,
      T1,
      T2,
      T3
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_resistance(Rep value, Cs... coeffs)
   {
      return resistance<Rep, T0, T1, T2, T3, Cs...>(value, coeffs...);
   }

   /// @brief Template specialization for named resistance units
   /// @tparam Named The named unit this resistance type is in terms of
   template<IsNamedresistanceUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_resistance(Cs... coeffs)
   {
      return resistance<double, Named, Cs...>(1.0, coeffs...);
   }

   /// @brief Template specialization for named resistance units
   /// @tparam Named The named unit this resistance type is in terms of
   template<IsNamedresistanceUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_resistance(Rep value, Cs... coeffs)
   {
      return resistance<Rep, Named, Cs...>(value, coeffs...);
   }

   template<is_resistance Dim>
   resistance(Dim) -> 
   resistance<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<chargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_RESISTANCE_IMPL_H