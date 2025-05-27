#ifndef STATIC_DIMENSION_ELECTRICPOTENTIAL_IMPL_H
#define STATIC_DIMENSION_ELECTRICPOTENTIAL_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"
#include "../../ChargeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named ElectricPotential unit
   template<typename T>
   concept IsNamedElectricPotentialUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         IsChargeUnit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a ElectricPotential type
   template<typename T>
   concept IsElectricPotential = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, 2>, 
      UnitExponent<PrimaryTime, -2>, 
      UnitExponent<PrimaryCharge, -1>
   >>;

   /// @brief Retrieves the value of a ElectricPotential object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of ElectricPotential
   /// @tparam LengthUnit The Length unit used for all Length components of ElectricPotential
   /// @tparam TimeUnit The Time unit used for all Time components of ElectricPotential
   /// @tparam ChargeUnit The Charge unit used for all Charge components of ElectricPotential
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsChargeUnit ChargeUnit,
      IsElectricPotential DimType>
   constexpr PrecisionType get_electricpotential_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, 2>,
         UnitExponent<TimeUnit, -2>,
         UnitExponent<ChargeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named ElectricPotential object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedElectricPotentialUnit Named, IsElectricPotential DimType>
   constexpr PrecisionType get_electricpotential_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class ElectricPotential;

   /// @brief Represents the default ElectricPotential
   template<>
   class ElectricPotential<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, 2>,
      UnitExponent<PrimaryTime, -2>,
      UnitExponent<PrimaryCharge, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, 2>,
         UnitExponent<PrimaryTime, -2>,
         UnitExponent<PrimaryCharge, -1>>;
      using Base::Base;

      explicit constexpr ElectricPotential(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsElectricPotential<Other>
      constexpr ElectricPotential(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named ElectricPotential units
   /// @tparam Named The named unit this ElectricPotential type is in terms of
   template<IsNamedElectricPotentialUnit Named>
   class ElectricPotential<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsElectricPotential<Other>
      constexpr ElectricPotential(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class ElectricPotential<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -2>,
      UnitExponent<typename Extractor<ChargeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -2>,
         UnitExponent<typename Extractor<ChargeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsElectricPotential<T>
      constexpr ElectricPotential(const T& base) : Base(base) {}
   };

   template<IsElectricPotential Dim>
   ElectricPotential(Dim) -> 
   ElectricPotential<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>,
      DimExtractor<ChargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_ELECTRICPOTENTIAL_IMPL_H