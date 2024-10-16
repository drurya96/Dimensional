#ifndef STATIC_DIMENSION_SPEED_H
#define STATIC_DIMENSION_SPEED_H

#include "Length.h"
#include "TimeDimension.h"

namespace Dimension
{
   template<typename LengthUnit, typename TimeUnit>
   class Speed : public BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>
   {
   public:
      static_assert(std::is_same_v<typename LengthUnit::Dim, typename Meters::Dim>, "Unit provided does not derive from TimeUnit");
      static_assert(std::is_same_v<typename TimeUnit::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      using BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>::BaseDimension;

      Speed(double val) : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(val){}

      template<typename LengthT, typename TimeT>
      Speed(const BaseDimension<std::tuple<LengthT>, std::tuple<TimeT>>& base) : 
         BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(base.template GetVal<std::tuple<LengthUnit>, std::tuple<TimeUnit>>()) {}

      
      template<typename LengthUnitRet, typename TimeUnitRet>
      double GetSpeed() const
      {
         return this->template GetVal<std::tuple<LengthUnitRet>, std::tuple<TimeUnitRet>>();
      }
   };

   template<typename LengthUnit, typename TimeUnit>
   Speed(LengthUnit, TimeUnit) -> Speed<LengthUnit, TimeUnit>;

   template<typename LengthUnit, typename TimeUnit>
   Speed(BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>) -> Speed<LengthUnit, TimeUnit>;
}

#endif //STATIC_DIMENSION_SPEED_H
