#ifndef STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H
#define STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H

#include "../../base_unit.h"
#include "../../base_dimension_impl.h"

#include "../../Dimension_Core/internal_temp/units/new_unit_stuff.h"

#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


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
   concept IsNamedmass_flow_rateUnit =
      (std::tuple_size_v<unit_units_t<T>> == 2) &&
      is_mass_unit<typename std::tuple_element_t<0, unit_units_t<T>>::unit> &&
      is_timespan_unit<typename std::tuple_element_t<1, unit_units_t<T>>::unit> &&
      (!std::is_base_of_v<FundamentalUnitTag, T>);

   /// @brief Concept to verify a dimension can be treated as a mass_flow_rate type
   template<typename T, typename Rep>
   concept is_mass_flow_rate_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_timespan, -1>
   >;

   template<typename T>
   concept is_mass_flow_rate = is_mass_flow_rate_as<T, double>;

   /// @brief Retrieves the value of a mass_flow_rate object with specific units
   /// @tparam massUnit The mass unit used for all mass components of mass_flow_rate
   /// @tparam timespanUnit The timespan unit used for all timespan components of mass_flow_rate
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      is_mass_flow_rate DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr DimType::rep get_mass_flow_rate_as(const DimType& obj)
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
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedmass_flow_rateUnit Named, is_mass_flow_rate DimType>
   // TODO: Unit test this and remove suppression
   constexpr DimType::rep get_mass_flow_rate_as(const DimType& obj)
   {
      return call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
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
   class mass_flow_rate<T0, T1, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_mass_flow_rate<T>
      // cppcheck-suppress noExplicitConstructor
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
   class mass_flow_rate<Rep, T0, T1, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1>::type, 1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_mass_flow_rate<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr mass_flow_rate(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<IsNamedmass_flow_rateUnit Named, is_coefficient... Cs>
   class mass_flow_rate<Named, Cs...> : public base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_mass_flow_rate<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr mass_flow_rate(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<rep_type Rep, IsNamedmass_flow_rateUnit Named, is_coefficient... Cs>
   class mass_flow_rate<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, unit_units_t<Named>, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_mass_flow_rate<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr mass_flow_rate(const Other& base)
         : Base(call_unpack<unit_units_t<Named>>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
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
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_mass_flow_rate(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return mass_flow_rate<double, T0, T1, Cs...>(1.0);
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
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_mass_flow_rate(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return mass_flow_rate<Rep, T0, T1, Cs...>(value);
   }

   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<IsNamedmass_flow_rateUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_mass_flow_rate(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return mass_flow_rate<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<IsNamedmass_flow_rateUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_mass_flow_rate(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return mass_flow_rate<Rep, Named, Cs...>(value);
   }

   template<is_mass_flow_rate Dim>
   mass_flow_rate(Dim) -> 
   mass_flow_rate<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H