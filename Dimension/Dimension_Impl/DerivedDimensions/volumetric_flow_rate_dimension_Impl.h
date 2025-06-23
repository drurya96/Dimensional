#ifndef STATIC_DIMENSION_VOLUMETRIC_FLOW_RATE_IMPL_H
#define STATIC_DIMENSION_VOLUMETRIC_FLOW_RATE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named volumetric_flow_rate unit
   template<typename T>
   concept IsNamedvolumetric_flow_rateUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a volumetric_flow_rate type
   template<typename T>
   concept Isvolumetric_flow_rate = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarylength, 3>, 
      unit_exponent<Primarytimespan, -1>
   >>;

   /// @brief Retrieves the value of a volumetric_flow_rate object with specific units
   /// @tparam lengthUnit The length unit used for all length components of volumetric_flow_rate
   /// @tparam timespanUnit The timespan unit used for all timespan components of volumetric_flow_rate
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      Isvolumetric_flow_rate DimType>
   constexpr PrecisionType get_volumetric_flow_rate_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 3>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named volumetric_flow_rate object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedvolumetric_flow_rateUnit Named, Isvolumetric_flow_rate DimType>
   constexpr PrecisionType get_volumetric_flow_rate_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class volumetric_flow_rate;

   /// @brief Represents the default volumetric_flow_rate
   template<>
   class volumetric_flow_rate<> : public base_dimension<
      unit_exponent<Primarylength, 3>,
      unit_exponent<Primarytimespan, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarylength, 3>,
         unit_exponent<Primarytimespan, -1>>;
      using Base::Base;

      explicit constexpr volumetric_flow_rate(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isvolumetric_flow_rate<Other>
      constexpr volumetric_flow_rate(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named volumetric_flow_rate units
   /// @tparam Named The named unit this volumetric_flow_rate type is in terms of
   template<IsNamedvolumetric_flow_rateUnit Named>
   class volumetric_flow_rate<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isvolumetric_flow_rate<Other>
      constexpr volumetric_flow_rate(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class volumetric_flow_rate<Units...> : public base_dimension<
      unit_exponent<typename Extractor<lengthType, Units...>::type, 3>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<lengthType, Units...>::type, 3>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isvolumetric_flow_rate<T>
      constexpr volumetric_flow_rate(const T& base) : Base(base) {}
   };

   template<Isvolumetric_flow_rate Dim>
   volumetric_flow_rate(Dim) -> 
   volumetric_flow_rate<
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_VOLUMETRIC_FLOW_RATE_IMPL_H