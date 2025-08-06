#ifndef STATIC_DIMENSION_PRESSURE_IMPL_H
#define STATIC_DIMENSION_PRESSURE_IMPL_H

#include "../../base_unit.h"
#include "../../base_dimension_impl.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

#include "../../Dimension_Core/internal_temp/utils.h"


namespace dimension
{

   template<
         typename T0,
         typename T1,
         typename T2
   >
   concept are_pressure_units =
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

   /// @brief Concept to verify a type can serve as a named pressure unit
   template<typename T>
   concept IsNamedpressureUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a pressure type
   template<typename T, typename Rep>
   concept is_pressure_as = dimension_convertible_to<T, Rep,
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, -1>, 
      unit_exponent<primary_timespan, -2>
   >;

   template<typename T>
   concept is_pressure = is_pressure_as<T, double>;

   /// @brief Retrieves the value of a pressure object with specific units
   /// @tparam massUnit The mass unit used for all mass components of pressure
   /// @tparam lengthUnit The length unit used for all length components of pressure
   /// @tparam timespanUnit The timespan unit used for all timespan components of pressure
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_pressure DimType>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr DimType::rep get_pressure_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, -1>,
         unit_exponent<timespanUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named pressure object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as the representative type of DimType
   template<IsNamedpressureUnit Named, is_pressure DimType>
   // TODO: Unit test this and remove suppression
   constexpr DimType::rep get_pressure_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class pressure;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_pressure_units<
      T0,
      T1,
      T2
   >
   class pressure<T0, T1, T2, Cs...> : public base_dimension_impl<double,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, -1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<double,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, -1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_pressure<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr pressure(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_pressure_units<
      T0,
      T1,
      T2
   >
   class pressure<Rep, T0, T1, T2, Cs...> : public base_dimension_impl<Rep,
      unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, -1>,
      unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension_impl<Rep,
         unit_exponent<typename unit_filter<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename unit_filter<lengthType, T0, T1, T2>::type, -1>,
         unit_exponent<typename unit_filter<timespanType, T0, T1, T2>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_pressure<T>
      // cppcheck-suppress noExplicitConstructor
      constexpr pressure(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named pressure units
   /// @tparam Named The named unit this pressure type is in terms of
   template<IsNamedpressureUnit Named, is_coefficient... Cs>
   class pressure<Named, Cs...> : public base_dimension_from_tuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_pressure<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr pressure(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named pressure units
   /// @tparam Named The named unit this pressure type is in terms of
   template<rep_type Rep, IsNamedpressureUnit Named, is_coefficient... Cs>
   class pressure<Rep, Named, Cs...> : public base_dimension_from_tuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimension_from_tuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_pressure<Other>
      // cppcheck-suppress noExplicitConstructor
      constexpr pressure(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_pressure_units<
      T0,
      T1,
      T2
   >
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_pressure(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return pressure<double, T0, T1, T2, Cs...>(1.0);
   }

   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_pressure_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_pressure(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return pressure<Rep, T0, T1, T2, Cs...>(value);
   }

   /// @brief Template specialization for named pressure units
   /// @tparam Named The named unit this pressure type is in terms of
   template<IsNamedpressureUnit Named, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_pressure(Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return pressure<double, Named, Cs...>(1.0);
   }

   /// @brief Template specialization for named pressure units
   /// @tparam Named The named unit this pressure type is in terms of
   template<IsNamedpressureUnit Named, rep_type Rep, is_coefficient... Cs>
   // TODO: Unit test this and remove suppression
   [[maybe_unused]]
   constexpr auto make_pressure(Rep value, Cs... coeffs)
   {
      ignore_unused(coeffs...);
      return pressure<Rep, Named, Cs...>(value);
   }

   template<is_pressure Dim>
   pressure(Dim) -> 
   pressure<
      simplified_unit_filter<massType, typename Dim::units>,
      simplified_unit_filter<lengthType, typename Dim::units>,
      simplified_unit_filter<timespanType, typename Dim::units>
   >;
}

#endif // STATIC_DIMENSION_PRESSURE_IMPL_H