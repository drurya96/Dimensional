#ifndef STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H
#define STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
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
   concept Isdiffusion_coefficient = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarylength, 2>, 
      unit_exponent<Primarytimespan, -1>
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
      Isdiffusion_coefficient DimType>
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
   template<IsNameddiffusion_coefficientUnit Named, Isdiffusion_coefficient DimType>
   constexpr PrecisionType get_diffusion_coefficient_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class diffusion_coefficient;

   /// @brief Represents the default diffusion_coefficient
   template<>
   class diffusion_coefficient<> : public base_dimension<
      unit_exponent<Primarylength, 2>,
      unit_exponent<Primarytimespan, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarylength, 2>,
         unit_exponent<Primarytimespan, -1>>;
      using Base::Base;

      explicit constexpr diffusion_coefficient(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isdiffusion_coefficient<Other>
      constexpr diffusion_coefficient(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named diffusion_coefficient units
   /// @tparam Named The named unit this diffusion_coefficient type is in terms of
   template<IsNameddiffusion_coefficientUnit Named>
   class diffusion_coefficient<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isdiffusion_coefficient<Other>
      constexpr diffusion_coefficient(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class diffusion_coefficient<Units...> : public base_dimension<
      unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isdiffusion_coefficient<T>
      constexpr diffusion_coefficient(const T& base) : Base(base) {}
   };

   template<Isdiffusion_coefficient Dim>
   diffusion_coefficient(Dim) -> 
   diffusion_coefficient<
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_DIFFUSION_COEFFICIENT_IMPL_H