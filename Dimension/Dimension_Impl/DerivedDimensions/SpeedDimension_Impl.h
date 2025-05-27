#ifndef STATIC_DIMENSION_SPEED_IMPL_H
#define STATIC_DIMENSION_SPEED_IMPL_H

#include "../../BaseDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Speed unit
   template<typename T>
   concept IsNamedSpeedUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsLengthUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Speed type
   template<typename T>
   concept IsSpeed = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryLength, 1>, 
      UnitExponent<PrimaryTime, -1>
   >>;

   /// @brief Retrieves the value of a Speed object with specific units
   /// @tparam LengthUnit The Length unit used for all Length components of Speed
   /// @tparam TimeUnit The Time unit used for all Time components of Speed
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsSpeed DimType>
   constexpr PrecisionType get_speed_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<LengthUnit, 1>,
         UnitExponent<TimeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named Speed object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedSpeedUnit Named, IsSpeed DimType>
   constexpr PrecisionType get_speed_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Speed;

   /// @brief Represents the default Speed
   template<>
   class Speed<> : public BaseDimension<
      UnitExponent<PrimaryLength, 1>,
      UnitExponent<PrimaryTime, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryLength, 1>,
         UnitExponent<PrimaryTime, -1>>;
      using Base::Base;

      explicit constexpr Speed(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsSpeed<Other>
      constexpr Speed(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Speed units
   /// @tparam Named The named unit this Speed type is in terms of
   template<IsNamedSpeedUnit Named>
   class Speed<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsSpeed<Other>
      constexpr Speed(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Speed<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<LengthType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<LengthType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsSpeed<T>
      constexpr Speed(const T& base) : Base(base) {}
   };

   template<IsSpeed Dim>
   Speed(Dim) -> 
   Speed<
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_SPEED_IMPL_H