#ifndef STATIC_DIMENSION_CURRENT_IMPL_H
#define STATIC_DIMENSION_CURRENT_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named current unit
   template<typename T>
   concept IsNamedcurrentUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         is_charge_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a current type
   template<typename T>
   concept Iscurrent = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarycharge, 1>, 
      unit_exponent<Primarytimespan, -1>
   >>;

   /// @brief Retrieves the value of a current object with specific units
   /// @tparam chargeUnit The charge unit used for all charge components of current
   /// @tparam timespanUnit The timespan unit used for all timespan components of current
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_charge_unit chargeUnit,
      is_timespan_unit timespanUnit,
      Iscurrent DimType>
   constexpr PrecisionType get_current_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<chargeUnit, 1>,
         unit_exponent<timespanUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named current object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedcurrentUnit Named, Iscurrent DimType>
   constexpr PrecisionType get_current_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class current;

   /// @brief Represents the default current
   template<>
   class current<> : public base_dimension<
      unit_exponent<Primarycharge, 1>,
      unit_exponent<Primarytimespan, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarycharge, 1>,
         unit_exponent<Primarytimespan, -1>>;
      using Base::Base;

      explicit constexpr current(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Iscurrent<Other>
      constexpr current(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named current units
   /// @tparam Named The named unit this current type is in terms of
   template<IsNamedcurrentUnit Named>
   class current<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Iscurrent<Other>
      constexpr current(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class current<Units...> : public base_dimension<
      unit_exponent<typename Extractor<chargeType, Units...>::type, 1>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<chargeType, Units...>::type, 1>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Iscurrent<T>
      constexpr current(const T& base) : Base(base) {}
   };

   template<Iscurrent Dim>
   current(Dim) -> 
   current<
      DimExtractor<chargeType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_CURRENT_IMPL_H