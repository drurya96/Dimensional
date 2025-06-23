#ifndef STATIC_DIMENSION_SPEED_IMPL_H
#define STATIC_DIMENSION_SPEED_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named speed unit
   template<typename T>
   concept IsNamedspeedUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a speed type
   template<typename T>
   concept Isspeed = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarylength, 1>, 
      unit_exponent<Primarytimespan, -1>
   >>;

   /// @brief Retrieves the value of a speed object with specific units
   /// @tparam lengthUnit The length unit used for all length components of speed
   /// @tparam timespanUnit The timespan unit used for all timespan components of speed
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      Isspeed DimType>
   constexpr PrecisionType get_speed_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 1>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named speed object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedspeedUnit Named, Isspeed DimType>
   constexpr PrecisionType get_speed_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class speed;

   /// @brief Represents the default speed
   template<>
   class speed<> : public base_dimension<
      unit_exponent<Primarylength, 1>,
      unit_exponent<Primarytimespan, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarylength, 1>,
         unit_exponent<Primarytimespan, -1>>;
      using Base::Base;

      explicit constexpr speed(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isspeed<Other>
      constexpr speed(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named speed units
   /// @tparam Named The named unit this speed type is in terms of
   template<IsNamedspeedUnit Named>
   class speed<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isspeed<Other>
      constexpr speed(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class speed<Units...> : public base_dimension<
      unit_exponent<typename Extractor<lengthType, Units...>::type, 1>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<lengthType, Units...>::type, 1>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isspeed<T>
      constexpr speed(const T& base) : Base(base) {}
   };

   template<Isspeed Dim>
   speed(Dim) -> 
   speed<
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_SPEED_IMPL_H