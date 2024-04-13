
#ifndef DIMENSION_TIME_H
#define DIMENSION_TIME_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>
#include <type_traits>

namespace Dimension
{
   template<typename ... is_inverse>
   class TimeUnit : public BaseUnit<is_inverse...>
   {
   public:
      explicit TimeUnit(std::string name) :
         unitName(name)
      {
         //numList.push_back(this);
      };

      TimeUnit() :
         unitName("")
      {};

      std::string unitName;

      std::string GetDimName() override {
         return "Time";
      };

      std::string GetUnitName() override {
         return unitName;
      }

      // The intention is to store a map of conversion functions to each known unit, minimally the "default" unit of this dimension
      //std::unordered_map<std::string, std::function<double(Time)>> conversions;
      //std::unordered_map<std::string, std::function<double(double)>> conversions;

      //bool add_conversion(TimeUnit toUnit, std::function<double(Time)> conversion)
      bool add_conversion(TimeUnit toUnit, std::function<double(double)> conversion)
      {
         conversions[toUnit.unitName] = conversion;
         return true;
      };

      // This might be needed for Singleton, need to revisit
      /*
      static const TimeUnit& instance() {
         static TimeUnit<T> inst("");
         return inst;
      }
      */

   private:
      constexpr double GetStandardID() override { return 2.0; };
      constexpr double GetInverseID() override { return 1 / 2.0; };
   };

   template<typename ... is_inverse>
   class Time : public BaseDimension<TimeUnit<>>
   {
   public:
      /*
      explicit Time(double value, TimeUnit<>* unit): BaseDimension(value),
         unit(unit)
      {
        // Numerator.push_back(unit);
      };
      */

      explicit Time(double value, TimeUnit<is_inverse...>* unit)
         : BaseDimension<TimeUnit<is_inverse...>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(unit) }, std::vector<BaseUnit<>*>{})//, numList({ unit }), denList({})
      {

      };

      Time(const BaseDimension<TimeUnit<is_inverse...>>& base) : BaseDimension<TimeUnit<is_inverse...>>(base.value, base.numList, base.denList)
      {

      }


      /*
      operator BaseDimension<TimeUnit<>>() const {
         return BaseDimension<TimeUnit<>>(value);
      }
      */
      

      //double value;
      
      //std::vector<BaseUnit*> Numerator;
      //TimeUnit<>* unit;

      double GetVal(TimeUnit<is_inverse...>* getUnit);

      //static bool initializeTimeUnits();

   };


   namespace TimeUnits
   {
      extern TimeUnit<> Seconds;
      extern TimeUnit<> Minutes;
   }
   
   inline bool initializeTimeUnits()
   {
      TimeUnits::Seconds.add_conversion(TimeUnits::Minutes, [](double val) {return val / 60.0; });
      TimeUnits::Minutes.add_conversion(TimeUnits::Seconds, [](double val) {return val * 60.0; });
      return true;
   }

   template<typename... is_inverse>
   inline double Time<is_inverse...>::GetVal(TimeUnit<is_inverse...>* getUnit)
   {
      
      if (getUnit == numList[0])
      {
         return value;
      }
      else
      {
         return numList[0]->conversions[getUnit->unitName](this->value);
      }
      
   }

   //using UpdatedSimplifiedUnits = RegisterSimplifier<TimeUnitSimplifier>;

}

#endif // DIMENSION_TIME_H