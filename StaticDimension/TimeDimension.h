#ifndef DIMENSION_TIME_H
#define DIMENSION_TIME_H

#include "BaseDimension.h"

namespace Dimension
{
   struct Time {};
   class Seconds;

   template<typename Unit>
   class TimeUnit : public BaseUnit<TimeUnit<Unit>>
   {
   public:
      using BaseUnit::BaseUnit;

      using Dim = Time;

      Seconds GetPrimary();

      /*
      Seconds GetPrimary()
      {
         return ConvertValue<Unit, Seconds>(*this);
         //return value;
      }
      */
   };

   //class TimeUnit : public BaseUnit<TimeUnit> { public: using BaseUnit::BaseUnit; };

   class Seconds : public TimeUnit<Seconds> { public: using TimeUnit::TimeUnit; };
   class Minutes : public TimeUnit<Minutes> { public: using TimeUnit::TimeUnit; };

   template<typename Unit>
   inline Seconds TimeUnit<Unit>::GetPrimary()
   {


      if constexpr (std::is_same_v<Unit, Seconds>)
      {
         return *(static_cast<Seconds*>(this));
      }
      else
      {
         return ConvertValue<Unit, Seconds>(*(static_cast<Unit*>(this)));
      }

      
      //return value;
   }







   template<>
   Minutes ConvertValue<Seconds, Minutes>(Seconds& obj)
   {
      return Minutes(obj.GetValue() / 60.0);
   }
   
   template<>
   Seconds ConvertValue<Minutes, Seconds>(Minutes& obj)
   {
      return Seconds(obj.GetValue() * 60.0);
   }
   
   
}

#endif //DIMENSION_TIME_H