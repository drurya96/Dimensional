#ifndef STATIC_DIMENSION_TIME_H
#define STATIC_DIMENSION_TIME_H

#include "StaticBaseDimension.h"

namespace StaticDimension
{
   struct TimeType {};
   struct Seconds;

   template<typename Unit>
   struct TimeUnit : public BaseUnit
   {
   public:
      using BaseUnit::BaseUnit;

      using Dim = TimeType;
      using Primary = Seconds;
   };

   struct Seconds : public TimeUnit<Seconds> { public: using TimeUnit::TimeUnit; };
   struct Minutes : public TimeUnit<Minutes> { public: using TimeUnit::TimeUnit; };
   struct Hours : public TimeUnit<Hours> { public: using TimeUnit::TimeUnit; };

   // TODO: Provide reasonable error message for failed conversion due to no primary conversion

   template<typename Unit = Seconds>
   class Time : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      static_assert(std::is_same_v<Unit::Dim, Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Time() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      Time(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      template<typename T>
      Time(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      template<typename T>
      double GetTime()
      {
         return this->GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   /*
   template<>
   inline double Convert<Seconds, Minutes>(double input)
   {
      return input / 60.0;
   }

   template<>
   inline double Convert<Minutes, Seconds>(double input)
   {
      return input * 60.0;
   }

   template<>
   inline double Convert<Seconds, Hours>(double input)
   {
      return input / 3600.0;
   }

   template<>
   inline double Convert<Hours, Seconds>(double input)
   {
      return input * 3600.0;
   }
   */
   
   template<> struct Conversion<Seconds, Minutes> { static constexpr PrecisionType slope = (1.0 / 60.0); };
   template<> struct Conversion<Minutes, Seconds> { static constexpr PrecisionType slope = 60.0; };
   template<> struct Conversion<Seconds, Hours> { static constexpr PrecisionType slope = (1.0 / 3600.0); };
   template<> struct Conversion<Hours, Seconds> { static constexpr PrecisionType slope = 3600.0; };

}

#endif //STATIC_DIMENSION_TIME_H