#ifndef STATIC_DIMENSION_CONDUCTANCE_IMPL_H
#define STATIC_DIMENSION_CONDUCTANCE_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/charge_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named conductance unit
   template<typename T>
   concept IsNamedconductanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         is_timespan_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_charge_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_mass_unit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a conductance type
   template<typename T>
   concept Isconductance = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarytimespan, 1>, 
      unit_exponent<Primarycharge, 2>, 
      unit_exponent<Primarymass, -1>, 
      unit_exponent<Primarylength, -2>
   >>;

   /// @brief Retrieves the value of a conductance object with specific units
   /// @tparam timespanUnit The timespan unit used for all timespan components of conductance
   /// @tparam chargeUnit The charge unit used for all charge components of conductance
   /// @tparam massUnit The mass unit used for all mass components of conductance
   /// @tparam lengthUnit The length unit used for all length components of conductance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_timespan_unit timespanUnit,
      is_charge_unit chargeUnit,
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      Isconductance DimType>
   constexpr PrecisionType get_conductance_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<timespanUnit, 1>,
         unit_exponent<chargeUnit, 2>,
         unit_exponent<massUnit, -1>,
         unit_exponent<lengthUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named conductance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedconductanceUnit Named, Isconductance DimType>
   constexpr PrecisionType get_conductance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class conductance;

   /// @brief Represents the default conductance
   template<>
   class conductance<> : public base_dimension<
      unit_exponent<Primarytimespan, 1>,
      unit_exponent<Primarycharge, 2>,
      unit_exponent<Primarymass, -1>,
      unit_exponent<Primarylength, -2>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarytimespan, 1>,
         unit_exponent<Primarycharge, 2>,
         unit_exponent<Primarymass, -1>,
         unit_exponent<Primarylength, -2>>;
      using Base::Base;

      explicit constexpr conductance(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isconductance<Other>
      constexpr conductance(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named conductance units
   /// @tparam Named The named unit this conductance type is in terms of
   template<IsNamedconductanceUnit Named>
   class conductance<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isconductance<Other>
      constexpr conductance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class conductance<Units...> : public base_dimension<
      unit_exponent<typename Extractor<timespanType, Units...>::type, 1>,
      unit_exponent<typename Extractor<chargeType, Units...>::type, 2>,
      unit_exponent<typename Extractor<massType, Units...>::type, -1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, -2>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<timespanType, Units...>::type, 1>,
         unit_exponent<typename Extractor<chargeType, Units...>::type, 2>,
         unit_exponent<typename Extractor<massType, Units...>::type, -1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isconductance<T>
      constexpr conductance(const T& base) : Base(base) {}
   };

   template<Isconductance Dim>
   conductance(Dim) -> 
   conductance<
      DimExtractor<timespanType, Dim>,
      DimExtractor<chargeType, Dim>,
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_CONDUCTANCE_IMPL_H