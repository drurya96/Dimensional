#ifndef STATIC_DIMENSION_TORQUE_IMPL_H
#define STATIC_DIMENSION_TORQUE_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"
#include "../../AngleDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Torque unit
   template<typename T>
   concept IsNamedTorqueUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         IsAngleUnit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Torque type
   template<typename T>
   concept IsTorque = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, 2>, 
      UnitExponent<PrimaryTime, -2>, 
      UnitExponent<PrimaryAngle, -1>
   >>;

   /// @brief Retrieves the value of a Torque object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of Torque
   /// @tparam LengthUnit The Length unit used for all Length components of Torque
   /// @tparam TimeUnit The Time unit used for all Time components of Torque
   /// @tparam AngleUnit The Angle unit used for all Angle components of Torque
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsAngleUnit AngleUnit,
      IsTorque DimType>
   constexpr PrecisionType get_torque_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, 2>,
         UnitExponent<TimeUnit, -2>,
         UnitExponent<AngleUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named Torque object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedTorqueUnit Named, IsTorque DimType>
   constexpr PrecisionType get_torque_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Torque;

   /// @brief Represents the default Torque
   template<>
   class Torque<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, 2>,
      UnitExponent<PrimaryTime, -2>,
      UnitExponent<PrimaryAngle, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, 2>,
         UnitExponent<PrimaryTime, -2>,
         UnitExponent<PrimaryAngle, -1>>;
      using Base::Base;

      explicit constexpr Torque(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsTorque<Other>
      constexpr Torque(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Torque units
   /// @tparam Named The named unit this Torque type is in terms of
   template<IsNamedTorqueUnit Named>
   class Torque<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsTorque<Other>
      constexpr Torque(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Torque<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -2>,
      UnitExponent<typename Extractor<AngleType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -2>,
         UnitExponent<typename Extractor<AngleType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsTorque<T>
      constexpr Torque(const T& base) : Base(base) {}
   };

   template<IsTorque Dim>
   Torque(Dim) -> 
   Torque<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>,
      DimExtractor<AngleType, Dim>
   >;
}

#endif // STATIC_DIMENSION_TORQUE_IMPL_H