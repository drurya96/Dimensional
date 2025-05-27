#ifndef STATIC_DIMENSION_ANGULARACCELERATION_IMPL_H
#define STATIC_DIMENSION_ANGULARACCELERATION_IMPL_H

#include "../../BaseDimension.h"
#include "../../AngleDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named AngularAcceleration unit
   template<typename T>
   concept IsNamedAngularAccelerationUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsAngleUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a AngularAcceleration type
   template<typename T>
   concept IsAngularAcceleration = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryAngle, 1>, 
      UnitExponent<PrimaryTime, -2>
   >>;

   /// @brief Retrieves the value of a AngularAcceleration object with specific units
   /// @tparam AngleUnit The Angle unit used for all Angle components of AngularAcceleration
   /// @tparam TimeUnit The Time unit used for all Time components of AngularAcceleration
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsAngleUnit AngleUnit,
      IsTimeUnit TimeUnit,
      IsAngularAcceleration DimType>
   constexpr PrecisionType get_angularacceleration_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<AngleUnit, 1>,
         UnitExponent<TimeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named AngularAcceleration object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedAngularAccelerationUnit Named, IsAngularAcceleration DimType>
   constexpr PrecisionType get_angularacceleration_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class AngularAcceleration;

   /// @brief Represents the default AngularAcceleration
   template<>
   class AngularAcceleration<> : public BaseDimension<
      UnitExponent<PrimaryAngle, 1>,
      UnitExponent<PrimaryTime, -2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryAngle, 1>,
         UnitExponent<PrimaryTime, -2>>;
      using Base::Base;

      explicit constexpr AngularAcceleration(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsAngularAcceleration<Other>
      constexpr AngularAcceleration(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named AngularAcceleration units
   /// @tparam Named The named unit this AngularAcceleration type is in terms of
   template<IsNamedAngularAccelerationUnit Named>
   class AngularAcceleration<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsAngularAcceleration<Other>
      constexpr AngularAcceleration(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class AngularAcceleration<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<AngleType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<AngleType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsAngularAcceleration<T>
      constexpr AngularAcceleration(const T& base) : Base(base) {}
   };

   template<IsAngularAcceleration Dim>
   AngularAcceleration(Dim) -> 
   AngularAcceleration<
      DimExtractor<AngleType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ANGULARACCELERATION_IMPL_H