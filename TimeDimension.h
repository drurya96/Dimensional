
#ifndef DIMENSION_TIME_H
#define DIMENSION_TIME_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

#include <cassert> // Necessary for assert in initializeTimeUnits. TODO: Remove if logic changes

namespace Dimension
{
   template<typename ... is_inverse>
   class TimeUnit : public BaseUnit<is_inverse...>
   {
   public:
      explicit TimeUnit(std::string name) : BaseUnit(name) {}

      TimeUnit() : BaseUnit() {}

      std::string GetDimName() override { return "Time"; }
      std::string GetUnitName() override { return unitName; }
      TimeUnit<>* GetBaseUnit() override { return &TimeUnits::Seconds; }

      bool add_conversion(TimeUnit toUnit, std::function<double(double)> conversion)
      {
         conversions[toUnit.unitName] = conversion;
         return true;
      };

   private:

   };

   template<typename ... is_inverse>
   class Time : public BaseDimension<TimeUnit<>>
   {
   public:

      explicit Time(double value, TimeUnit<is_inverse...>* unit)
         : BaseDimension<TimeUnit<is_inverse...>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(unit) }, std::vector<BaseUnit<>*>{})//, numList({ unit }), denList({})
      {}

      Time(const BaseDimension<TimeUnit<is_inverse...>>& base) : BaseDimension<TimeUnit<is_inverse...>>(base.value, base.numList, base.denList)
      {}

   };

   namespace TimeUnits
   {
      extern TimeUnit<> Seconds;
      extern TimeUnit<> Minutes;
   }
   
   static std::vector<BaseUnit<>*> TimeUnitVector;

   inline bool initializeTimeUnits()
   {
      TimeUnits::Seconds.add_conversion(TimeUnits::Minutes, [](double val) {return val / 60.0; });
      TimeUnits::Minutes.add_conversion(TimeUnits::Seconds, [](double val) {return val * 60.0; });

      TimeUnits::Seconds.baseUnitVector = &TimeUnitVector;
      TimeUnits::Minutes.baseUnitVector = &TimeUnitVector;


      TimeUnitVector.push_back(&TimeUnits::Seconds);
      TimeUnitVector.push_back(&TimeUnits::Minutes);


      // Validate there is a conversion to Seconds from each unit
      // TODO: Consider returning false instead of asserting
      for (auto unit : TimeUnitVector)
      {
         auto findit = unit->conversions.find(TimeUnits::Seconds.GetUnitName());
         auto findit2 = TimeUnits::Seconds.conversions.find(unit->GetUnitName());
         assert((findit != unit->conversions.end()) || (unit == &TimeUnits::Seconds));
         assert((findit2 != TimeUnits::Seconds.conversions.end()) || (unit == &TimeUnits::Seconds));
      }

      return true;
   }

}

#endif // DIMENSION_TIME_H