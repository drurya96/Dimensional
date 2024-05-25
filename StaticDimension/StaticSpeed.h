#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "StaticLength.h"
#include "StaticTime.h"

namespace StaticDimension
{
   template<typename LengthUnit = Meters, typename TimeUnit = Seconds>
   class Speed : public BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>
   {
   public:
      static_assert(std::is_same_v<LengthUnit::Dim, Meters::Dim>, "Unit provided does not derive from TimeUnit");
      static_assert(std::is_same_v<TimeUnit::Dim, Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      using BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>::BaseDimension;

      Speed(double val) : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(val, std::tuple<LengthUnit>{}, std::tuple<TimeUnit>{}) {} // TODO: Is this how I want to handle this?

      template<typename LengthT, typename TimeT>
      Speed(const BaseDimension<std::tuple<LengthT>, std::tuple<TimeT>>& base) : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(base.GetVal<std::tuple<LengthUnit>, std::tuple<TimeUnit>>()) {}

      
      template<typename LengthUnitRet, typename TimeUnitRet>
      double GetSpeed()
      {
         return this->GetVal<std::tuple<LengthUnitRet>, std::tuple<TimeUnitRet>>();
      }
   };
}

#endif //STATIC_DIMENSION_SPEED_H