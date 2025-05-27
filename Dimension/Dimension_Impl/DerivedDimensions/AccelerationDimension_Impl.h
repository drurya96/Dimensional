#ifndef STATIC_DIMENSION_ACCELERATION_IMPL_H
#define STATIC_DIMENSION_ACCELERATION_IMPL_H

#include "../../BaseDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Acceleration unit
   template<typename T>
   concept IsNamedAccelerationUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsLengthUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Acceleration type
   template<typename T>
   concept IsAcceleration = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryLength, 1>, 
      UnitExponent<PrimaryTime, -2>
   >>;

   /// @brief Retrieves the value of a Acceleration object with specific units
   /// @tparam LengthUnit The Length unit used for all Length components of Acceleration
   /// @tparam TimeUnit The Time unit used for all Time components of Acceleration
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsAcceleration DimType>
   constexpr PrecisionType get_acceleration_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<LengthUnit, 1>,
         UnitExponent<TimeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named Acceleration object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedAccelerationUnit Named, IsAcceleration DimType>
   constexpr PrecisionType get_acceleration_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Acceleration;

   /// @brief Represents the default Acceleration
   template<>
   class Acceleration<> : public BaseDimension<
      UnitExponent<PrimaryLength, 1>,
      UnitExponent<PrimaryTime, -2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryLength, 1>,
         UnitExponent<PrimaryTime, -2>>;
      using Base::Base;

      explicit constexpr Acceleration(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsAcceleration<Other>
      constexpr Acceleration(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Acceleration units
   /// @tparam Named The named unit this Acceleration type is in terms of
   template<IsNamedAccelerationUnit Named>
   class Acceleration<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsAcceleration<Other>
      constexpr Acceleration(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Acceleration<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<LengthType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<LengthType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsAcceleration<T>
      constexpr Acceleration(const T& base) : Base(base) {}
   };

   template<IsAcceleration Dim>
   Acceleration(Dim) -> 
   Acceleration<
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ACCELERATION_IMPL_H