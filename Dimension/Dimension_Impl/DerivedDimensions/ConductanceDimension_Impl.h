#ifndef STATIC_DIMENSION_CONDUCTANCE_IMPL_H
#define STATIC_DIMENSION_CONDUCTANCE_IMPL_H

#include "../../BaseDimension.h"
#include "../../TimeDimension.h"
#include "../../ChargeDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Conductance unit
   template<typename T>
   concept IsNamedConductanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 4 &&
         IsTimeUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsChargeUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsMassUnit<typename std::tuple_element_t<2, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<3, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Conductance type
   template<typename T>
   concept IsConductance = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryTime, 1>, 
      UnitExponent<PrimaryCharge, 2>, 
      UnitExponent<PrimaryMass, -1>, 
      UnitExponent<PrimaryLength, -2>
   >>;

   /// @brief Retrieves the value of a Conductance object with specific units
   /// @tparam TimeUnit The Time unit used for all Time components of Conductance
   /// @tparam ChargeUnit The Charge unit used for all Charge components of Conductance
   /// @tparam MassUnit The Mass unit used for all Mass components of Conductance
   /// @tparam LengthUnit The Length unit used for all Length components of Conductance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsTimeUnit TimeUnit,
      IsChargeUnit ChargeUnit,
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsConductance DimType>
   constexpr PrecisionType get_conductance_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<TimeUnit, 1>,
         UnitExponent<ChargeUnit, 2>,
         UnitExponent<MassUnit, -1>,
         UnitExponent<LengthUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named Conductance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedConductanceUnit Named, IsConductance DimType>
   constexpr PrecisionType get_conductance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Conductance;

   /// @brief Represents the default Conductance
   template<>
   class Conductance<> : public BaseDimension<
      UnitExponent<PrimaryTime, 1>,
      UnitExponent<PrimaryCharge, 2>,
      UnitExponent<PrimaryMass, -1>,
      UnitExponent<PrimaryLength, -2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryTime, 1>,
         UnitExponent<PrimaryCharge, 2>,
         UnitExponent<PrimaryMass, -1>,
         UnitExponent<PrimaryLength, -2>>;
      using Base::Base;

      explicit constexpr Conductance(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsConductance<Other>
      constexpr Conductance(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Conductance units
   /// @tparam Named The named unit this Conductance type is in terms of
   template<IsNamedConductanceUnit Named>
   class Conductance<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsConductance<Other>
      constexpr Conductance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Conductance<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<TimeType, Units...>::type, 1>,
      UnitExponent<typename Extractor<ChargeType, Units...>::type, 2>,
      UnitExponent<typename Extractor<MassType, Units...>::type, -1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, -2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<TimeType, Units...>::type, 1>,
         UnitExponent<typename Extractor<ChargeType, Units...>::type, 2>,
         UnitExponent<typename Extractor<MassType, Units...>::type, -1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsConductance<T>
      constexpr Conductance(const T& base) : Base(base) {}
   };

   template<IsConductance Dim>
   Conductance(Dim) -> 
   Conductance<
      DimExtractor<TimeType, Dim>,
      DimExtractor<ChargeType, Dim>,
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_CONDUCTANCE_IMPL_H