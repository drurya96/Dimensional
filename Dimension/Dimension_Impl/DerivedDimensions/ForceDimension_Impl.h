#ifndef STATIC_DIMENSION_FORCE_IMPL_H
#define STATIC_DIMENSION_FORCE_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   concept IsForceUnits = 
    std::is_same_v<typename MassUnit::Dim,   MassType> &&
    std::is_same_v<typename LengthUnit::Dim, LengthType> &&
    std::is_same_v<typename TimeUnit1::Dim,  TimeType> &&
    std::is_same_v<typename TimeUnit2::Dim,  TimeType>;

   template<typename T>
   struct is_force : std::false_type {};

   template<typename MassT, typename LengthT, typename TimeT1, typename TimeT2>
   struct is_force<BaseDimension<std::tuple<MassT, LengthT>, std::tuple<TimeT1, TimeT2>>> : std::bool_constant<IsForceUnits<MassT, LengthT, TimeT1, TimeT2>> {};

   template<typename T>
   constexpr bool is_force_v = is_force<T>::value;

   template<typename T>
   concept force_type = is_force_v<T>;

   template<typename MassT, typename LengthT, typename TimeT1, typename TimeT2>
   requires IsForceUnits<MassT, LengthT, TimeT1, TimeT2>
   PrecisionType getForce(force_type auto obj)
   {
      return obj.template GetVal<std::tuple<MassT, LengthT>, std::tuple<TimeT1, TimeT2>>();
   }


   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsForceUnits<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>
   class Force : public BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>
   {
   public:
      using BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>::BaseDimension;

      Force(double val) : BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>(val){}

      template<typename MassT, typename LengthT, typename TimeT1, typename TimeT2>
      requires IsForceUnits<MassT, LengthT, TimeT1, TimeT2>
      Force(const BaseDimension<std::tuple<MassT, LengthT>, std::tuple<TimeT1, TimeT2>>& base) : 
         BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>(base.template GetVal<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>()) {}

      
      template<typename MassUnitRet, typename LengthUnitRet, typename TimeUnitRet1, typename TimeUnitRet2>
      requires IsForceUnits<MassUnitRet, LengthUnitRet, TimeUnitRet1, TimeUnitRet2>
      [[deprecated("Use the free function getForce() instead.")]]
      double GetForce() const
      {
         return getSpeed<MassUnitRet, LengthUnitRet, TimeUnitRet1, TimeUnitRet2>(*this);
      }
   };

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsForceUnits<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>
   Force(MassUnit, LengthUnit, TimeUnit1, TimeUnit2) -> Force<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>;

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsForceUnits<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>
   Force(BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>) -> Force<LengthUnit, TimeUnit1, TimeUnit2>;

   template<typename MassT, typename LengthT, typename TimeT1, typename TimeT2>
   struct is_force<Force<MassT, LengthT, TimeT1, TimeT2>> : std::bool_constant<IsForceUnits<MassT, LengthT, TimeT1, TimeT2>> {};

}

#endif //STATIC_DIMENSION_FORCE_IMPL_H
