#ifndef STATIC_DIMENSION_RESISTANCE_IMPL_H
#define STATIC_DIMENSION_RESISTANCE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named resistance unit
   template<typename T>
   concept IsNamedresistanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a resistance type
   template<typename T>
   concept Isresistance = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, 2>, 
      unit_exponent<Primarytimespan, -1>, 
      unit_exponent<Primarycharge, -2>
   >>;

   /// @brief Retrieves the value of a resistance object with specific units
   /// @tparam massUnit The mass unit used for all mass components of resistance
   /// @tparam lengthUnit The length unit used for all length components of resistance
   /// @tparam timespanUnit The timespan unit used for all timespan components of resistance
   /// @tparam chargeUnit The charge unit used for all charge components of resistance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      Isresistance DimType>
   constexpr PrecisionType get_resistance_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -1>,
         unit_exponent<chargeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named resistance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedresistanceUnit Named, Isresistance DimType>
   constexpr PrecisionType get_resistance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class resistance;

   /// @brief Represents the default resistance
   template<>
   class resistance<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarylength, 2>,
      unit_exponent<Primarytimespan, -1>,
      unit_exponent<Primarycharge, -2>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarylength, 2>,
         unit_exponent<Primarytimespan, -1>,
         unit_exponent<Primarycharge, -2>>;
      using Base::Base;

      explicit constexpr resistance(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isresistance<Other>
      constexpr resistance(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named resistance units
   /// @tparam Named The named unit this resistance type is in terms of
   template<IsNamedresistanceUnit Named>
   class resistance<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isresistance<Other>
      constexpr resistance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class resistance<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>,
      unit_exponent<typename Extractor<chargeType, Units...>::type, -2>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>,
         unit_exponent<typename Extractor<chargeType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isresistance<T>
      constexpr resistance(const T& base) : Base(base) {}
   };

   template<Isresistance Dim>
   resistance(Dim) -> 
   resistance<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<chargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_RESISTANCE_IMPL_H