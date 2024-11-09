#ifndef STATIC_DIMENSION_TIME_H
#define STATIC_DIMENSION_TIME_H

#include "BaseDimension.h"

namespace Dimension
{
   struct TimeType {};
   struct Seconds;

   template<typename TimeUnit>
   concept IsTimeUnit = std::is_same_v<typename TimeUnit::Dim, TimeType>;

   template<typename Unit>
   struct TimeUnit : public BaseUnit<Unit>
   {
   public:
      using BaseUnit<Unit>::BaseUnit;

      using Dim = TimeType;
      using Primary = Seconds;
   };

   struct Seconds : public TimeUnit<Seconds> { public: using TimeUnit::TimeUnit; };
   struct Minutes : public TimeUnit<Minutes> { public: using TimeUnit::TimeUnit; };
   struct Hours : public TimeUnit<Hours> { public: using TimeUnit::TimeUnit; };

   template<typename T>
   struct is_time : std::is_convertible<T, BaseDimension<std::tuple<Seconds>, std::tuple<>>> {};

   template<typename T>
   constexpr bool is_time_v = is_time<T>::value;

   template<typename T>
   concept time_type = is_time_v<T>;

   template<IsTimeUnit T>
   PrecisionType getTime(time_type auto obj)
   {
      return obj.template GetVal<std::tuple<T>, std::tuple<>>();
   }

   template<IsTimeUnit Unit>
   class Time : public BaseDimension<std::tuple<Unit>, std::tuple<>>
   {
   public:
      using BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension;

      Time() : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(0.0) {}

      Time(double val) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(val) {}

      template<IsTimeUnit T>
      Time(const BaseDimension<std::tuple<T>, std::tuple<>>& base) : BaseDimension<std::tuple<Unit>, std::tuple<>>::BaseDimension(base.template GetVal<std::tuple<Unit>, std::tuple<>>()) {}

      
      template<IsTimeUnit T>
      [[deprecated("Use the free function getTime() instead.")]]
      double GetTime() const
      {
         return getTime<T>(*this);
      }
   };

   template<IsTimeUnit T>
   Time(T) -> Time<T>;

   template<IsTimeUnit TimeUnit>
   Time(BaseDimension<std::tuple<TimeUnit>, std::tuple<>>) -> Time<TimeUnit>;

   template<> struct Conversion<Seconds, Minutes> { static constexpr PrecisionType slope = (1.0 / 60.0); };
   template<> struct Conversion<Seconds, Hours> { static constexpr PrecisionType slope = (1.0 / 3600.0); };

   ALL_SI_PREFIXES(Seconds, TimeUnit);

}

#endif //STATIC_DIMENSION_TIME_H
