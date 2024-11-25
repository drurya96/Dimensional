#ifndef STATIC_DIMENSION_TIME_IMPL_H
#define STATIC_DIMENSION_TIME_IMPL_H

#include "../../BaseDimension.h"

namespace Dimension
{
   struct TimeType {};
   struct Seconds;

   template<typename TimeUnit>
   concept IsTimeUnit = std::is_same_v<typename TimeUnit::Dim, TimeType>;

   template<typename Unit, StringLiteral Name, StringLiteral Abbreviation>
   struct TimeUnit : public BaseUnit<Unit, Name, Abbreviation, "Time">
   {
   public:
      using Dim = TimeType;
      using Primary = Seconds;
      
   };

   template<typename T>
   struct is_time : std::false_type {};

   template<typename T>
   struct is_time<BaseDimension<std::tuple<T>, std::tuple<>>> : std::bool_constant<IsTimeUnit<T>> {};

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

   template<typename T>
   struct is_time<Time<T>> : std::bool_constant<IsTimeUnit<T>> {};

}

#endif //STATIC_DIMENSION_TIME_IMPL_H
