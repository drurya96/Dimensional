#ifndef STATIC_DIMENSION_FORCE_IMPL_H
#define STATIC_DIMENSION_FORCE_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Force unit
   template<typename T>
   concept IsNamedForceUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 3 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit> &&
         IsTimeUnit<typename std::tuple_element_t<2, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Force type
   template<typename T>
   concept IsForce = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, 1>, 
      UnitExponent<PrimaryTime, -2>
   >>;

   /// @brief Retrieves the value of a Force object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of Force
   /// @tparam LengthUnit The Length unit used for all Length components of Force
   /// @tparam TimeUnit The Time unit used for all Time components of Force
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsTimeUnit TimeUnit,
      IsForce DimType>
   constexpr PrecisionType get_force_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, 1>,
         UnitExponent<TimeUnit, -2>
      >(obj);
   }

   /// @brief Retrieves the value of a named Force object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedForceUnit Named, IsForce DimType>
   constexpr PrecisionType get_force_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Force;

   /// @brief Represents the default Force
   template<>
   class Force<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, 1>,
      UnitExponent<PrimaryTime, -2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, 1>,
         UnitExponent<PrimaryTime, -2>>;
      using Base::Base;

      explicit constexpr Force(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsForce<Other>
      constexpr Force(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Force units
   /// @tparam Named The named unit this Force type is in terms of
   template<IsNamedForceUnit Named>
   class Force<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsForce<Other>
      constexpr Force(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Force<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, 1>,
      UnitExponent<typename Extractor<TimeType, Units...>::type, -2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, 1>,
         UnitExponent<typename Extractor<TimeType, Units...>::type, -2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsForce<T>
      constexpr Force(const T& base) : Base(base) {}
   };

   template<IsForce Dim>
   Force(Dim) -> 
   Force<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>,
      DimExtractor<TimeType, Dim>
   >;
}

#endif // STATIC_DIMENSION_FORCE_IMPL_H