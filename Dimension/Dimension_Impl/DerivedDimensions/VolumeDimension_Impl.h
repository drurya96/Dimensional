#ifndef STATIC_DIMENSION_VOLUME_IMPL_H
#define STATIC_DIMENSION_VOLUME_IMPL_H

#include "../../BaseDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept to verify a type can serve as a named Volume unit
   template<typename T>
   concept IsNamedVolumeUnit = requires {
      typename T::units;
      requires 
         std::tuple_size_v<typename T::units> == 1 &&
         IsLengthUnit<typename std::tuple_element_t<0, typename T::units>::unit>;
      requires !std::is_base_of_v<FundamentalUnitTag, T>;
   };

   /// @brief Concept to verify a dimension can be treated as a Volume type
   template<typename T>
   concept IsVolume = std::is_convertible_v<T, BaseDimension<
      UnitExponent<PrimaryLength, 3>
   >>;

   /// @brief Retrieves the value of a Volume object with specific units
   /// @tparam LengthUnit The Length unit used for all Length components of Volume
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<
      IsLengthUnit LengthUnit,
      IsVolume DimType>
   constexpr PrecisionType get_volume_as(const DimType& obj)
   {
      return get_dimension_as<
         UnitExponent<LengthUnit, 3>
      >(obj);
   }

   /// @brief Retrieves the value of a named Volume object.
   /// @tparam Named The named unit to extract in terms of
   /// @tparam DimType The dimension object type, deduced
   /// @param obj The dimension to extract a raw value from
   /// @return The raw value in terms of template units as a PrecisionType
   template<IsNamedVolumeUnit Named, IsVolume DimType>
   constexpr PrecisionType get_volume_as(const DimType& obj)
   {
      return call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(obj); });
   }

   template<typename... Ts>
   class Volume;

   /// @brief Represents the default Volume
   template<>
   class Volume<> : public BaseDimension<
      UnitExponent<PrimaryLength, 3>>
   {
   public:
      using Base = BaseDimension<
         UnitExponent<PrimaryLength, 3>>;
      using Base::Base;

      explicit constexpr Volume(PrecisionType val) : Base(val) {}

      template<typename Other>
      requires IsVolume<Other>
      constexpr Volume(const Other& base)
         : Base(call_unpack<typename Base::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };

   /// @brief Template specialization for named Volume units
   /// @tparam Named The named unit this Volume type is in terms of
   template<IsNamedVolumeUnit Named>
   class Volume<Named> : public BaseDimensionFromTuple<typename Named::units>::dim
   {
   public:
      using Base = typename BaseDimensionFromTuple<typename Named::units>::dim;
      using Base::Base;

      template<typename Other>
      requires IsVolume<Other>
      constexpr Volume(const Other& base)
         : Base(call_unpack<typename Named::units>([&]<typename... Units> { return get_dimension_as<Units...>(base); })) {}
   };


   template<typename... Units>
   class Volume<Units...> : public BaseDimension<
      UnitExponent<typename Extractor<LengthType, Units...>::type, 3>
   >
   {
   public:
      using Base = BaseDimension<
         UnitExponent<typename Extractor<LengthType, Units...>::type, 3>
      >;
   
      using Base::Base;
   
      template<typename T>
      requires IsVolume<T>
      constexpr Volume(const T& base) : Base(base) {}
   };

   template<IsVolume Dim>
   Volume(Dim) -> 
   Volume<
      DimExtractor<LengthType, Dim>
   >;
}

#endif // STATIC_DIMENSION_VOLUME_IMPL_H