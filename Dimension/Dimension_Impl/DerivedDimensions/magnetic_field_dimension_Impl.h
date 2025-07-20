#ifndef STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H
#define STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"


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
   concept IsNamedmagnetic_fieldUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a magnetic_field type
   template<typename T>
   concept is_magnetic_field = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_timespan, -1>, 
      unit_exponent<primary_charge, -1>
   >>;

   /// @brief Retrieves the value of a magnetic_field object with specific units
   /// @tparam massUnit The mass unit used for all mass components of magnetic_field
   /// @tparam timespanUnit The timespan unit used for all timespan components of magnetic_field
   /// @tparam chargeUnit The charge unit used for all charge components of magnetic_field
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      is_magnetic_field DimType>
   constexpr PrecisionType get_magnetic_field_as(const DimType& obj)
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
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmagnetic_fieldUnit Named, is_magnetic_field DimType>
   constexpr PrecisionType get_magnetic_field_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
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
   class magnetic_field<T0, T1, T2, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -1>,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -1>,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_magnetic_field<T>
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
   class magnetic_field<Rep, T0, T1, T2, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -1>,
      unit_exponent<typename Extractor<chargeType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -1>,
         unit_exponent<typename Extractor<chargeType, T0, T1, T2>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_magnetic_field<T>
      constexpr magnetic_field(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<IsNamedmagnetic_fieldUnit Named, is_coefficient... Cs>
   class magnetic_field<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_magnetic_field<Other>
      constexpr magnetic_field(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<rep_type Rep, IsNamedmagnetic_fieldUnit Named, is_coefficient... Cs>
   class magnetic_field<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_magnetic_field<Other>
      constexpr magnetic_field(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




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
   constexpr auto make_magnetic_field(Cs... coeffs)
   {
      return magnetic_field<double, T0, T1, T2, Cs...>(1.0, coeffs...);
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
   constexpr auto make_magnetic_field(Rep value, Cs... coeffs)
   {
      return magnetic_field<Rep, T0, T1, T2, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<IsNamedmagnetic_fieldUnit Named, is_coefficient... Cs>
   constexpr auto make_magnetic_field(Cs... coeffs)
   {
      return magnetic_field<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<IsNamedmagnetic_fieldUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_magnetic_field(Rep value, Cs... coeffs)
   {
      return magnetic_field<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_magnetic_field Dim>
   magnetic_field(Dim) -> 
   magnetic_field<
      DimExtractor<massType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<chargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H