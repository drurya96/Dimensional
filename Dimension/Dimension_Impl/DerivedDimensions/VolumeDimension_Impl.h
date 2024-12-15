#ifndef STATIC_DIMENSION_VOLUME_IMPL_H
#define STATIC_DIMENSION_VOLUME_IMPL_H

#include "../../LengthDimension.h"

namespace Dimension
{

   // Concept for a named Volume unit (e.g., Liters)
   template<typename NamedVolume>
   concept IsNamedVolumeUnit = requires {
      typename NamedVolume::NumTuple;
      typename NamedVolume::DenTuple;
   };

   // Concept for a Volume dimension (basic units)
   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   concept IsVolumeUnits = 
      std::is_same_v<typename LengthUnit1::Dim, LengthType> &&
      std::is_same_v<typename LengthUnit2::Dim, LengthType> &&
      std::is_same_v<typename LengthUnit3::Dim, LengthType>;

   // General concept for a Volume type
   template<typename T>
   concept IsVolumeType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && IsVolumeUnits<typename std::tuple_element_t<0, typename T::NumTuple>,
                     typename std::tuple_element_t<1, typename T::NumTuple>,
                     typename std::tuple_element_t<2, typename T::NumTuple>>;

   // Free function for retrieving Volume values
   template<typename LengthT1, typename LengthT2, typename LengthT3, typename VolumeType>
   requires IsVolumeUnits<LengthT1, LengthT2, LengthT3> && IsVolumeType<VolumeType>
   PrecisionType getVolume(const VolumeType& obj)
   {
      return obj.template GetVal<std::tuple<LengthT1, LengthT2, LengthT3>, std::tuple<>>();
   }

   template<typename NamedVolume, typename VolumeType>
   requires IsNamedVolumeUnit<NamedVolume> && IsVolumeType<VolumeType>
   PrecisionType getVolume(const VolumeType& obj)
   {
      return obj.template GetVal<typename NamedVolume::NumTuple, typename NamedVolume::DenTuple>();
   }

   template<typename... Ts>
   class Volume;

   template<typename LengthT1, typename LengthT2, typename LengthT3>
   requires IsVolumeUnits<LengthT1, LengthT2, LengthT3>
   class Volume<LengthT1, LengthT2, LengthT3> : public BaseDimension<std::tuple<LengthT1, LengthT2, LengthT3>, std::tuple<>>
   {
   public:
      using Base = BaseDimension<std::tuple<LengthT1, LengthT2, LengthT3>, std::tuple<>>;
      using Base::Base;

      Volume(double val) : Base(val) {}

      template<typename NamedVolume>
      requires IsNamedVolumeUnit<NamedVolume>
      Volume(const NamedVolume& base) : Base(base) {}

      template<typename LengthUnitRet1, typename LengthUnitRet2, typename LengthUnitRet3>
      requires IsVolumeUnits<LengthUnitRet1, LengthUnitRet2, LengthUnitRet3>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<LengthUnitRet1, LengthUnitRet2, LengthUnitRet3>(*this);
      }

      template<typename NamedVolume>
      requires IsNamedVolumeUnit<NamedVolume>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<NamedVolume>(*this);
      }
   };

   // Volume class for named units
   template<typename NamedVolume>
   requires IsNamedVolumeUnit<NamedVolume>
   class Volume<NamedVolume> : public BaseDimension<typename NamedVolume::NumTuple, typename NamedVolume::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedVolume::NumTuple, typename NamedVolume::DenTuple>;
      using Base::Base;

      Volume(double val) : Base(val) {}

      template<typename OtherVolume>
      requires IsVolumeType<OtherVolume>
      Volume(const OtherVolume& base)
         : Base(base.template GetVal<typename NamedVolume::NumTuple, typename NamedVolume::DenTuple>()) {}

      template<typename LengthUnitRet1, typename LengthUnitRet2, typename LengthUnitRet3>
      requires IsVolumeUnits<LengthUnitRet1, LengthUnitRet2, LengthUnitRet3>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<LengthUnitRet1, LengthUnitRet2, LengthUnitRet3>(*this);
      }

      template<typename NamedVolumeUnit>
      requires IsNamedVolumeUnit<NamedVolumeUnit>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<NamedVolumeUnit>(*this);
      }
   };

   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   requires IsVolumeUnits<LengthUnit1, LengthUnit2, LengthUnit3>
   Volume(LengthUnit1, LengthUnit2, LengthUnit3) -> Volume<LengthUnit1, LengthUnit2, LengthUnit3>;

   template<typename NamedVolume>
   requires IsNamedVolumeUnit<NamedVolume>
   Volume(NamedVolume) -> Volume<NamedVolume>;

   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   requires IsVolumeUnits<LengthUnit1, LengthUnit2, LengthUnit3>
   Volume(BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>) -> Volume<LengthUnit1, LengthUnit2, LengthUnit3>;

}

#endif //STATIC_DIMENSION_VOLUME_IMPL_H
