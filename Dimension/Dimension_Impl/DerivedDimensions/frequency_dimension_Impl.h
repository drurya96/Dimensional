#ifndef STATIC_DIMENSION_FREQUENCY_IMPL_H
#define STATIC_DIMENSION_FREQUENCY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named frequency unit
   template<typename T>
   concept IsNamedfrequencyUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 1 &&
         is_timespan_unit<typename std::tuple_element_t<0, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a frequency type
   template<typename T>
   concept Isfrequency = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarytimespan, -1>
   >>;

   /// @brief Retrieves the value of a frequency object with specific units
   /// @tparam timespanUnit The timespan unit used for all timespan components of frequency
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_timespan_unit timespanUnit,
      Isfrequency DimType>
   constexpr PrecisionType get_frequency_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named frequency object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedfrequencyUnit Named, Isfrequency DimType>
   constexpr PrecisionType get_frequency_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class frequency;

   /// @brief Represents the default frequency
   template<>
   class frequency<> : public base_dimension<
      unit_exponent<Primarytimespan, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarytimespan, -1>>;
      using Base::Base;

      explicit constexpr frequency(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isfrequency<Other>
      constexpr frequency(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named frequency units
   /// @tparam Named The named unit this frequency type is in terms of
   template<IsNamedfrequencyUnit Named>
   class frequency<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isfrequency<Other>
      constexpr frequency(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class frequency<Units...> : public base_dimension<
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isfrequency<T>
      constexpr frequency(const T& base) : Base(base) {}
   };

   template<Isfrequency Dim>
   frequency(Dim) -> 
   frequency<
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_FREQUENCY_IMPL_H