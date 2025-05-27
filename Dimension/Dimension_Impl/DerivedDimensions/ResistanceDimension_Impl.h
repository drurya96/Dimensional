#ifndef STATIC_DIMENSION_RESISTANCE_IMPL_H
#define STATIC_DIMENSION_RESISTANCE_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"
#include "../../ChargeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Resistance unit
   template<typename T>
   concept IsNamedResistanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         IsChargeUnit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Resistance type
   template<typename T>
   concept IsResistance = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, 2>, 
      UnitExponent<PrimaryTime, -1>, 
      UnitExponent<PrimaryCharge, -2>
   >>;

   /// @brief Retrieves the value of a Resistance object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of Resistance
   /// @tparam LengthUnit The Length unit used for all Length components of Resistance
   /// @tparam TimeUnit The Time unit used for all Time components of Resistance
   /// @tparam ChargeUnit The Charge unit used for all Charge components of Resistance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsChargeUnit ChargeUnit,
      IsResistance DimType>
   constexpr PrecisionType get_resistance_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, 2>,
         UnitExponent<TimeUnit, -1>,
         UnitExponent<ChargeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named Resistance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedResistanceUnit Named, IsResistance DimType>
   constexpr PrecisionType get_resistance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Resistance;

   /// @brief Represents the default Resistance
   template<>
   class Resistance<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, 2>,
      UnitExponent<PrimaryTime, -1>,
      UnitExponent<PrimaryCharge, -2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, 2>,
         UnitExponent<PrimaryTime, -1>,
         UnitExponent<PrimaryCharge, -2>>;
      using Base::Base;

      explicit constexpr Resistance(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsResistance<Other>
      constexpr Resistance(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Resistance units
   /// @tparam Named The named unit this Resistance type is in terms of
   template<IsNamedResistanceUnit Named>
   class Resistance<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsResistance<Other>
      constexpr Resistance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Resistance<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>,
      UnitExponent<typename Extractor<ChargeType, Units...>::type, -2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>,
         UnitExponent<typename Extractor<ChargeType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsResistance<T>
      constexpr Resistance(const T& base) : Base(base) {}
   };

   template<IsResistance Dim>
   Resistance(Dim) -> 
   Resistance<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>,
      DimExtractor<ChargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_RESISTANCE_IMPL_H