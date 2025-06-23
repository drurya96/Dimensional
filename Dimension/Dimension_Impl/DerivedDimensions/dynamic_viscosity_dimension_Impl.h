#ifndef STATIC_DIMENSION_DYNAMIC_VISCOSITY_IMPL_H
#define STATIC_DIMENSION_DYNAMIC_VISCOSITY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named dynamic_viscosity unit
   template<typename T>
   concept IsNameddynamic_viscosityUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a dynamic_viscosity type
   template<typename T>
   concept Isdynamic_viscosity = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarytimespan, -1>, 
      unit_exponent<Primarylength, -1>
   >>;

   /// @brief Retrieves the value of a dynamic_viscosity object with specific units
   /// @tparam massUnit The mass unit used for all mass components of dynamic_viscosity
   /// @tparam timespanUnit The timespan unit used for all timespan components of dynamic_viscosity
   /// @tparam lengthUnit The length unit used for all length components of dynamic_viscosity
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_timespan_unit timespanUnit,
      is_length_unit lengthUnit,
      Isdynamic_viscosity DimType>
   constexpr PrecisionType get_dynamic_viscosity_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<timespanUnit, -1>,
         unit_exponent<lengthUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named dynamic_viscosity object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNameddynamic_viscosityUnit Named, Isdynamic_viscosity DimType>
   constexpr PrecisionType get_dynamic_viscosity_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class dynamic_viscosity;

   /// @brief Represents the default dynamic_viscosity
   template<>
   class dynamic_viscosity<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarytimespan, -1>,
      unit_exponent<Primarylength, -1>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarytimespan, -1>,
         unit_exponent<Primarylength, -1>>;
      using Base::Base;

      explicit constexpr dynamic_viscosity(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isdynamic_viscosity<Other>
      constexpr dynamic_viscosity(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named dynamic_viscosity units
   /// @tparam Named The named unit this dynamic_viscosity type is in terms of
   template<IsNameddynamic_viscosityUnit Named>
   class dynamic_viscosity<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isdynamic_viscosity<Other>
      constexpr dynamic_viscosity(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class dynamic_viscosity<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, -1>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isdynamic_viscosity<T>
      constexpr dynamic_viscosity(const T& base) : Base(base) {}
   };

   template<Isdynamic_viscosity Dim>
   dynamic_viscosity(Dim) -> 
   dynamic_viscosity<
      DimExtractor<massType, Dim>,
      DimExtractor<timespanType, Dim>,
      DimExtractor<lengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_DYNAMIC_VISCOSITY_IMPL_H