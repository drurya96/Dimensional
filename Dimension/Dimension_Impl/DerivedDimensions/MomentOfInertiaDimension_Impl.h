#ifndef STATIC_DIMENSION_MOMENTOFINERTIA_IMPL_H
#define STATIC_DIMENSION_MOMENTOFINERTIA_IMPL_H

#include "../../BaseDimension.h"
#include "../../MassDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named MomentOfInertia unit
   template<typename T>
   concept IsNamedMomentOfInertiaUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsMassUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsLengthUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a MomentOfInertia type
   template<typename T>
   concept IsMomentOfInertia = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryMass, 1>, 
      UnitExponent<PrimaryLength, 2>
   >>;

   /// @brief Retrieves the value of a MomentOfInertia object with specific units
   /// @tparam MassUnit The Mass unit used for all Mass components of MomentOfInertia
   /// @tparam LengthUnit The Length unit used for all Length components of MomentOfInertia
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsMassUnit MassUnit,
      IsLengthUnit LengthUnit,
      IsMomentOfInertia DimType>
   constexpr PrecisionType get_momentofinertia_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<MassUnit, 1>,
         UnitExponent<LengthUnit, 2>
      >(obj);
   }

   /// @brief Retrieves the value of a named MomentOfInertia object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedMomentOfInertiaUnit Named, IsMomentOfInertia DimType>
   constexpr PrecisionType get_momentofinertia_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class MomentOfInertia;

   /// @brief Represents the default MomentOfInertia
   template<>
   class MomentOfInertia<> : public BaseDimension<
      UnitExponent<PrimaryMass, 1>,
      UnitExponent<PrimaryLength, 2>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryMass, 1>,
         UnitExponent<PrimaryLength, 2>>;
      using Base::Base;

      explicit constexpr MomentOfInertia(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsMomentOfInertia<Other>
      constexpr MomentOfInertia(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named MomentOfInertia units
   /// @tparam Named The named unit this MomentOfInertia type is in terms of
   template<IsNamedMomentOfInertiaUnit Named>
   class MomentOfInertia<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsMomentOfInertia<Other>
      constexpr MomentOfInertia(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class MomentOfInertia<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
      UnitExponent<typename Extractor<LengthType, Units...>::type, 2>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<MassType, Units...>::type, 1>,
         UnitExponent<typename Extractor<LengthType, Units...>::type, 2>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsMomentOfInertia<T>
      constexpr MomentOfInertia(const T& base) : Base(base) {}
   };

   template<IsMomentOfInertia Dim>
   MomentOfInertia(Dim) -> 
   MomentOfInertia<
      DimExtractor<MassType, Dim>,
      DimExtractor<LengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_MOMENTOFINERTIA_IMPL_H