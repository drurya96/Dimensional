#ifndef STATIC_DIMENSION_TIME_H
#define STATIC_DIMENSION_TIME_H

#include "StaticBaseDimension.h"

namespace StaticDimension
{
   struct TimeType {};
   class Seconds;
   class Minutes;

   template<typename Unit>
   class TimeUnit : public BaseUnit<TimeUnit<Unit>>
   {
   public:
      using BaseUnit::BaseUnit;

      using Dim = TimeType;
      using Primary = Seconds;

      Primary GetPrimary() const;
   };

   class Seconds : public TimeUnit<Seconds> { public: using TimeUnit::TimeUnit; };
   class Minutes : public TimeUnit<Minutes> { public: using TimeUnit::TimeUnit; };
   class Hours : public TimeUnit<Hours> { public: using TimeUnit::TimeUnit; };

   template<typename Unit>
   inline typename TimeUnit<Unit>::Primary TimeUnit<Unit>::GetPrimary() const
   {
      return GetPrimaryImpl<Unit, Primary>(*(static_cast<const Unit*>(this)));
   }

   // TODO: Provide reasonable error message for failed conversion due to no primary conversion

   template<>
   inline Minutes ConvertValue<Seconds, Minutes>(const Seconds& obj)
   {
      return Minutes(obj.GetValue() / 60.0);
   }
   
   template<>
   inline Seconds ConvertValue<Minutes, Seconds>(const Minutes& obj)
   {
      return Seconds(obj.GetValue() * 60.0);
   }
   
   template<>
   inline Hours ConvertValue<Seconds, Hours>(const Seconds& obj)
   {
      return Hours(obj.GetValue() / 3600.0);
   }

   template<>
   inline Seconds ConvertValue<Hours, Seconds>(const Hours& obj)
   {
      return Seconds(obj.GetValue() * 3600.0);
   }



   template<typename Unit = Seconds>
   class Time : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<Unit::Dim, Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      using BaseDimension::BaseDimension;

      Time() : BaseDimension(1.0, std::tuple<Unit>{0.0}, std::tuple<>{}) {}

      Time(double val) : BaseDimension(1.0, std::tuple<Unit>{val}, std::tuple<>{}) {}

      template<typename T>
      Time(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension(base.GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<typename T>
      double GetTime()
      {
         return value * ConvertValue<Unit, T>(std::get<0>(numList)).GetValue();
      }
      
   };

   
}

#endif //STATIC_DIMENSION_TIME_H