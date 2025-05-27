#ifndef STATIC_DIMENSION_DYNAMICVISCOSITY_IMPL_H
#define STATIC_DIMENSION_DYNAMICVISCOSITY_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../TimeDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named DynamicViscosity unit
   template<typename T>
   concept IsNamedDynamicViscosityUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a DynamicViscosity type
   template<typename T>
   concept IsDynamicViscosity = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryTime, -1>, 
      UnitExponent<PrimaryLength, -1>
   >>;

   /// @brief Retrieves the value of a DynamicViscosity object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of DynamicViscosity
   /// @tparam TimeUnit The Time unit used for all Time components of DynamicViscosity
   /// @tparam LengthUnit The Length unit used for all Length components of DynamicViscosity
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsTimeUnit TimeUnit,
      IsLengthUnit LengthUnit,
      IsDynamicViscosity DimType>
   constexpr PrecisionType get_dynamicviscosity_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<TimeUnit, -1>,
         UnitExponent<LengthUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named DynamicViscosity object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedDynamicViscosityUnit Named, IsDynamicViscosity DimType>
   constexpr PrecisionType get_dynamicviscosity_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class DynamicViscosity;

   /// @brief Represents the default DynamicViscosity
   template<>
   class DynamicViscosity<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryTime, -1>,
      UnitExponent<PrimaryLength, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryTime, -1>,
         UnitExponent<PrimaryLength, -1>>;
      using Base::Base;

      explicit constexpr DynamicViscosity(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsDynamicViscosity<Other>
      constexpr DynamicViscosity(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named DynamicViscosity units
   /// @tparam Named The named unit this DynamicViscosity type is in terms of
   template<IsNamedDynamicViscosityUnit Named>
   class DynamicViscosity<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsDynamicViscosity<Other>
      constexpr DynamicViscosity(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class DynamicViscosity<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsDynamicViscosity<T>
      constexpr DynamicViscosity(const T& base) : Base(base) {}
   };

   template<IsDynamicViscosity Dim>
   DynamicViscosity(Dim) -> 
   DynamicViscosity<
      DimExtractor<MassType, Dim>,
      DimExtractor<TimeType, Dim>,
      DimExtractor<LengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_DYNAMICVISCOSITY_IMPL_H