#ifndef STATIC_DIMENSION_Volume_H
#define STATIC_DIMENSION_Volume_H

#include "LengthDimension.h"

namespace Dimension
{
   template<typename LengthUnit1, typename LengthUnit2, typename LengthUnit3>
   class Volume : public BaseDimension<std::tuple<LengthUnit1, LengthUnit2, LengthUnit3>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<typename MassUnit::Dim, typename Grams::Dim>, "Unit provided does not derive from MassUnit");
      static_assert(std::is_same_v<typename LengthUnit::Dim, typename Meters::Dim>, "Unit provided does not derive from LengthUnit");
      static_assert(std::is_same_v<typename TimeUnit1::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      static_assert(std::is_same_v<typename TimeUnit2::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
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

   struct Liter : public DerivedUnit
   {
      using NumTuple = std::tuple<DeciMeters, DeciMeters, DeciMeters>;
      using DenTuple = std::tuple<>;
   };

   struct MilliLiter : public DerivedUnit
   {
      using NumTuple = std::tuple<CentiMeters, CentiMeters, CentiMeters>;
      using DenTuple = std::tuple<>;
   };

   struct GallonLength : public LengthUnit<GallonLength> { public: using LengthUnit::LengthUnit; };

   template<> struct Conversion<Meters, GallonLength> { static constexpr PrecisionType slope = (1.0 / 0.15562); };
   template<> struct Conversion<GallonLength, Meters> { static constexpr PrecisionType slope = 0.15562; };

   struct Gallon : public DerivedUnit
   {
      using NumTuple = std::tuple<GallonLength, GallonLength, GallonLength>;
      using DenTuple = std::tuple<>;
   };

}

#endif //STATIC_DIMENSION_Volume_H
