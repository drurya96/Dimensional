#ifndef STATIC_DIMENSION_HEAT_FLUX_IMPL_H
#define STATIC_DIMENSION_HEAT_FLUX_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_heat_flux_units =
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_mass_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named heat_flux unit
   template<typename T>
   concept IsNamedheat_fluxUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a heat_flux type
   template<typename T>
   concept Isheat_flux = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarytimespan, -3>
   >>;

   /// @brief Retrieves the value of a heat_flux object with specific units
   /// @tparam massUnit The mass unit used for all mass components of heat_flux
   /// @tparam timespanUnit The timespan unit used for all timespan components of heat_flux
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      Isheat_flux DimType>
   constexpr PrecisionType get_heat_flux_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<timespanUnit, -3>
      >(obj);
   }

   /// @brief Retrieves the value of a named heat_flux object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedheat_fluxUnit Named, Isheat_flux DimType>
   constexpr PrecisionType get_heat_flux_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class heat_flux;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_heat_flux_units<
      T0,
      T1
   >
   class heat_flux<T0, T1, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
      unit_exponent<typename Extractor<timespanType, T0, T1>::type, -3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
         unit_exponent<typename Extractor<timespanType, T0, T1>::type, -3>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isheat_flux<T>
      constexpr heat_flux(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_heat_flux_units<
      T0,
      T1
   >
   class heat_flux<Rep, T0, T1, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
      unit_exponent<typename Extractor<timespanType, T0, T1>::type, -3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1>::type, 1>,
         unit_exponent<typename Extractor<timespanType, T0, T1>::type, -3>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isheat_flux<T>
      constexpr heat_flux(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named heat_flux units
   /// @tparam Named The named unit this heat_flux type is in terms of
   template<IsNamedheat_fluxUnit Named, is_coefficient... Cs>
   class heat_flux<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires Isheat_flux<Other>
      constexpr heat_flux(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named heat_flux units
   /// @tparam Named The named unit this heat_flux type is in terms of
   template<rep_type Rep, IsNamedheat_fluxUnit Named, is_coefficient... Cs>
   class heat_flux<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires Isheat_flux<Other>
      constexpr heat_flux(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_heat_flux_units<
      T0,
      T1
   >
   constexpr auto make_heat_flux(Cs... coeffs)
   {
      return heat_flux<double, T0, T1, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_heat_flux_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   constexpr auto make_heat_flux(Rep value, Cs... coeffs)
   {
      return heat_flux<Rep, T0, T1, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named heat_flux units
   /// @tparam Named The named unit this heat_flux type is in terms of
   template<IsNamedheat_fluxUnit Named, is_coefficient... Cs>
   constexpr auto make_heat_flux(Cs... coeffs)
   {
      return heat_flux<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named heat_flux units
   /// @tparam Named The named unit this heat_flux type is in terms of
   template<IsNamedheat_fluxUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_heat_flux(Rep value, Cs... coeffs)
   {
      return heat_flux<Rep, Named, Cs...>(value, coeffs...);
   }









   template<Isheat_flux Dim>
   heat_flux(Dim) -> 
   heat_flux<
      DimExtractor<massType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_HEAT_FLUX_IMPL_H