#ifndef STATIC_DIMENSION_CAPACITANCE_IMPL_H
#define STATIC_DIMENSION_CAPACITANCE_IMPL_H

#include "../../BaseDimension.h"
#include "../../ChargeDimension.h"
#include "../../TimeDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Capacitance unit
   template<typename T>
   concept IsNamedCapacitanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         IsChargeUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsMassUnit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Capacitance type
   template<typename T>
   concept IsCapacitance = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryCharge, 2>, 
      UnitExponent<PrimaryTime, 2>, 
      UnitExponent<PrimaryMass, -1>, 
      UnitExponent<PrimaryLength, -2>
   >>;

   /// @brief Retrieves the value of a Capacitance object with specific units
   /// @tparam ChargeUnit The Charge unit used for all Charge components of Capacitance
   /// @tparam TimeUnit The Time unit used for all Time components of Capacitance
   /// @tparam MassUnit The Mass unit used for all Mass components of Capacitance
   /// @tparam LengthUnit The Length unit used for all Length components of Capacitance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsChargeUnit ChargeUnit,
      IsTimeUnit TimeUnit,
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsCapacitance DimType>
   constexpr PrecisionType get_capacitance_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<ChargeUnit, 2>,
         UnitExponent<TimeUnit, 2>,
         UnitExponent<MassUnit, -1>,
         UnitExponent<LengthUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named Capacitance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedCapacitanceUnit Named, IsCapacitance DimType>
   constexpr PrecisionType get_capacitance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Capacitance;

   /// @brief Represents the default Capacitance
   template<>
   class Capacitance<> : public BaseDimension<
      UnitExponent<PrimaryCharge, 2>,
      UnitExponent<PrimaryTime, 2>,
      UnitExponent<PrimaryMass, -1>,
      UnitExponent<PrimaryLength, -2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryCharge, 2>,
         UnitExponent<PrimaryTime, 2>,
         UnitExponent<PrimaryMass, -1>,
         UnitExponent<PrimaryLength, -2>>;
      using Base::Base;

      explicit constexpr Capacitance(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsCapacitance<Other>
      constexpr Capacitance(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Capacitance units
   /// @tparam Named The named unit this Capacitance type is in terms of
   template<IsNamedCapacitanceUnit Named>
   class Capacitance<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsCapacitance<Other>
      constexpr Capacitance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Capacitance<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<ChargeType, Units...>::type, 2>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, 2>,
      UnitExponent<typename Extractor<MassType, Units...>::type, -1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, -2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<ChargeType, Units...>::type, 2>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, 2>,
         UnitExponent<typename Extractor<MassType, Units...>::type, -1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsCapacitance<T>
      constexpr Capacitance(const T& base) : Base(base) {}
   };

   template<IsCapacitance Dim>
   Capacitance(Dim) -> 
   Capacitance<
      DimExtractor<ChargeType, Dim>,
      DimExtractor<TimeType, Dim>,
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_CAPACITANCE_IMPL_H