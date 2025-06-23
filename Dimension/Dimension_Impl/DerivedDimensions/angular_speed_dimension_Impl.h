#ifndef STATIC_DIMENSION_ANGULAR_SPEED_IMPL_H
#define STATIC_DIMENSION_ANGULAR_SPEED_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/angle_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named angular_speed unit
   template<typename T>
   concept IsNamedangular_speedUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_angle_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a angular_speed type
   template<typename T>
   concept Isangular_speed = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primaryangle, 1>, 
      unit_exponent<Primarytimespan, -1>
   >>;

   /// @brief Retrieves the value of a angular_speed object with specific units
   /// @tparam angleUnit The angle unit used for all angle components of angular_speed
   /// @tparam timespanUnit The timespan unit used for all timespan components of angular_speed
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_angle_unit angleUnit,
      is_timespan_unit timespanUnit,
      Isangular_speed DimType>
   constexpr PrecisionType get_angular_speed_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<angleUnit, 1>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named angular_speed object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedangular_speedUnit Named, Isangular_speed DimType>
   constexpr PrecisionType get_angular_speed_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class angular_speed;

   /// @brief Represents the default angular_speed
   template<>
   class angular_speed<> : public base_dimension<
      unit_exponent<Primaryangle, 1>,
      unit_exponent<Primarytimespan, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primaryangle, 1>,
         unit_exponent<Primarytimespan, -1>>;
      using Base::Base;

      explicit constexpr angular_speed(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isangular_speed<Other>
      constexpr angular_speed(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named angular_speed units
   /// @tparam Named The named unit this angular_speed type is in terms of
   template<IsNamedangular_speedUnit Named>
   class angular_speed<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isangular_speed<Other>
      constexpr angular_speed(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class angular_speed<Units...> : public base_dimension<
      unit_exponent<typename Extractor<angleType, Units...>::type, 1>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<angleType, Units...>::type, 1>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isangular_speed<T>
      constexpr angular_speed(const T& base) : Base(base) {}
   };

   template<Isangular_speed Dim>
   angular_speed(Dim) -> 
   angular_speed<
      DimExtractor<angleType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ANGULAR_SPEED_IMPL_H