
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
         if (unitName != "")
         {
            unitName = ""; // placeholder logic for name construction
         }
      };

      std::string unitName;
      LengthUnit<>& lengthUnit;
      TimeUnit<>& timeUnit;
   };

   class Speed : public BaseDimension<LengthUnit<>, TimeUnit<Inverse>>
   {
   public:

      explicit Speed(double value, LengthUnit<>* length, TimeUnit<>* time)
         : BaseDimension<LengthUnit<>, TimeUnit<Inverse>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(length) }, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(time) })//, numList({ unit }), denList({})
      {

      };

      
      Speed(const BaseDimension<LengthUnit<>, TimeUnit<Inverse>>& base) : BaseDimension<LengthUnit<>, TimeUnit<Inverse>>(base.value, base.numList, base.denList){}
      

   };

   // TODO: Not using this yet, but consider adding it for convenience
   namespace SpeedUnits
   {
      extern SpeedUnit<> MetersPerSecond;
      extern SpeedUnit<> FeetPerSecond;
   }

}

#endif // DIMENSION_SPEED_H