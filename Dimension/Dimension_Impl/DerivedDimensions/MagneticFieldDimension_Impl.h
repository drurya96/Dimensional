#ifndef STATIC_DIMENSION_MAGNETICFIELD_IMPL_H
#define STATIC_DIMENSION_MAGNETICFIELD_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../TimeDimension.h"
#include "../../ChargeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named MagneticField unit
   template<typename T>
   concept IsNamedMagneticFieldUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsChargeUnit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a MagneticField type
   template<typename T>
   concept IsMagneticField = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryTime, -1>, 
      UnitExponent<PrimaryCharge, -1>
   >>;

   /// @brief Retrieves the value of a MagneticField object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of MagneticField
   /// @tparam TimeUnit The Time unit used for all Time components of MagneticField
   /// @tparam ChargeUnit The Charge unit used for all Charge components of MagneticField
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsTimeUnit TimeUnit,
      IsChargeUnit ChargeUnit,
      IsMagneticField DimType>
   constexpr PrecisionType get_magneticfield_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<TimeUnit, -1>,
         UnitExponent<ChargeUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named MagneticField object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedMagneticFieldUnit Named, IsMagneticField DimType>
   constexpr PrecisionType get_magneticfield_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class MagneticField;

   /// @brief Represents the default MagneticField
   template<>
   class MagneticField<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryTime, -1>,
      UnitExponent<PrimaryCharge, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryTime, -1>,
         UnitExponent<PrimaryCharge, -1>>;
      using Base::Base;

      explicit constexpr MagneticField(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsMagneticField<Other>
      constexpr MagneticField(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named MagneticField units
   /// @tparam Named The named unit this MagneticField type is in terms of
   template<IsNamedMagneticFieldUnit Named>
   class MagneticField<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsMagneticField<Other>
      constexpr MagneticField(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class MagneticField<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>,
      UnitExponent<typename Extractor<ChargeType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>,
         UnitExponent<typename Extractor<ChargeType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsMagneticField<T>
      constexpr MagneticField(const T& base) : Base(base) {}
   };

   template<IsMagneticField Dim>
   MagneticField(Dim) -> 
   MagneticField<
      DimExtractor<MassType, Dim>,
      DimExtractor<TimeType, Dim>,
      DimExtractor<ChargeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MAGNETICFIELD_IMPL_H