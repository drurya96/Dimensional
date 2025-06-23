#ifndef STATIC_DIMENSION_SPECIFIC_HEAT_CAPACITY_IMPL_H
#define STATIC_DIMENSION_SPECIFIC_HEAT_CAPACITY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/temperature_dimension.h"

namespace dimension
{
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
   concept Isspecific_heat_capacity = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarylength, 2>, 
      unit_exponent<Primarytimespan, -2>, 
      unit_exponent<Primarytemperature, -1>
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
      Isspecific_heat_capacity DimType>
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
   template<IsNamedspecific_heat_capacityUnit Named, Isspecific_heat_capacity DimType>
   constexpr PrecisionType get_specific_heat_capacity_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class specific_heat_capacity;

   /// @brief Represents the default specific_heat_capacity
   template<>
   class specific_heat_capacity<> : public base_dimension<
      unit_exponent<Primarylength, 2>,
      unit_exponent<Primarytimespan, -2>,
      unit_exponent<Primarytemperature, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarylength, 2>,
         unit_exponent<Primarytimespan, -2>,
         unit_exponent<Primarytemperature, -1>>;
      using Base::Base;

      explicit constexpr specific_heat_capacity(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isspecific_heat_capacity<Other>
      constexpr specific_heat_capacity(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named specific_heat_capacity units
   /// @tparam Named The named unit this specific_heat_capacity type is in terms of
   template<IsNamedspecific_heat_capacityUnit Named>
   class specific_heat_capacity<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isspecific_heat_capacity<Other>
      constexpr specific_heat_capacity(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class specific_heat_capacity<Units...> : public base_dimension<
      unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -2>,
      unit_exponent<typename Extractor<temperatureType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -2>,
         unit_exponent<typename Extractor<temperatureType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isspecific_heat_capacity<T>
      constexpr specific_heat_capacity(const T& base) : Base(base) {}
   };

   template<Isspecific_heat_capacity Dim>
   specific_heat_capacity(Dim) -> 
   specific_heat_capacity<
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<temperatureType, Dim>
   >;
}

#endif // STATIC_DIMENSION_SPECIFIC_HEAT_CAPACITY_IMPL_H