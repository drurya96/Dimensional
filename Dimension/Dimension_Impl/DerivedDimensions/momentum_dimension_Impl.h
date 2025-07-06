#ifndef STATIC_DIMENSION_MOMENTUM_IMPL_H
#define STATIC_DIMENSION_MOMENTUM_IMPL_H

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
   concept are_momentum_units =
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

   /// @brief Concept to verify a type can serve as a named momentum unit
   template<typename T>
   concept IsNamedmomentumUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a momentum type
   template<typename T>
   concept Ismomentum = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, 1>, 
      unit_exponent<Primarytimespan, -1>
   >>;

   /// @brief Retrieves the value of a momentum object with specific units
   /// @tparam massUnit The mass unit used for all mass components of momentum
   /// @tparam lengthUnit The length unit used for all length components of momentum
   /// @tparam timespanUnit The timespan unit used for all timespan components of momentum
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      Ismomentum DimType>
   constexpr PrecisionType get_momentum_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 1>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named momentum object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmomentumUnit Named, Ismomentum DimType>
   constexpr PrecisionType get_momentum_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class momentum;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_momentum_units<
      T0,
      T1,
      T2
   >
   class momentum<T0, T1, T2, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Ismomentum<T>
      constexpr momentum(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_momentum_units<
      T0,
      T1,
      T2
   >
   class momentum<Rep, T0, T1, T2, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 1>,
      unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<massType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<lengthType, T0, T1, T2>::type, 1>,
         unit_exponent<typename Extractor<timespanType, T0, T1, T2>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Ismomentum<T>
      constexpr momentum(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named momentum units
   /// @tparam Named The named unit this momentum type is in terms of
   template<IsNamedmomentumUnit Named, is_coefficient... Cs>
   class momentum<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires Ismomentum<Other>
      constexpr momentum(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named momentum units
   /// @tparam Named The named unit this momentum type is in terms of
   template<rep_type Rep, IsNamedmomentumUnit Named, is_coefficient... Cs>
   class momentum<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires Ismomentum<Other>
      constexpr momentum(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      is_coefficient... Cs
   >
   requires are_momentum_units<
      T0,
      T1,
      T2
   >
   constexpr auto make_momentum(Cs... coeffs)
   {
      return momentum<double, T0, T1, T2, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      IsBasicUnitType T2,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_momentum_units<
      T0,
      T1,
      T2
   > && (!is_coefficient<Rep>)
   constexpr auto make_momentum(Rep value, Cs... coeffs)
   {
      return momentum<Rep, T0, T1, T2, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named momentum units
   /// @tparam Named The named unit this momentum type is in terms of
   template<IsNamedmomentumUnit Named, is_coefficient... Cs>
   constexpr auto make_momentum(Cs... coeffs)
   {
      return momentum<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named momentum units
   /// @tparam Named The named unit this momentum type is in terms of
   template<IsNamedmomentumUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_momentum(Rep value, Cs... coeffs)
   {
      return momentum<Rep, Named, Cs...>(value, coeffs...);
   }









   template<Ismomentum Dim>
   momentum(Dim) -> 
   momentum<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MOMENTUM_IMPL_H