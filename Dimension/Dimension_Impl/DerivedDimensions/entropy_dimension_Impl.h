#ifndef STATIC_DIMENSION_ENTROPY_IMPL_H
#define STATIC_DIMENSION_ENTROPY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/temperature_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named entropy unit
   template<typename T>
   concept IsNamedentropyUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         is_temperature_unit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a entropy type
   template<typename T>
   concept Isentropy = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, 2>, 
      unit_exponent<Primarytimespan, -2>, 
      unit_exponent<Primarytemperature, -1>
   >>;

   /// @brief Retrieves the value of a entropy object with specific units
   /// @tparam massUnit The mass unit used for all mass components of entropy
   /// @tparam lengthUnit The length unit used for all length components of entropy
   /// @tparam timespanUnit The timespan unit used for all timespan components of entropy
   /// @tparam temperatureUnit The temperature unit used for all temperature components of entropy
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_temperature_unit temperatureUnit,
      Isentropy DimType>
   constexpr PrecisionType get_entropy_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -2>,
         unit_exponent<temperatureUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named entropy object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedentropyUnit Named, Isentropy DimType>
   constexpr PrecisionType get_entropy_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class entropy;

   /// @brief Represents the default entropy
   template<>
   class entropy<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarylength, 2>,
      unit_exponent<Primarytimespan, -2>,
      unit_exponent<Primarytemperature, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarylength, 2>,
         unit_exponent<Primarytimespan, -2>,
         unit_exponent<Primarytemperature, -1>>;
      using Base::Base;

      explicit constexpr entropy(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isentropy<Other>
      constexpr entropy(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named entropy units
   /// @tparam Named The named unit this entropy type is in terms of
   template<IsNamedentropyUnit Named>
   class entropy<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isentropy<Other>
      constexpr entropy(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class entropy<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -2>,
      unit_exponent<typename Extractor<temperatureType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -2>,
         unit_exponent<typename Extractor<temperatureType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isentropy<T>
      constexpr entropy(const T& base) : Base(base) {}
   };

   template<Isentropy Dim>
   entropy(Dim) -> 
   entropy<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<temperatureType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ENTROPY_IMPL_H