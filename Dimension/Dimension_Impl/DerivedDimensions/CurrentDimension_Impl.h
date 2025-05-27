#ifndef STATIC_DIMENSION_CURRENT_IMPL_H
#define STATIC_DIMENSION_CURRENT_IMPL_H

#include "../../BaseDimension.h"
#include "../../ChargeDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Current unit
   template<typename T>
   concept IsNamedCurrentUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsChargeUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Current type
   template<typename T>
   concept IsCurrent = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryCharge, 1>, 
      UnitExponent<PrimaryTime, -1>
   >>;

   /// @brief Retrieves the value of a Current object with specific units
   /// @tparam ChargeUnit The Charge unit used for all Charge components of Current
   /// @tparam TimeUnit The Time unit used for all Time components of Current
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsChargeUnit ChargeUnit,
      IsTimeUnit TimeUnit,
      IsCurrent DimType>
   constexpr PrecisionType get_current_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<ChargeUnit, 1>,
         UnitExponent<TimeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named Current object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedCurrentUnit Named, IsCurrent DimType>
   constexpr PrecisionType get_current_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Current;

   /// @brief Represents the default Current
   template<>
   class Current<> : public BaseDimension<
      UnitExponent<PrimaryCharge, 1>,
      UnitExponent<PrimaryTime, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryCharge, 1>,
         UnitExponent<PrimaryTime, -1>>;
      using Base::Base;

      explicit constexpr Current(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsCurrent<Other>
      constexpr Current(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Current units
   /// @tparam Named The named unit this Current type is in terms of
   template<IsNamedCurrentUnit Named>
   class Current<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsCurrent<Other>
      constexpr Current(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Current<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<ChargeType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<ChargeType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsCurrent<T>
      constexpr Current(const T& base) : Base(base) {}
   };

   template<IsCurrent Dim>
   Current(Dim) -> 
   Current<
      DimExtractor<ChargeType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_CURRENT_IMPL_H