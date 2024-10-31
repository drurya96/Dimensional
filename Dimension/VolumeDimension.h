#ifndef STATIC_DIMENSION_Volume_H
#define STATIC_DIMENSION_Volume_H

#include "LengthDimension.h"

namespace Dimension
{

   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   concept IsVolumeUnits = 
      std::is_same_v<typename LengthUnit1::Dim, LengthType> &&
      std::is_same_v<typename LengthUnit2::Dim, LengthType> &&
      std::is_same_v<typename LengthUnit3::Dim, LengthType>;

   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   requires IsVolumeUnits<LengthUnit1, LengthUnit2, LengthUnit3>
   class Volume : public BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>::BaseDimension;

      Volume(double val) : BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>(val){}

      template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
      Volume(const BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>& base) : 
         BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>(base.template GetVal<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>()) {}

      
      template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
      double GetVolume() const
      {
         return this->template GetVal<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>();
      }
   };

   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   Volume(LengthUnit1, LengthUnit2, LengthUnit3) -> Volume<LengthUnit1, LengthUnit2, LengthUnit3>;

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   Volume(BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>) -> Volume<LengthUnit1, LengthUnit2, LengthUnit3>;

   struct Liter
   {
      using NumTuple = std::tuple<DeciMeters, DeciMeters, DeciMeters>;
      using DenTuple = std::tuple<>;
   };

   struct MilliLiter
   {
      using NumTuple = std::tuple<CentiMeters, CentiMeters, CentiMeters>;
      using DenTuple = std::tuple<>;
   };

   struct GallonLength : public LengthUnit<GallonLength> { public: using LengthUnit::LengthUnit; };

   template<> struct Conversion<Meters, GallonLength> { static constexpr PrecisionType slope = (1.0 / 0.15562); };
   template<> struct Conversion<GallonLength, Meters> { static constexpr PrecisionType slope = 0.15562; };

   struct Gallon
   {
      using NumTuple = std::tuple<GallonLength, GallonLength, GallonLength>;
      using DenTuple = std::tuple<>;
   };

   template<typename T>
   struct is_volume : std::is_convertible<T, Volume<Meters, Meters, Meters>> {};

   template<typename T>
   constexpr bool is_volume_v = is_volume<T>::value;

   template<typename T>
   concept volume_type = is_volume_v<T>;

}

#endif //STATIC_DIMENSION_Volume_H
