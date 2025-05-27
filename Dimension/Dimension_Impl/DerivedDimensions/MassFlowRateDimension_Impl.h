#ifndef STATIC_DIMENSION_MASSFLOWRATE_IMPL_H
#define STATIC_DIMENSION_MASSFLOWRATE_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named MassFlowRate unit
   template<typename T>
   concept IsNamedMassFlowRateUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a MassFlowRate type
   template<typename T>
   concept IsMassFlowRate = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryTime, -1>
   >>;

   /// @brief Retrieves the value of a MassFlowRate object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of MassFlowRate
   /// @tparam TimeUnit The Time unit used for all Time components of MassFlowRate
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsTimeUnit TimeUnit,
      IsMassFlowRate DimType>
   constexpr PrecisionType get_massflowrate_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<TimeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named MassFlowRate object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedMassFlowRateUnit Named, IsMassFlowRate DimType>
   constexpr PrecisionType get_massflowrate_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class MassFlowRate;

   /// @brief Represents the default MassFlowRate
   template<>
   class MassFlowRate<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryTime, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryTime, -1>>;
      using Base::Base;

      explicit constexpr MassFlowRate(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsMassFlowRate<Other>
      constexpr MassFlowRate(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named MassFlowRate units
   /// @tparam Named The named unit this MassFlowRate type is in terms of
   template<IsNamedMassFlowRateUnit Named>
   class MassFlowRate<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsMassFlowRate<Other>
      constexpr MassFlowRate(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class MassFlowRate<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsMassFlowRate<T>
      constexpr MassFlowRate(const T& base) : Base(base) {}
   };

   template<IsMassFlowRate Dim>
   MassFlowRate(Dim) -> 
   MassFlowRate<
      DimExtractor<MassType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MASSFLOWRATE_IMPL_H