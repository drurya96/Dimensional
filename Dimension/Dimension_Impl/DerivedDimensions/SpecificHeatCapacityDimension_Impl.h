#ifndef STATIC_DIMENSION_SPECIFICHEATCAPACITY_IMPL_H
#define STATIC_DIMENSION_SPECIFICHEATCAPACITY_IMPL_H

#include "../../BaseDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"
#include "../../TemperatureDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named SpecificHeatCapacity unit
   template<typename T>
   concept IsNamedSpecificHeatCapacityUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         IsLengthUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsTemperatureUnit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a SpecificHeatCapacity type
   template<typename T>
   concept IsSpecificHeatCapacity = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryLength, 2>, 
      UnitExponent<PrimaryTime, -2>, 
      UnitExponent<PrimaryTemperature, -1>
   >>;

   /// @brief Retrieves the value of a SpecificHeatCapacity object with specific units
   /// @tparam LengthUnit The Length unit used for all Length components of SpecificHeatCapacity
   /// @tparam TimeUnit The Time unit used for all Time components of SpecificHeatCapacity
   /// @tparam TemperatureUnit The Temperature unit used for all Temperature components of SpecificHeatCapacity
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsTemperatureUnit TemperatureUnit,
      IsSpecificHeatCapacity DimType>
   constexpr PrecisionType get_specificheatcapacity_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<LengthUnit, 2>,
         UnitExponent<TimeUnit, -2>,
         UnitExponent<TemperatureUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named SpecificHeatCapacity object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedSpecificHeatCapacityUnit Named, IsSpecificHeatCapacity DimType>
   constexpr PrecisionType get_specificheatcapacity_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class SpecificHeatCapacity;

   /// @brief Represents the default SpecificHeatCapacity
   template<>
   class SpecificHeatCapacity<> : public BaseDimension<
      UnitExponent<PrimaryLength, 2>,
      UnitExponent<PrimaryTime, -2>,
      UnitExponent<PrimaryTemperature, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryLength, 2>,
         UnitExponent<PrimaryTime, -2>,
         UnitExponent<PrimaryTemperature, -1>>;
      using Base::Base;

      explicit constexpr SpecificHeatCapacity(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsSpecificHeatCapacity<Other>
      constexpr SpecificHeatCapacity(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named SpecificHeatCapacity units
   /// @tparam Named The named unit this SpecificHeatCapacity type is in terms of
   template<IsNamedSpecificHeatCapacityUnit Named>
   class SpecificHeatCapacity<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsSpecificHeatCapacity<Other>
      constexpr SpecificHeatCapacity(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class SpecificHeatCapacity<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -2>,
      UnitExponent<typename Extractor<TemperatureType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -2>,
         UnitExponent<typename Extractor<TemperatureType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsSpecificHeatCapacity<T>
      constexpr SpecificHeatCapacity(const T& base) : Base(base) {}
   };

   template<IsSpecificHeatCapacity Dim>
   SpecificHeatCapacity(Dim) -> 
   SpecificHeatCapacity<
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>,
      DimExtractor<TemperatureType, Dim>
   >;
}

#endif // STATIC_DIMENSION_SPECIFICHEATCAPACITY_IMPL_H