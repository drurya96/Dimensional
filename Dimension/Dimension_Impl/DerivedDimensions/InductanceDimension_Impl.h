#ifndef STATIC_DIMENSION_INDUCTANCE_IMPL_H
#define STATIC_DIMENSION_INDUCTANCE_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../ChargeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Inductance unit
   template<typename T>
   concept IsNamedInductanceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsChargeUnit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Inductance type
   template<typename T>
   concept IsInductance = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, 2>, 
      UnitExponent<PrimaryCharge, -2>
   >>;

   /// @brief Retrieves the value of a Inductance object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of Inductance
   /// @tparam LengthUnit The Length unit used for all Length components of Inductance
   /// @tparam ChargeUnit The Charge unit used for all Charge components of Inductance
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsChargeUnit ChargeUnit,
      IsInductance DimType>
   constexpr PrecisionType get_inductance_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, 2>,
         UnitExponent<ChargeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named Inductance object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedInductanceUnit Named, IsInductance DimType>
   constexpr PrecisionType get_inductance_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Inductance;

   /// @brief Represents the default Inductance
   template<>
   class Inductance<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, 2>,
      UnitExponent<PrimaryCharge, -2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, 2>,
         UnitExponent<PrimaryCharge, -2>>;
      using Base::Base;

      explicit constexpr Inductance(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsInductance<Other>
      constexpr Inductance(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Inductance units
   /// @tparam Named The named unit this Inductance type is in terms of
   template<IsNamedInductanceUnit Named>
   class Inductance<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsInductance<Other>
      constexpr Inductance(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Inductance<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
      UnitExponent<typename Extractor<ChargeType, Units...>::type, -2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>,
         UnitExponent<typename Extractor<ChargeType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsInductance<T>
      constexpr Inductance(const T& base) : Base(base) {}
   };

   template<IsInductance Dim>
   Inductance(Dim) -> 
   Inductance<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>,
      DimExtractor<ChargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_INDUCTANCE_IMPL_H