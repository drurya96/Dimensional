#ifndef STATIC_DIMENSION_ACCELERATION_H
#define STATIC_DIMENSION_ACCELERATION_H

#include "LengthDimension.h"
#include "TimeDimension.h"

namespace Dimension
{
   template<typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   class Acceleration : public BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>
   {
   public:
      static_assert(std::is_same_v<typename LengthUnit::Dim, typename Meters::Dim>, "Unit provided does not derive from LengthUnit");
      static_assert(std::is_same_v<typename TimeUnit1::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      static_assert(std::is_same_v<typename TimeUnit2::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      using BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>::BaseDimension;

      Acceleration(double val) : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>(val){}

      template<typename LengthT, typename TimeT1, typename TimeT2>
      Acceleration(const BaseDimension<std::tuple<LengthT>, std::tuple<TimeT1, TimeT2>>& base) : 
         BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>(base.template GetVal<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>()) {}

      
      template<typename LengthUnitRet, typename TimeUnitRet1, typename TimeUnitRet2>
      double GetAcceleration()
      {
         return this->template GetVal<std::tuple<LengthUnitRet>, std::tuple<TimeUnitRet1, TimeUnitRet2>>();
      }
   };

   template<typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   Acceleration(LengthUnit, TimeUnit1, TimeUnit2) -> Acceleration<LengthUnit, TimeUnit1, TimeUnit2>;

   template<typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   Acceleration(BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>) -> Acceleration<LengthUnit, TimeUnit1, TimeUnit2>;
}

#endif //STATIC_DIMENSION_ACCELERATION_H
