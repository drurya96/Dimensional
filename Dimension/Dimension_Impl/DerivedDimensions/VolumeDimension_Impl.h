#ifndef STATIC_DIMENSION_VOLUME_IMPL_H
#define STATIC_DIMENSION_VOLUME_IMPL_H

#include "../../LengthDimension.h"

namespace Dimension
{

   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   concept IsVolumeUnits = 
      std::is_same_v<typename LengthUnit1::Dim, LengthType> &&
      std::is_same_v<typename LengthUnit2::Dim, LengthType> &&
      std::is_same_v<typename LengthUnit3::Dim, LengthType>;

   template<typename T>
   struct is_volume : std::false_type {};

   template<typename LengthT1, typename LengthT2, typename LengthT3>
   struct is_volume<BaseDimension<std::tuple<LengthT1, LengthT2, LengthT3>, std::tuple<>>> : std::bool_constant<IsVolumeUnits<LengthT1, LengthT2, LengthT3>> {};

   template<typename T>
   constexpr bool is_volume_v = is_volume<T>::value;

   template<typename T>
   concept volume_type = is_volume_v<T>;

   template<typename LengthT1, typename LengthT2, typename LengthT3>
   requires IsVolumeUnits<LengthT1, LengthT2, LengthT3>
   PrecisionType getVolume(volume_type auto obj)
   {
      return obj.template GetVal<std::tuple<LengthT1, LengthT2, LengthT3>, std::tuple<>>();
   }

   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   requires IsVolumeUnits<LengthUnit1, LengthUnit2, LengthUnit3>
   class Volume : public BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>::BaseDimension;

      Volume(double val) : BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>(val){}

      template<typename LengthT1, typename LengthT2, typename LengthT3>
      requires IsVolumeUnits<LengthT1, LengthT2, LengthT3>
      Volume(const BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>& base) : 
         BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>(base.template GetVal<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>()) {}

      
      template<typename LengthT1, typename LengthT2, typename LengthT3>
      requires IsVolumeUnits<LengthT1, LengthT2, LengthT3>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<LengthT1, LengthT2, LengthT3>(*this);
      }
   };

   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   requires IsVolumeUnits<LengthUnit1, LengthUnit2, LengthUnit3>
   Volume(LengthUnit1, LengthUnit2, LengthUnit3) -> Volume<LengthUnit1, LengthUnit2, LengthUnit3>;

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsVolumeUnits<LengthUnit1, LengthUnit2, LengthUnit3>
   Volume(BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>) -> Volume<LengthUnit1, LengthUnit2, LengthUnit3>;

   template<typename LengthT1, typename LengthT2, typename LengthT3>
   struct is_volume<Volume<LengthT1, LengthT2, LengthT3>> : std::bool_constant<IsVolumeUnits<LengthT1, LengthT2, LengthT3>> {};

}

#endif //STATIC_DIMENSION_VOLUME_IMPL_H
