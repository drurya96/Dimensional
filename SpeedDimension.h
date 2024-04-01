
#ifndef DIMENSION_SPEED_H
#define DIMENSION_SPEED_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

#include "LengthDimension.h"
#include "TimeDimension.h"

namespace Dimension
{
   template<typename ... is_inverse>
   class SpeedUnit : public BaseUnit<is_inverse...>
   {
   public:
      explicit SpeedUnit(LengthUnit<> lengthUnit, TimeUnit<> timeUnit, std::string unitName = "") :
         unitName(unitName),
         lengthUnit(lengthUnit),
         timeUnit(timeUnit)
      {
         //numList.push_back(&lengthUnit);
         //denList.push_back(&timeUnit);

         if (unitName != "")
         {
            unitName = ""; // placeholder logic for name construction
         }
      };

      std::string unitName;
      LengthUnit<>& lengthUnit;
      TimeUnit<>& timeUnit;
   };

   class Speed
   {
   public:
      explicit Speed(double value, SpeedUnit<>& unit):
         value(value),
         lengthUnit(unit.lengthUnit),
         timeUnit(unit.timeUnit)
      {

      };

      double value;
      LengthUnit<>& lengthUnit;
      TimeUnit<>& timeUnit;

   };

   namespace SpeedUnits
   {
      extern SpeedUnit<> MetersPerSecond;
      extern SpeedUnit<> FeetPerSecond;
   }
   
   //Speed operator/(const Length& length, const Time& time);

   //Speed operator/(const BaseDimension& length, const BaseDimension& time);

}

#endif // DIMENSION_SPEED_H