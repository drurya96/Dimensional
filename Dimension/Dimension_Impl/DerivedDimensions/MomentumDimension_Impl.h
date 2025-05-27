#ifndef STATIC_DIMENSION_MOMENTUM_IMPL_H
#define STATIC_DIMENSION_MOMENTUM_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Momentum unit
   template<typename T>
   concept IsNamedMomentumUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Momentum type
   template<typename T>
   concept IsMomentum = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, 1>, 
      UnitExponent<PrimaryTime, -1>
   >>;

   /// @brief Retrieves the value of a Momentum object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of Momentum
   /// @tparam LengthUnit The Length unit used for all Length components of Momentum
   /// @tparam TimeUnit The Time unit used for all Time components of Momentum
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsMomentum DimType>
   constexpr PrecisionType get_momentum_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, 1>,
         UnitExponent<TimeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named Momentum object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedMomentumUnit Named, IsMomentum DimType>
   constexpr PrecisionType get_momentum_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Momentum;

   /// @brief Represents the default Momentum
   template<>
   class Momentum<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, 1>,
      UnitExponent<PrimaryTime, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, 1>,
         UnitExponent<PrimaryTime, -1>>;
      using Base::Base;

      explicit constexpr Momentum(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsMomentum<Other>
      constexpr Momentum(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Momentum units
   /// @tparam Named The named unit this Momentum type is in terms of
   template<IsNamedMomentumUnit Named>
   class Momentum<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsMomentum<Other>
      constexpr Momentum(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Momentum<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsMomentum<T>
      constexpr Momentum(const T& base) : Base(base) {}
   };

   template<IsMomentum Dim>
   Momentum(Dim) -> 
   Momentum<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MOMENTUM_IMPL_H