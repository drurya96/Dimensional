#ifndef STATIC_DIMENSION_ACCELERATION_H
#define STATIC_DIMENSION_ACCELERATION_H

#include "LengthDimension.h"
#include "TimeDimension.h"

namespace Dimension
{

   template<typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   concept IsAccelerationUnits = 
      std::is_same_v<typename LengthUnit::Dim, LengthType> &&
      std::is_same_v<typename TimeUnit1::Dim,  TimeType> &&
      std::is_same_v<typename TimeUnit2::Dim,  TimeType>;

   template<typename T>
   struct is_acceleration : std::is_convertible<T, BaseDimension<std::tuple<Meters>, std::tuple<Seconds, Seconds>>> {};

   template<typename T>
   constexpr bool is_acceleration_v = is_acceleration<T>::value;

   template<typename T>
   concept acceleration_type = is_acceleration_v<T>;

   template<typename LengthT, typename TimeT1, typename TimeT2>
   requires IsAccelerationUnits<LengthT, TimeT1, TimeT2>
   PrecisionType getAcceleration(acceleration_type auto obj)
   {
      return obj.template GetVal<std::tuple<LengthT>, std::tuple<TimeT1, TimeT2>>();
   }

   template<typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsAccelerationUnits<LengthUnit, TimeUnit1, TimeUnit2>
   class Acceleration : public BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>
   {
   public:
      static_assert(std::is_same_v<typename LengthUnit::Dim, typename Meters::Dim>, "Unit provided does not derive from LengthUnit");
      static_assert(std::is_same_v<typename TimeUnit1::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      static_assert(std::is_same_v<typename TimeUnit2::Dim, typename Seconds::Dim>, "Unit provided does not derive from TimeUnit");
      using BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>::BaseDimension;

      Acceleration(double val) : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>(val){}

      template<typename LengthT, typename TimeT1, typename TimeT2>
      requires IsAccelerationUnits<LengthT, TimeT1, TimeT2>
      Acceleration(const BaseDimension<std::tuple<LengthT>, std::tuple<TimeT1, TimeT2>>& base) : 
         BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>(base.template GetVal<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>()) {}

      
      template<typename LengthUnitRet, typename TimeUnitRet1, typename TimeUnitRet2>
      requires IsAccelerationUnits<LengthUnitRet, TimeUnitRet1, TimeUnitRet2>
      [[deprecated("Use the free function getAcceleration() instead.")]]
      double GetAcceleration() const
      {
         return getSpeed<LengthUnitRet, TimeUnitRet1, TimeUnitRet2>(*this);
      }
   };

   template<typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsAccelerationUnits<LengthUnit, TimeUnit1, TimeUnit2>
   Acceleration(LengthUnit, TimeUnit1, TimeUnit2) -> Acceleration<LengthUnit, TimeUnit1, TimeUnit2>;

   template<typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsAccelerationUnits<LengthUnit, TimeUnit1, TimeUnit2>
   Acceleration(BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>) -> Acceleration<LengthUnit, TimeUnit1, TimeUnit2>;

}

#endif //STATIC_DIMENSION_ACCELERATION_H
