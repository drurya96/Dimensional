#ifndef STATIC_DIMENSION_SPECIFIC_HEAT_CAPACITY_IMPL_H
#define STATIC_DIMENSION_SPECIFIC_HEAT_CAPACITY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/temperature_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2
   >
   concept are_specific_heat_capacity_units =
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1> && 
               is_temperature_unit<T2>
         ) ||
         (
               is_length_unit<T0> && 
               is_temperature_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_length_unit<T1> && 
               is_temperature_unit<T2>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_temperature_unit<T1> && 
               is_length_unit<T2>
         ) ||
         (
               is_temperature_unit<T0> && 
               is_length_unit<T1> && 
               is_timespan_unit<T2>
         ) ||
         (
               is_temperature_unit<T0> && 
               is_timespan_unit<T1> && 
               is_length_unit<T2>
         )
   ;

   /// @brief Concept to verify a type can serve as a named specific_heat_capacity unit
   template<typename T>
   concept IsNamedspecific_heat_capacityUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_temperature_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a specific_heat_capacity type
   template<typename T>
   concept is_specific_heat_capacity = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_timespan, -2>, 
      unit_exponent<primary_temperature, -1>
   >>;

   /// @brief Retrieves the value of a specific_heat_capacity object with specific units
   /// @tparam lengthUnit The length unit used for all length components of specific_heat_capacity
   /// @tparam timespanUnit The timespan unit used for all timespan components of specific_heat_capacity
   /// @tparam temperatureUnit The temperature unit used for all temperature components of specific_heat_capacity
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_temperature_unit temperatureUnit,
      is_specific_heat_capacity DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr PrecisionType get_specific_heat_capacity_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -2>,
         unit_exponent<temperatureUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named specific_heat_capacity object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedspecific_heat_capacityUnit Named, is_specific_heat_capacity DimType>
   // TODO: Unit test this and remove suppression
   constexpr PrecisionType get_specific_heat_capacity_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class specific_heat_capacity;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_specific_heat_capacity_units<
      T0,
      T1,
      T2
   >
   class specific_heat_capacity<T0, T1, T2, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -2>,
      unit_exponent<typename Extractor<temperatureType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -2>,
         unit_exponent<typename Extractor<temperatureType, T0, T1, T2>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_specific_heat_capacity<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr specific_heat_capacity(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_specific_heat_capacity_units<
      T0,
      T1,
      T2
   >
   class specific_heat_capacity<Rep, T0, T1, T2, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -2>,
      unit_exponent<typename Extractor<temperatureType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -2>,
         unit_exponent<typename Extractor<temperatureType, T0, T1, T2>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_specific_heat_capacity<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr specific_heat_capacity(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named specific_heat_capacity units
   /// @tparam Named The named unit this specific_heat_capacity type is in terms of
   template<IsNamedspecific_heat_capacityUnit Named, is_coefficient... Cs>
   class specific_heat_capacity<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_specific_heat_capacity<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr specific_heat_capacity(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named specific_heat_capacity units
   /// @tparam Named The named unit this specific_heat_capacity type is in terms of
   template<rep_type Rep, IsNamedspecific_heat_capacityUnit Named, is_coefficient... Cs>
   class specific_heat_capacity<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_specific_heat_capacity<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr specific_heat_capacity(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_specific_heat_capacity_units<
      T0,
      T1,
      T2
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_specific_heat_capacity(Cs... coeffs)
   {
      return specific_heat_capacity<double, T0, T1, T2, Cs...>(1.0, coeffs...);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_specific_heat_capacity_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_specific_heat_capacity(Rep value, Cs... coeffs)
   {
      return specific_heat_capacity<Rep, T0, T1, T2, Cs...>(value, coeffs...);
   }

   /// @brief Template specialization for named specific_heat_capacity units
   /// @tparam Named The named unit this specific_heat_capacity type is in terms of
   template<IsNamedspecific_heat_capacityUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_specific_heat_capacity(Cs... coeffs)
   {
      return specific_heat_capacity<double, Named, Cs...>(1.0, coeffs...);
   }

   /// @brief Template specialization for named specific_heat_capacity units
   /// @tparam Named The named unit this specific_heat_capacity type is in terms of
   template<IsNamedspecific_heat_capacityUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_specific_heat_capacity(Rep value, Cs... coeffs)
   {
      return specific_heat_capacity<Rep, Named, Cs...>(value, coeffs...);
   }

   template<is_specific_heat_capacity Dim>
   specific_heat_capacity(Dim) -> 
   specific_heat_capacity<
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<temperatureType, Dim>
   >;
}

#endif // STATIC_DIMENSION_SPECIFIC_HEAT_CAPACITY_IMPL_H