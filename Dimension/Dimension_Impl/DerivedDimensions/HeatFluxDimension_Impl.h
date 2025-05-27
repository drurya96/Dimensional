#ifndef STATIC_DIMENSION_HEATFLUX_IMPL_H
#define STATIC_DIMENSION_HEATFLUX_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named HeatFlux unit
   template<typename T>
   concept IsNamedHeatFluxUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a HeatFlux type
   template<typename T>
   concept IsHeatFlux = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryTime, -3>
   >>;

   /// @brief Retrieves the value of a HeatFlux object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of HeatFlux
   /// @tparam TimeUnit The Time unit used for all Time components of HeatFlux
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsTimeUnit TimeUnit,
      IsHeatFlux DimType>
   constexpr PrecisionType get_heatflux_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<TimeUnit, -3>
      >(obj);
   }

   /// @brief Retrieves the value of a named HeatFlux object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedHeatFluxUnit Named, IsHeatFlux DimType>
   constexpr PrecisionType get_heatflux_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class HeatFlux;

   /// @brief Represents the default HeatFlux
   template<>
   class HeatFlux<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryTime, -3>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryTime, -3>>;
      using Base::Base;

      explicit constexpr HeatFlux(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsHeatFlux<Other>
      constexpr HeatFlux(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named HeatFlux units
   /// @tparam Named The named unit this HeatFlux type is in terms of
   template<IsNamedHeatFluxUnit Named>
   class HeatFlux<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsHeatFlux<Other>
      constexpr HeatFlux(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class HeatFlux<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -3>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -3>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsHeatFlux<T>
      constexpr HeatFlux(const T& base) : Base(base) {}
   };

   template<IsHeatFlux Dim>
   HeatFlux(Dim) -> 
   HeatFlux<
      DimExtractor<MassType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_HEATFLUX_IMPL_H