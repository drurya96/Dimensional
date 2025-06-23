#ifndef STATIC_DIMENSION_ENERGY_IMPL_H
#define STATIC_DIMENSION_ENERGY_IMPL_H

#include "../../base_dimension.h"
#include "../../dimensions/fundamental/mass_dimension.h"
#include "../../dimensions/fundamental/length_dimension.h"
#include "../../dimensions/fundamental/timespan_dimension.h"

namespace dimension
{
   /// @brief Concept to verify a type can serve as a named energy unit
   template<typename T>
   concept IsNamedenergyUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         is_mass_unit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         is_length_unit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         is_timespan_unit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a energy type
   template<typename T>
   concept Isenergy = std::is_convertible_v<T, base_dimension<
      unit_exponent<Primarymass, 1>, 
      unit_exponent<Primarylength, 2>, 
      unit_exponent<Primarytimespan, -2>
   >>;

   /// @brief Retrieves the value of a energy object with specific units
   /// @tparam massUnit The mass unit used for all mass components of energy
   /// @tparam lengthUnit The length unit used for all length components of energy
   /// @tparam timespanUnit The timespan unit used for all timespan components of energy
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      is_mass_unit massUnit,
      is_length_unit lengthUnit,
      is_timespan_unit timespanUnit,
      Isenergy DimType>
   constexpr PrecisionType get_energy_as(const DimType& obj)
   {
      return get_dimension_as<
         unit_exponent<massUnit, 1>,
         unit_exponent<lengthUnit, 2>,
         unit_exponent<timespanUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named energy object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedenergyUnit Named, Isenergy DimType>
   constexpr PrecisionType get_energy_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class energy;

   /// @brief Represents the default energy
   template<>
   class energy<> : public base_dimension<
      unit_exponent<Primarymass, 1>,
      unit_exponent<Primarylength, 2>,
      unit_exponent<Primarytimespan, -2>>
   {
   public:
      using Base = base_dimension<
         unit_exponent<Primarymass, 1>,
         unit_exponent<Primarylength, 2>,
         unit_exponent<Primarytimespan, -2>>;
      using Base::Base;

      explicit constexpr energy(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires Isenergy<Other>
      constexpr energy(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named energy units
   /// @tparam Named The named unit this energy type is in terms of
   template<IsNamedenergyUnit Named>
   class energy<Named> : public base_dimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename base_dimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires Isenergy<Other>
      constexpr energy(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class energy<Units...> : public base_dimension<
      unit_exponent<typename Extractor<massType, Units...>::type, 1>,
      unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
      unit_exponent<typename Extractor<timespanType, Units...>::type, -2>
   >
   {
   public:
      using Base = base_dimension<
         unit_exponent<typename Extractor<massType, Units...>::type, 1>,
         unit_exponent<typename Extractor<lengthType, Units...>::type, 2>,
         unit_exponent<typename Extractor<timespanType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires Isenergy<T>
      constexpr energy(const T& base) : Base(base) {}
   };

   template<Isenergy Dim>
   energy(Dim) -> 
   energy<
      DimExtractor<massType, Dim>,
      DimExtractor<lengthType, Dim>,
      DimExtractor<timespanType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ENERGY_IMPL_H