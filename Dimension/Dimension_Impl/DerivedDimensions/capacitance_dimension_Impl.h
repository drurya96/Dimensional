#ifndef STATIC_DIMENSION_CAPACITANCE_IMPL_H
#define STATIC_DIMENSION_CAPACITANCE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named capacitance unit
   template<typename T>
   concept IsNamedcapacitanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         is_charge_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_mass_unit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a capacitance type
   template<typename T>
   concept Iscapacitance = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarycharge, 2>, 
      unit_exponent<Primarytimespan, 2>, 
      unit_exponent<Primarymass, -1>, 
      unit_exponent<Primarylength, -2>
   >>;

   /// @brief Retrieves the value of a capacitance object with specific units
   /// @tparam chargeUnit The charge unit used for all charge components of capacitance
   /// @tparam timespanUnit The timespan unit used for all timespan components of capacitance
   /// @tparam massUnit The mass unit used for all mass components of capacitance
   /// @tparam lengthUnit The length unit used for all length components of capacitance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_charge_unit chargeUnit,
      is_timespan_unit timespanUnit,
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      Iscapacitance DimType>
   constexpr PrecisionType get_capacitance_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<chargeUnit, 2>,
         unit_exponent<timespanUnit, 2>,
         unit_exponent<massUnit, -1>,
         unit_exponent<lengthUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named capacitance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedcapacitanceUnit Named, Iscapacitance DimType>
   constexpr PrecisionType get_capacitance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class capacitance;

   /// @brief Represents the default capacitance
   template<>
   class capacitance<> : public base_dimension<
      unit_exponent<Primarycharge, 2>,
      unit_exponent<Primarytimespan, 2>,
      unit_exponent<Primarymass, -1>,
      unit_exponent<Primarylength, -2>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarycharge, 2>,
         unit_exponent<Primarytimespan, 2>,
         unit_exponent<Primarymass, -1>,
         unit_exponent<Primarylength, -2>>;
      using Base::Base;

      explicit constexpr capacitance(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Iscapacitance<Other>
      constexpr capacitance(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named capacitance units
   /// @tparam Named The named unit this capacitance type is in terms of
   template<IsNamedcapacitanceUnit Named>
   class capacitance<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Iscapacitance<Other>
      constexpr capacitance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class capacitance<Units...> : public base_dimension<
      unit_exponent<typename Extractor<chargeType, Units...>::type, 2>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, 2>,
      unit_exponent<typename Extractor<massType, Units...>::type, -1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, -2>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<chargeType, Units...>::type, 2>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, 2>,
         unit_exponent<typename Extractor<massType, Units...>::type, -1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Iscapacitance<T>
      constexpr capacitance(const T& base) : Base(base) {}
   };

   template<Iscapacitance Dim>
   capacitance(Dim) -> 
   capacitance<
      DimExtractor<chargeType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_CAPACITANCE_IMPL_H