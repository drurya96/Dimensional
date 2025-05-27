#ifndef STATIC_DIMENSION_ANGULARSPEED_IMPL_H
#define STATIC_DIMENSION_ANGULARSPEED_IMPL_H

#include "../../BaseDimension.h"
#include "../../AngleDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named AngularSpeed unit
   template<typename T>
   concept IsNamedAngularSpeedUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsAngleUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a AngularSpeed type
   template<typename T>
   concept IsAngularSpeed = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryAngle, 1>, 
      UnitExponent<PrimaryTime, -1>
   >>;

   /// @brief Retrieves the value of a AngularSpeed object with specific units
   /// @tparam AngleUnit The Angle unit used for all Angle components of AngularSpeed
   /// @tparam TimeUnit The Time unit used for all Time components of AngularSpeed
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsAngleUnit AngleUnit,
      IsTimeUnit TimeUnit,
      IsAngularSpeed DimType>
   constexpr PrecisionType get_angularspeed_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<AngleUnit, 1>,
         UnitExponent<TimeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named AngularSpeed object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedAngularSpeedUnit Named, IsAngularSpeed DimType>
   constexpr PrecisionType get_angularspeed_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class AngularSpeed;

   /// @brief Represents the default AngularSpeed
   template<>
   class AngularSpeed<> : public BaseDimension<
      UnitExponent<PrimaryAngle, 1>,
      UnitExponent<PrimaryTime, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryAngle, 1>,
         UnitExponent<PrimaryTime, -1>>;
      using Base::Base;

      explicit constexpr AngularSpeed(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsAngularSpeed<Other>
      constexpr AngularSpeed(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named AngularSpeed units
   /// @tparam Named The named unit this AngularSpeed type is in terms of
   template<IsNamedAngularSpeedUnit Named>
   class AngularSpeed<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsAngularSpeed<Other>
      constexpr AngularSpeed(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class AngularSpeed<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<AngleType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<AngleType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsAngularSpeed<T>
      constexpr AngularSpeed(const T& base) : Base(base) {}
   };

   template<IsAngularSpeed Dim>
   AngularSpeed(Dim) -> 
   AngularSpeed<
      DimExtractor<AngleType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ANGULARSPEED_IMPL_H