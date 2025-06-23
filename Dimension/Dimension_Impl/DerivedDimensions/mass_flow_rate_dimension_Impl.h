#ifndef STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H
#define STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named mass_flow_rate unit
   template<typename T>
   concept IsNamedmass_flow_rateUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a mass_flow_rate type
   template<typename T>
   concept Ismass_flow_rate = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarytimespan, -1>
   >>;

   /// @brief Retrieves the value of a mass_flow_rate object with specific units
   /// @tparam massUnit The mass unit used for all mass components of mass_flow_rate
   /// @tparam timespanUnit The timespan unit used for all timespan components of mass_flow_rate
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      Ismass_flow_rate DimType>
   constexpr PrecisionType get_mass_flow_rate_as(const DimType& obj)
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
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmass_flow_rateUnit Named, Ismass_flow_rate DimType>
   constexpr PrecisionType get_mass_flow_rate_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class mass_flow_rate;

   /// @brief Represents the default mass_flow_rate
   template<>
   class mass_flow_rate<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarytimespan, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarytimespan, -1>>;
      using Base::Base;

      explicit constexpr mass_flow_rate(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Ismass_flow_rate<Other>
      constexpr mass_flow_rate(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named mass_flow_rate units
   /// @tparam Named The named unit this mass_flow_rate type is in terms of
   template<IsNamedmass_flow_rateUnit Named>
   class mass_flow_rate<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Ismass_flow_rate<Other>
      constexpr mass_flow_rate(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class mass_flow_rate<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Ismass_flow_rate<T>
      constexpr mass_flow_rate(const T& base) : Base(base) {}
   };

   template<Ismass_flow_rate Dim>
   mass_flow_rate(Dim) -> 
   mass_flow_rate<
      DimExtractor<massType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MASS_FLOW_RATE_IMPL_H