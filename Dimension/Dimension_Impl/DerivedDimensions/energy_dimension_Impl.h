#ifndef STATIC_DIMENSION_ENERGY_IMPL_H
#define STATIC_DIMENSION_ENERGY_IMPL_H

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
   concept are_energy_units =
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

   /// @brief Concept to verify a type can serve as a named energy unit
   template<typename T>
   concept IsNamedenergyUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a energy type
   template<typename T>
   concept is_energy = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_mass, 1>, 
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_timespan, -2>
   >>;

   /// @brief Retrieves the value of a energy object with specific units
   /// @tparam massUnit The mass unit used for all mass components of energy
   /// @tparam lengthUnit The length unit used for all length components of energy
   /// @tparam timespanUnit The timespan unit used for all timespan components of energy
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_energy DimType>
   constexpr PrecisionType get_energy_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named energy object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedenergyUnit Named, is_energy DimType>
   constexpr PrecisionType get_energy_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class energy;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_energy_units<
      T0,
      T1,
      T2
   >
   class energy<T0, T1, T2, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_energy<T>
      constexpr energy(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_energy_units<
      T0,
      T1,
      T2
   >
   class energy<Rep, T0, T1, T2, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -2>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -2>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_energy<T>
      constexpr energy(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<IsNamedenergyUnit Named, is_coefficient... Cs>
   class energy<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_energy<Other>
      constexpr energy(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<rep_type Rep, IsNamedenergyUnit Named, is_coefficient... Cs>
   class energy<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_energy<Other>
      constexpr energy(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_energy_units<
      T0,
      T1,
      T2
   >
   constexpr auto make_energy(Cs... coeffs)
   {
      return energy<double, T0, T1, T2, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_energy_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   constexpr auto make_energy(Rep value, Cs... coeffs)
   {
      return energy<Rep, T0, T1, T2, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<IsNamedenergyUnit Named, is_coefficient... Cs>
   constexpr auto make_energy(Cs... coeffs)
   {
      return energy<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<IsNamedenergyUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_energy(Rep value, Cs... coeffs)
   {
      return energy<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_energy Dim>
   energy(Dim) -> 
   energy<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ENERGY_IMPL_H