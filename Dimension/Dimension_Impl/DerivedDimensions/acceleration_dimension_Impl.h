#ifndef STATIC_DIMENSION_ACCELERATION_IMPL_H
#define STATIC_DIMENSION_ACCELERATION_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named acceleration unit
   template<typename T>
   concept IsNamedaccelerationUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_length_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a acceleration type
   template<typename T>
   concept Isacceleration = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarylength, 1>, 
      unit_exponent<Primarytimespan, -2>
   >>;

   /// @brief Retrieves the value of a acceleration object with specific units
   /// @tparam lengthUnit The length unit used for all length components of acceleration
   /// @tparam timespanUnit The timespan unit used for all timespan components of acceleration
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      Isacceleration DimType>
   constexpr PrecisionType get_acceleration_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<lengthUnit, 1>,
         unit_exponent<timespanUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named acceleration object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedaccelerationUnit Named, Isacceleration DimType>
   constexpr PrecisionType get_acceleration_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class acceleration;

   /// @brief Represents the default acceleration
   template<>
   class acceleration<> : public base_dimension<
      unit_exponent<Primarylength, 1>,
      unit_exponent<Primarytimespan, -2>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarylength, 1>,
         unit_exponent<Primarytimespan, -2>>;
      using Base::Base;

      explicit constexpr acceleration(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isacceleration<Other>
      constexpr acceleration(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named acceleration units
   /// @tparam Named The named unit this acceleration type is in terms of
   template<IsNamedaccelerationUnit Named>
   class acceleration<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isacceleration<Other>
      constexpr acceleration(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class acceleration<Units...> : public base_dimension<
      unit_exponent<typename Extractor<lengthType, Units...>::type, 1>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -2>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<lengthType, Units...>::type, 1>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isacceleration<T>
      constexpr acceleration(const T& base) : Base(base) {}
   };

   template<Isacceleration Dim>
   acceleration(Dim) -> 
   acceleration<
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ACCELERATION_IMPL_H