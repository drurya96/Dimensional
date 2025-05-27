#ifndef STATIC_DIMENSION_SPECIFICVOLUME_IMPL_H
#define STATIC_DIMENSION_SPECIFICVOLUME_IMPL_H

#include "../../BaseDimension.h"
#include "../../LengthDimension.h"
#include "../../MassDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named SpecificVolume unit
   template<typename T>
   concept IsNamedSpecificVolumeUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 2 &&
         IsLengthUnit<typename std::tuple_element_t<0, typename T::units>::unit> &&
         IsMassUnit<typename std::tuple_element_t<1, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a SpecificVolume type
   template<typename T>
   concept IsSpecificVolume = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryLength, 3>, 
      UnitExponent<PrimaryMass, -1>
   >>;

   /// @brief Retrieves the value of a SpecificVolume object with specific units
   /// @tparam LengthUnit The Length unit used for all Length components of SpecificVolume
   /// @tparam MassUnit The Mass unit used for all Mass components of SpecificVolume
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsLengthUnit LengthUnit,
      IsMassUnit MassUnit,
      IsSpecificVolume DimType>
   constexpr PrecisionType get_specificvolume_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<LengthUnit, 3>,
         UnitExponent<MassUnit, -1>
      >(obj);
   }

   /// @brief Retrieves the value of a named SpecificVolume object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedSpecificVolumeUnit Named, IsSpecificVolume DimType>
   constexpr PrecisionType get_specificvolume_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class SpecificVolume;

   /// @brief Represents the default SpecificVolume
   template<>
   class SpecificVolume<> : public BaseDimension<
      UnitExponent<PrimaryLength, 3>,
      UnitExponent<PrimaryMass, -1>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryLength, 3>,
         UnitExponent<PrimaryMass, -1>>;
      using Base::Base;

      explicit constexpr SpecificVolume(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsSpecificVolume<Other>
      constexpr SpecificVolume(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named SpecificVolume units
   /// @tparam Named The named unit this SpecificVolume type is in terms of
   template<IsNamedSpecificVolumeUnit Named>
   class SpecificVolume<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsSpecificVolume<Other>
      constexpr SpecificVolume(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class SpecificVolume<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<LengthType, Units...>::type, 3>,
      UnitExponent<typename Extractor<MassType, Units...>::type, -1>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<LengthType, Units...>::type, 3>,
         UnitExponent<typename Extractor<MassType, Units...>::type, -1>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsSpecificVolume<T>
      constexpr SpecificVolume(const T& base) : Base(base) {}
   };

   template<IsSpecificVolume Dim>
   SpecificVolume(Dim) -> 
   SpecificVolume<
      DimExtractor<LengthType, Dim>,
      DimExtractor<MassType, Dim>
   >;
}

#endif // STATIC_DIMENSION_SPECIFICVOLUME_IMPL_H