#ifndef STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H
#define STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_mass_flow_rate_units =
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_mass_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named mass_flow_rate unit
   template<typename T>
   concept IsNamedmass_flow_rateUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a mass_flow_rate type
   template<typename T>
   concept is_mass_flow_rate = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_timespan, -1>
   >>;

   /// @brief Retrieves the value of a mass_flow_rate object with specific units
   /// @tparam massUnit The mass unit used for all mass components of mass_flow_rate
   /// @tparam timespanUnit The timespan unit used for all timespan components of mass_flow_rate
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      is_mass_flow_rate DimType>
   constexpr PrecisionType get_mass_flow_rate_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named mass_flow_rate object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmass_flow_rateUnit Named, is_mass_flow_rate DimType>
   constexpr PrecisionType get_mass_flow_rate_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class mass_flow_rate;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_mass_flow_rate_units<
      T0,
      T1
   >
   class mass_flow_rate<T0, T1, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
      unit_exponent<typename Extractor<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
         unit_exponent<typename Extractor<timespanType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_mass_flow_rate<T>
      constexpr mass_flow_rate(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_mass_flow_rate_units<
      T0,
      T1
   >
   class mass_flow_rate<Rep, T0, T1, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
      unit_exponent<typename Extractor<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
         unit_exponent<typename Extractor<timespanType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_mass_flow_rate<T>
      constexpr mass_flow_rate(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<IsNamedmass_flow_rateUnit Named, is_coefficient... Cs>
   class mass_flow_rate<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_mass_flow_rate<Other>
      constexpr mass_flow_rate(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<rep_type Rep, IsNamedmass_flow_rateUnit Named, is_coefficient... Cs>
   class mass_flow_rate<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_mass_flow_rate<Other>
      constexpr mass_flow_rate(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_mass_flow_rate_units<
      T0,
      T1
   >
   constexpr auto make_mass_flow_rate(Cs... coeffs)
   {
      return mass_flow_rate<double, T0, T1, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_mass_flow_rate_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   constexpr auto make_mass_flow_rate(Rep value, Cs... coeffs)
   {
      return mass_flow_rate<Rep, T0, T1, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<IsNamedmass_flow_rateUnit Named, is_coefficient... Cs>
   constexpr auto make_mass_flow_rate(Cs... coeffs)
   {
      return mass_flow_rate<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<IsNamedmass_flow_rateUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_mass_flow_rate(Rep value, Cs... coeffs)
   {
      return mass_flow_rate<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_mass_flow_rate Dim>
   mass_flow_rate(Dim) -> 
   mass_flow_rate<
      DimExtractor<massType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H