#ifndef STATIC_DIMENSION_POWER_IMPL_H
#define STATIC_DIMENSION_POWER_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2
   >
   concept are_power_units =
         (
               is_mass_unit<T0> && 
               is_length_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_mass_unit<T0> && 
               is_timespan_unit<T1> && 
               is_length_unit<T2>
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
         )
   ;

   /// @brief Concept to verify a type can serve as a named power unit
   template<typename T>
   concept IsNamedpowerUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a power type
   template<typename T>
   concept Ispower = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, 2>, 
      unit_exponent<Primarytimespan, -3>
   >>;

   /// @brief Retrieves the value of a power object with specific units
   /// @tparam massUnit The mass unit used for all mass components of power
   /// @tparam lengthUnit The length unit used for all length components of power
   /// @tparam timespanUnit The timespan unit used for all timespan components of power
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      Ispower DimType>
   constexpr PrecisionType get_power_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -3>
      >(obj);
   }

   /// @brief Retrieves the value of a named power object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedpowerUnit Named, Ispower DimType>
   constexpr PrecisionType get_power_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class power;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_power_units<
      T0,
      T1,
      T2
   >
   class power<T0, T1, T2, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -3>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Ispower<T>
      constexpr power(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_power_units<
      T0,
      T1,
      T2
   >
   class power<Rep, T0, T1, T2, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -3>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -3>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Ispower<T>
      constexpr power(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named power units
   /// @tparam Named The named unit this power type is in terms of
   template<IsNamedpowerUnit Named, is_coefficient... Cs>
   class power<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires Ispower<Other>
      constexpr power(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named power units
   /// @tparam Named The named unit this power type is in terms of
   template<rep_type Rep, IsNamedpowerUnit Named, is_coefficient... Cs>
   class power<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires Ispower<Other>
      constexpr power(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_power_units<
      T0,
      T1,
      T2
   >
   constexpr auto make_power(Cs... coeffs)
   {
      return power<double, T0, T1, T2, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_power_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   constexpr auto make_power(Rep value, Cs... coeffs)
   {
      return power<Rep, T0, T1, T2, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named power units
   /// @tparam Named The named unit this power type is in terms of
   template<IsNamedpowerUnit Named, is_coefficient... Cs>
   constexpr auto make_power(Cs... coeffs)
   {
      return power<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named power units
   /// @tparam Named The named unit this power type is in terms of
   template<IsNamedpowerUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_power(Rep value, Cs... coeffs)
   {
      return power<Rep, Named, Cs...>(value, coeffs...);
   }









   template<Ispower Dim>
   power(Dim) -> 
   power<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_POWER_IMPL_H