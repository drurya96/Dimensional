#ifndef STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H
#define STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named magnetic_field unit
   template<typename T>
   concept IsNamedmagnetic_fieldUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a magnetic_field type
   template<typename T>
   concept Ismagnetic_field = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarytimespan, -1>, 
      unit_exponent<Primarycharge, -1>
   >>;

   /// @brief Retrieves the value of a magnetic_field object with specific units
   /// @tparam massUnit The mass unit used for all mass components of magnetic_field
   /// @tparam timespanUnit The timespan unit used for all timespan components of magnetic_field
   /// @tparam chargeUnit The charge unit used for all charge components of magnetic_field
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      Ismagnetic_field DimType>
   constexpr PrecisionType get_magnetic_field_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<timespanUnit, -1>,
         unit_exponent<chargeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named magnetic_field object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedmagnetic_fieldUnit Named, Ismagnetic_field DimType>
   constexpr PrecisionType get_magnetic_field_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class magnetic_field;

   /// @brief Represents the default magnetic_field
   template<>
   class magnetic_field<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarytimespan, -1>,
      unit_exponent<Primarycharge, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarytimespan, -1>,
         unit_exponent<Primarycharge, -1>>;
      using Base::Base;

      explicit constexpr magnetic_field(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Ismagnetic_field<Other>
      constexpr magnetic_field(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named magnetic_field units
   /// @tparam Named The named unit this magnetic_field type is in terms of
   template<IsNamedmagnetic_fieldUnit Named>
   class magnetic_field<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Ismagnetic_field<Other>
      constexpr magnetic_field(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class magnetic_field<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>,
      unit_exponent<typename Extractor<chargeType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>,
         unit_exponent<typename Extractor<chargeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Ismagnetic_field<T>
      constexpr magnetic_field(const T& base) : Base(base) {}
   };

   template<Ismagnetic_field Dim>
   magnetic_field(Dim) -> 
   magnetic_field<
      DimExtractor<massType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<chargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MAGNETIC_FIELD_IMPL_H