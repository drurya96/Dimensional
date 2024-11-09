#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "LengthDimension.h"
#include "TimeDimension.h"

namespace Dimension
{

   template<typename LengthUnit, typename TimeUnit>
   concept IsSpeedUnits = 
      std::is_same_v<typename LengthUnit::Dim, LengthType> &&
      std::is_same_v<typename TimeUnit::Dim,   TimeType>;

   template<typename T>
   struct is_speed : std::is_convertible<T, BaseDimension<std::tuple<Meters>, std::tuple<Seconds>>> {};

   template<typename T>
   constexpr bool is_speed_v = is_speed<T>::value;

   template<typename T>
   concept speed_type = is_speed_v<T>;

   template<typename LengthT, typename TimeT>
   requires IsSpeedUnits<LengthT, TimeT>
   PrecisionType getSpeed(speed_type auto obj)
   {
      return obj.template GetVal<std::tuple<LengthT>, std::tuple<TimeT>>();
   }

   template<typename LengthUnit, typename TimeUnit>
   requires IsSpeedUnits<LengthUnit, TimeUnit>
   class Speed : public BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>
   {
   public:
      using BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>::BaseDimension;

      Speed(double val) : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(val){}

      template<typename LengthT, typename TimeT>
      requires IsSpeedUnits<LengthT, TimeT>
      Speed(const BaseDimension<std::tuple<LengthT>, std::tuple<TimeT>>& base) : 
         BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(base.template GetVal<std::tuple<LengthUnit>, std::tuple<TimeUnit>>()) {}

      
      template<typename LengthUnitRet, typename TimeUnitRet>
      requires IsSpeedUnits<LengthUnitRet, TimeUnitRet>
      [[deprecated("Use the free function getSpeed() instead.")]]
      double GetSpeed() const
      {
         return getSpeed<LengthUnitRet, TimeUnitRet>(*this);
      }
   };

   template<typename LengthUnit, typename TimeUnit>
   requires IsSpeedUnits<LengthUnit, TimeUnit>
   Speed(LengthUnit, TimeUnit) -> Speed<LengthUnit, TimeUnit>;

   template<typename LengthUnit, typename TimeUnit>
   requires IsSpeedUnits<LengthUnit, TimeUnit>
   Speed(BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>) -> Speed<LengthUnit, TimeUnit>;

}

#endif //STATIC_DIMENSION_SPEED_H
