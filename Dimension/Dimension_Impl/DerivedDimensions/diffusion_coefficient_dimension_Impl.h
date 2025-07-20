#ifndef STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H
#define STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"


namespace dimension
{

   template<
         typename T0,
         typename T1
   >
   concept are_diffusion_coefficient_units =
         (
               is_length_unit<T0> && 
               is_timespan_unit<T1>
         ) ||
         (
               is_timespan_unit<T0> && 
               is_length_unit<T1>
         )
   ;

   /// @brief Concept to verify a type can serve as a named diffusion_coefficient unit
   template<typename T>
   concept IsNameddiffusion_coefficientUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a diffusion_coefficient type
   template<typename T>
   concept is_diffusion_coefficient = std::is_convertible_v<T, base_dimension<
      unit_exponent<primary_length, 2>, 
      unit_exponent<primary_timespan, -1>
   >>;

   /// @brief Retrieves the value of a diffusion_coefficient object with specific units
   /// @tparam lengthUnit The length unit used for all length components of diffusion_coefficient
   /// @tparam timespanUnit The timespan unit used for all timespan components of diffusion_coefficient
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_diffusion_coefficient DimType>
   constexpr PrecisionType get_diffusion_coefficient_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named diffusion_coefficient object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNameddiffusion_coefficientUnit Named, is_diffusion_coefficient DimType>
   constexpr PrecisionType get_diffusion_coefficient_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class diffusion_coefficient;




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_diffusion_coefficient_units<
      T0,
      T1
   >
   class diffusion_coefficient<T0, T1, Cs...> : public base_dimension<double,
      unit_exponent<typename Extractor<lengthType, T0, T1>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<double,
         unit_exponent<typename Extractor<lengthType, T0, T1>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_diffusion_coefficient<T>
      constexpr diffusion_coefficient(const T& base) : Base(base) {}
   };




   template<
      rep_type Rep,
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_diffusion_coefficient_units<
      T0,
      T1
   >
   class diffusion_coefficient<Rep, T0, T1, Cs...> : public base_dimension<Rep,
      unit_exponent<typename Extractor<lengthType, T0, T1>::type, 2>,
      unit_exponent<typename Extractor<timespanType, T0, T1>::type, -1>,
      Cs...
   >
   {
   public:
      using Base = base_dimension<Rep,
         unit_exponent<typename Extractor<lengthType, T0, T1>::type, 2>,
         unit_exponent<typename Extractor<timespanType, T0, T1>::type, -1>,
         Cs...
      >;
   
      using Base::Base;
   
      template<typename T>
      requires is_diffusion_coefficient<T>
      constexpr diffusion_coefficient(const T& base) : Base(base) {}
   };




   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<IsNameddiffusion_coefficientUnit Named, is_coefficient... Cs>
   class diffusion_coefficient<Named, Cs...> : public base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<double, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_diffusion_coefficient<Other>
      constexpr diffusion_coefficient(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<rep_type Rep, IsNameddiffusion_coefficientUnit Named, is_coefficient... Cs>
   class diffusion_coefficient<Rep, Named, Cs...> : public base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<Rep, typename Named::units, std::tuple<Cs...>>::dim;
      using Base::Base;

      template<typename Other>
      requires is_diffusion_coefficient<Other>
      constexpr diffusion_coefficient(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };




   




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      is_coefficient... Cs
   >
   requires are_diffusion_coefficient_units<
      T0,
      T1
   >
   constexpr auto make_diffusion_coefficient(Cs... coeffs)
   {
      return diffusion_coefficient<double, T0, T1, Cs...>(1.0, coeffs...);
   }




   template<
      IsBasicUnitType T0,
      IsBasicUnitType T1,
      rep_type Rep,
      is_coefficient... Cs
   >
   requires are_diffusion_coefficient_units<
      T0,
      T1
   > && (!is_coefficient<Rep>)
   constexpr auto make_diffusion_coefficient(Rep value, Cs... coeffs)
   {
      return diffusion_coefficient<Rep, T0, T1, Cs...>(value, coeffs...);
   }




   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<IsNameddiffusion_coefficientUnit Named, is_coefficient... Cs>
   constexpr auto make_diffusion_coefficient(Cs... coeffs)
   {
      return diffusion_coefficient<double, Named, Cs...>(1.0, coeffs...);
   }






   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<IsNameddiffusion_coefficientUnit Named, rep_type Rep, is_coefficient... Cs>
   constexpr auto make_diffusion_coefficient(Rep value, Cs... coeffs)
   {
      return diffusion_coefficient<Rep, Named, Cs...>(value, coeffs...);
   }









   template<is_diffusion_coefficient Dim>
   diffusion_coefficient(Dim) -> 
   diffusion_coefficient<
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H