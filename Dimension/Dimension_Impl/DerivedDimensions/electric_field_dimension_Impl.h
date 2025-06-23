#ifndef STATIC_DIMENSION_ELECTRIC_FIELD_IMPL_H
#define STATIC_DIMENSION_ELECTRIC_FIELD_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named electric_field unit
   template<typename T>
   concept IsNamedelectric_fieldUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a electric_field type
   template<typename T>
   concept Iselectric_field = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, 1>, 
      unit_exponent<Primarytimespan, -2>, 
      unit_exponent<Primarycharge, -1>
   >>;

   /// @brief Retrieves the value of a electric_field object with specific units
   /// @tparam massUnit The mass unit used for all mass components of electric_field
   /// @tparam lengthUnit The length unit used for all length components of electric_field
   /// @tparam timespanUnit The timespan unit used for all timespan components of electric_field
   /// @tparam chargeUnit The charge unit used for all charge components of electric_field
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      Iselectric_field DimType>
   constexpr PrecisionType get_electric_field_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 1>,
         unit_exponent<timespanUnit, -2>,
         unit_exponent<chargeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named electric_field object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedelectric_fieldUnit Named, Iselectric_field DimType>
   constexpr PrecisionType get_electric_field_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class electric_field;

   /// @brief Represents the default electric_field
   template<>
   class electric_field<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarylength, 1>,
      unit_exponent<Primarytimespan, -2>,
      unit_exponent<Primarycharge, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarylength, 1>,
         unit_exponent<Primarytimespan, -2>,
         unit_exponent<Primarycharge, -1>>;
      using Base::Base;

      explicit constexpr electric_field(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Iselectric_field<Other>
      constexpr electric_field(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named electric_field units
   /// @tparam Named The named unit this electric_field type is in terms of
   template<IsNamedelectric_fieldUnit Named>
   class electric_field<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Iselectric_field<Other>
      constexpr electric_field(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class electric_field<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, 1>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -2>,
      unit_exponent<typename Extractor<chargeType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, 1>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -2>,
         unit_exponent<typename Extractor<chargeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Iselectric_field<T>
      constexpr electric_field(const T& base) : Base(base) {}
   };

   template<Iselectric_field Dim>
   electric_field(Dim) -> 
   electric_field<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<chargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ELECTRIC_FIELD_IMPL_H