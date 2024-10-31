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

   // TODO: Provide reasonable error message for failed conversion due to no primary conversion

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
      double GetTime() const
      {
         return this->template GetVal<std::tuple<T>, std::tuple<>>();
      }
   };

   template<IsTimeUnit T>
   Time(T) -> Time<T>;

   template<IsTimeUnit TimeUnit>
   Time(BaseDimension<std::tuple<TimeUnit>, std::tuple<>>) -> Time<TimeUnit>;

   template<> struct Conversion<Seconds, Minutes> { static constexpr PrecisionType slope = (1.0 / 60.0); };
   template<> struct Conversion<Minutes, Seconds> { static constexpr PrecisionType slope = 60.0; };
   template<> struct Conversion<Seconds, Hours> { static constexpr PrecisionType slope = (1.0 / 3600.0); };
   template<> struct Conversion<Hours, Seconds> { static constexpr PrecisionType slope = 3600.0; };

   ALL_SI_PREFIXES(Seconds, TimeUnit);

   template<typename T>
   struct is_time : std::is_convertible<T, Time<Seconds>> {};

   template<typename T>
   constexpr bool is_time_v = is_time<T>::value;

   template<typename T>
   concept time_type = is_time_v<T>;

}

#endif //STATIC_DIMENSION_TIME_H
