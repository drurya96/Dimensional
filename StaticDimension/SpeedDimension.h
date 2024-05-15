#ifndef DIMENSION_SPEED_H
#define DIMENSION_SPEED_H

#include "LengthDimension.h"
#include "TimeDimension.h"

namespace Dimension
{
   template<typename LengthUnit = Meters, typename TimeUnit = Seconds>
   class Speed : public BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>
   {
   public:
      static_assert(std::is_same_v<LengthUnit::Dim, Meters::Dim>, "Unit provided does not derive from TimeUnit");
      static_assert(std::is_same_v<TimeUnit::Dim, Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      using BaseDimension::BaseDimension;

      Speed(double val) : BaseDimension(1.0, std::tuple<LengthUnit>{val}, std::tuple<TimeUnit>{1.0}) {} // TODO: Is this how I want to handle this?


      /// @brief Cast operator from a BaseDimension
      // TODO: Can this be made more efficient?
      // TODO: Can Cast operator be generalized?
      // TODO: Ensure all units have a cast operator
      Speed(const BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>& base) : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(base)
      {}

      
      template<typename LengthUnitRet, typename TimeUnitRet>
      double GetSpeed()
      {
         // TODO: Check if direct conversions are more efficient
         //   Compile may optimize to the same, unclear
         return GetVal<std::tuple<LengthUnitRet>, std::tuple<TimeUnitRet>>();
      }
      

   };
}



#endif //DIMENSION_SPEED_H