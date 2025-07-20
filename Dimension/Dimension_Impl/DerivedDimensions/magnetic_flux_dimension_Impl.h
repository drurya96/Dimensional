#ifndef STATIC_DIMENSION_MAGNETIC_FLUX_IMPL_H
#define STATIC_DIMENSION_MAGNETIC_FLUX_IMPL_H

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
   concept are_magnetic_flux_units =
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

   /// @brief Concept to verify a type can serve as a named magnetic_flux unit
   template<typename T>
   concept IsNamedmagnetic_fluxUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a magnetic_flux type
   template<typename T>
   concept is_magnetic_flux = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_timespan, -1>, 
      unit_exponent<primary_charge, -1>
   >>;

   /// @brief Retrieves the value of a magnetic_flux object with specific units
   /// @tparam massUnit The mass unit used for all mass components of magnetic_flux
   /// @tparam lengthUnit The length unit used for all length components of magnetic_flux
   /// @tparam timespanUnit The timespan unit used for all timespan components of magnetic_flux
   /// @tparam chargeUnit The charge unit used for all charge components of magnetic_flux
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      is_magnetic_flux DimType>
   constexpr PrecisionType get_magnetic_flux_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -1>,
         unit_exponent<chargeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named magnetic_flux object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmagnetic_fluxUnit Named, is_magnetic_flux DimType>
   constexpr PrecisionType get_magnetic_flux_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class magnetic_flux;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_magnetic_flux_units<
      T0,
      T1,
      T2,
      T3
   >
   class magnetic_flux<T0, T1, T2, T3, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, -1>,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, -1>,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_magnetic_flux<T>
      constexpr magnetic_flux(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_magnetic_flux_units<
      T0,
      T1,
      T2,
      T3
   >
   class magnetic_flux<Rep, T0, T1, T2, T3, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, -1>,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1, T2, T3>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2, T3>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2, T3>::type, -1>,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2, T3>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_magnetic_flux<T>
      constexpr magnetic_flux(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named magnetic_flux units
   /// @tparam Named The named unit this magnetic_flux type is in terms of
   template<IsNamedmagnetic_fluxUnit Named, is_coefficient... Cs>
   class magnetic_flux<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_magnetic_flux<Other>
      constexpr magnetic_flux(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named magnetic_flux units
   /// @tparam Named The named unit this magnetic_flux type is in terms of
   template<rep_type Rep, IsNamedmagnetic_fluxUnit Named, is_coefficient... Cs>
   class magnetic_flux<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_magnetic_flux<Other>
      constexpr magnetic_flux(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      is_coefficient... Cs
   >
   requires are_magnetic_flux_units<
      T0,
      T1,
      T2,
      T3
   >
   constexpr auto make_magnetic_flux(Cs... coeffs)
   {
      return magnetic_flux<double, T0, T1, T2, T3, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      IsBasicUnitType T3,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_magnetic_flux_units<
      T0,
      T1,
      T2,
      T3
   > && (!is_coefficient<Rep>)
   constexpr auto make_magnetic_flux(Rep value, Cs... coeffs)
   {
      return magnetic_flux<Rep, T0, T1, T2, T3, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named magnetic_flux units
   /// @tparam Named The named unit this magnetic_flux type is in terms of
   template<IsNamedmagnetic_fluxUnit Named, is_coefficient... Cs>
   constexpr auto make_magnetic_flux(Cs... coeffs)
   {
      return magnetic_flux<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named magnetic_flux units
   /// @tparam Named The named unit this magnetic_flux type is in terms of
   template<IsNamedmagnetic_fluxUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_magnetic_flux(Rep value, Cs... coeffs)
   {
      return magnetic_flux<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_magnetic_flux Dim>
   magnetic_flux(Dim) -> 
   magnetic_flux<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<chargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MAGNETIC_FLUX_IMPL_H