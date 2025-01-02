#ifndef STATIC_DIMENSION_PRESSURE_IMPL_H
#define STATIC_DIMENSION_PRESSURE_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   // Concept for a named Pressure unit (e.g., Pascals)
   template<typename NamedPressure>
   concept IsNamedPressureUnit = requires {
      typename NamedPressure::NumTuple;
      typename NamedPressure::DenTuple;
   };

   // Concept for a Pressure dimension (basic units)
   template<typename MassUnit, typename LengthUnit1, typename TimeUnit1, typename TimeUnit2>
   concept IsPressureUnits = 
      std::is_same_v<typename MassUnit::Dim, MassType> &&
      std::is_same_v<typename LengthUnit1::Dim, LengthType> &&
      std::is_same_v<typename TimeUnit1::Dim, TimeType> &&
      std::is_same_v<typename TimeUnit2::Dim, TimeType>;

   // General concept for an Pressure type
   template<typename T>
   concept IsPressureType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && IsPressureUnits<typename std::tuple_element_t<0, typename T::NumTuple>,
                     typename std::tuple_element_t<1, typename T::NumTuple>,
                     typename std::tuple_element_t<0, typename T::DenTuple>,
                     typename std::tuple_element_t<1, typename T::DenTuple>>;

   // Free function for retrieving Pressure values
   template<typename MassT, typename LengthT1, typename TimeT1, typename TimeT2, typename PressureType>
   requires IsPressureUnits<MassT, LengthT1, TimeT1, TimeT2> && IsPressureType<PressureType>
   constexpr PrecisionType getPressure(const PressureType& obj)
   {
      return obj.template GetVal<std::tuple<MassT, LengthT1>, std::tuple<TimeT1, TimeT2>>();
   }

   template<typename NamedPressure, typename PressureType>
   requires IsNamedPressureUnit<NamedPressure> && IsPressureType<PressureType>
   constexpr PrecisionType getPressure(const PressureType& obj)
   {
      return obj.template GetVal<typename NamedPressure::NumTuple, typename NamedPressure::DenTuple>();
   }

   template<typename... Ts>
   class Pressure;

   template<typename MassUnit, typename LengthUnit1, typename TimeUnit1, typename TimeUnit2>
   requires IsPressureUnits<MassUnit, LengthUnit1, TimeUnit1, TimeUnit2>
   class Pressure<MassUnit, LengthUnit1, TimeUnit1, TimeUnit2> : public BaseDimension<std::tuple<MassUnit, LengthUnit1>, std::tuple<TimeUnit1, TimeUnit2>>
   {
   public:
      using Base = BaseDimension<std::tuple<MassUnit, LengthUnit1>, std::tuple<TimeUnit1, TimeUnit2>>;
      using Base::Base;

      constexpr Pressure(double val) : Base(val) {}

      template<typename NamedPressure>
      requires IsNamedPressureUnit<NamedPressure>
      constexpr Pressure(const NamedPressure& base) : Base(base) {}

      template<typename MassUnitRet, typename LengthUnitRet1, typename TimeUnitRet1, typename TimeUnitRet2>
      requires IsPressureUnits<MassUnitRet, LengthUnitRet1, TimeUnitRet1, TimeUnitRet2>
      [[deprecated("Use the free function getPressure() instead.")]]
      double GetPressure() const
      {
         return getPressure<MassUnitRet, LengthUnitRet1, TimeUnitRet1, TimeUnitRet2>(*this);
      }

      template<typename NamedPressure>
      requires IsNamedPressureUnit<NamedPressure>
      [[deprecated("Use the free function getPressure() instead.")]]
      double GetPressure() const
      {
         return getPressure<NamedPressure>(*this);
      }
   };

   // Pressure class for named units
   template<typename NamedPressure>
   requires IsNamedPressureUnit<NamedPressure>
   class Pressure<NamedPressure> : public BaseDimension<typename NamedPressure::NumTuple, typename NamedPressure::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedPressure::NumTuple, typename NamedPressure::DenTuple>;
      using Base::Base;

      constexpr Pressure(double val) : Base(val) {}

      template<typename OtherPressure>
      requires IsPressureType<OtherPressure>
      constexpr Pressure(const OtherPressure& base)
         : Base(base.template GetVal<typename NamedPressure::NumTuple, typename NamedPressure::DenTuple>()) {}

      template<typename MassUnitRet, typename LengthUnitRet1, typename TimeUnitRet1, typename TimeUnitRet2>
      requires IsPressureUnits<MassUnitRet, LengthUnitRet1, TimeUnitRet1, TimeUnitRet2>
      [[deprecated("Use the free function getPressure() instead.")]]
      double GetPressure() const
      {
         return getPressure<MassUnitRet, LengthUnitRet1, TimeUnitRet1, TimeUnitRet2>(*this);
      }

      template<typename NamedPressureUnit>
      requires IsNamedPressureUnit<NamedPressureUnit>
      [[deprecated("Use the free function getPressure() instead.")]]
      double GetPressure() const
      {
         return getPressure<NamedPressureUnit>(*this);
      }
   };

   template<typename MassUnit, typename LengthUnit1, typename TimeUnit1, typename TimeUnit2>
   requires IsPressureUnits<MassUnit, LengthUnit1, TimeUnit1, TimeUnit2>
   Pressure(MassUnit, LengthUnit1, TimeUnit1, TimeUnit2) -> Pressure<MassUnit, LengthUnit1, TimeUnit1, TimeUnit2>;

   template<typename NamedPressure>
   requires IsNamedPressureUnit<NamedPressure>
   Pressure(NamedPressure) -> Pressure<NamedPressure>;

   template<typename MassUnit, typename LengthUnit1, typename TimeUnit1, typename TimeUnit2>
   requires IsPressureUnits<MassUnit, LengthUnit1, TimeUnit1, TimeUnit2>
   Pressure(BaseDimension<std::tuple<MassUnit, LengthUnit1>, std::tuple<TimeUnit1, TimeUnit2>>) -> Pressure<MassUnit, LengthUnit1, TimeUnit1, TimeUnit2>;
}

#endif //STATIC_DIMENSION_PRESSURE_IMPL_H
