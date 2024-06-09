#ifndef STATIC_DIMENSION_FORCE_H
#define STATIC_DIMENSION_FORCE_H

#include "StaticMass.h"
#include "StaticLength.h"
#include "StaticTime.h"

namespace StaticDimension
{
   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   class Force : public BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>
   {
   public:
      static_assert(std::is_same_v<typename MassUnit::Dim, typename Grams::Dim>, "Unit provided does not derive from MassUnit");
      static_assert(std::is_same_v<typename LengthUnit::Dim, typename Meters::Dim>, "Unit provided does not derive from LengthUnit");
      static_assert(std::is_same_v<typename TimeUnit1::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      static_assert(std::is_same_v<typename TimeUnit2::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      using BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>::BaseDimension;

      Force(double val) : BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>(val){}

      template<typename MassT, typename LengthT, typename TimeT1, typename TimeT2>
      Force(const BaseDimension<std::tuple<MassT, LengthT>, std::tuple<TimeT1, TimeT2>>& base) : 
         BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>(base.template GetVal<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>()) {}

      
      template<typename MassUnitRet, typename LengthUnitRet, typename TimeUnitRet1, typename TimeUnitRet2>
      double GetForce()
      {
         return this->template GetVal<std::tuple<MassUnitRet, LengthUnitRet>, std::tuple<TimeUnitRet1, TimeUnitRet2>>();
      }
   };

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   Force(MassUnit, LengthUnit, TimeUnit1, TimeUnit2) -> Force<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>;

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   Force(BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>) -> Force<LengthUnit, TimeUnit1, TimeUnit2>;

   struct Newton : public DerivedUnit
   {
      using NumTuple = std::tuple<KiloGrams, Meters>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

   struct PoundForce : public DerivedUnit
   {
      using NumTuple = std::tuple<PoundMass, Feet>;
      using DenTuple = std::tuple<Seconds, Seconds>;
   };

}

#endif //STATIC_DIMENSION_FORCE_H
