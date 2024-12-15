#ifndef STATIC_DIMENSION_ENERGY_IMPL_H
#define STATIC_DIMENSION_ENERGY_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   // Concept for a named Energy unit (e.g., Newtons)
   template<typename NamedEnergy>
   concept IsNamedEnergyUnit = requires {
      typename NamedEnergy::NumTuple;
      typename NamedEnergy::DenTuple;
   };

   // Concept for a Energy dimension (basic units)
   template<typename MassUnit, typename LengthUnit1, typename LengthUnit2, typename TimeUnit1, typename TimeUnit2>
   concept IsEnergyUnits = 
      std::is_same_v<typename MassUnit::Dim, MassType> &&
      std::is_same_v<typename LengthUnit1::Dim, LengthType> &&
      std::is_same_v<typename LengthUnit2::Dim, LengthType> &&
      std::is_same_v<typename TimeUnit1::Dim, TimeType> &&
      std::is_same_v<typename TimeUnit2::Dim, TimeType>;

   // General concept for an Energy type
   template<typename T>
   concept IsEnergyType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && IsEnergyUnits<typename std::tuple_element_t<0, typename T::NumTuple>,
                     typename std::tuple_element_t<1, typename T::NumTuple>,
                     typename std::tuple_element_t<2, typename T::NumTuple>,
                     typename std::tuple_element_t<0, typename T::DenTuple>,
                     typename std::tuple_element_t<1, typename T::DenTuple>>;

   // Free function for retrieving Energy values
   template<typename MassT, typename LengthT1, typename LengthT2, typename TimeT1, typename TimeT2, typename EnergyType>
   requires IsEnergyUnits<MassT, LengthT1, LengthT2, TimeT1, TimeT2> && IsEnergyType<EnergyType>
   PrecisionType getEnergy(const EnergyType& obj)
   {
      return obj.template GetVal<std::tuple<MassT, LengthT1, LengthT2>, std::tuple<TimeT1, TimeT2>>();
   }

   template<typename NamedEnergy, typename EnergyType>
   requires IsNamedEnergyUnit<NamedEnergy> && IsEnergyType<EnergyType>
   PrecisionType getEnergy(const EnergyType& obj)
   {
      return obj.template GetVal<typename NamedEnergy::NumTuple, typename NamedEnergy::DenTuple>();
   }

   template<typename... Ts>
   class Energy;

   template<typename MassUnit, typename LengthUnit1, typename LengthUnit2, typename TimeUnit1, typename TimeUnit2>
   requires IsEnergyUnits<MassUnit, LengthUnit1, LengthUnit2, TimeUnit1, TimeUnit2>
   class Energy<MassUnit, LengthUnit1, LengthUnit2, TimeUnit1, TimeUnit2> : public BaseDimension<std::tuple<MassUnit, LengthUnit1, LengthUnit2>, std::tuple<TimeUnit1, TimeUnit2>>
   {
   public:
      using Base = BaseDimension<std::tuple<MassUnit, LengthUnit1, LengthUnit2>, std::tuple<TimeUnit1, TimeUnit2>>;
      using Base::Base;

      Energy(double val) : Base(val) {}

      template<typename NamedEnergy>
      requires IsNamedEnergyUnit<NamedEnergy>
      Energy(const NamedEnergy& base) : Base(base) {}

      template<typename MassUnitRet, typename LengthUnitRet1, typename LengthUnitRet2, typename TimeUnitRet1, typename TimeUnitRet2>
      requires IsEnergyUnits<MassUnitRet, LengthUnitRet1, LengthUnitRet2, TimeUnitRet1, TimeUnitRet2>
      [[deprecated("Use the free function getEnergy() instead.")]]
      double GetEnergy() const
      {
         return getEnergy<MassUnitRet, LengthUnitRet1, LengthUnitRet2, TimeUnitRet1, TimeUnitRet2>(*this);
      }

      template<typename NamedEnergy>
      requires IsNamedEnergyUnit<NamedEnergy>
      [[deprecated("Use the free function getEnergy() instead.")]]
      double GetEnergy() const
      {
         return getEnergy<NamedEnergy>(*this);
      }
   };

   // Energy class for named units
   template<typename NamedEnergy>
   requires IsNamedEnergyUnit<NamedEnergy>
   class Energy<NamedEnergy> : public BaseDimension<typename NamedEnergy::NumTuple, typename NamedEnergy::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedEnergy::NumTuple, typename NamedEnergy::DenTuple>;
      using Base::Base;

      Energy(double val) : Base(val) {}

      template<typename OtherEnergy>
      requires IsEnergyType<OtherEnergy>
      Energy(const OtherEnergy& base)
         : Base(base.template GetVal<typename NamedEnergy::NumTuple, typename NamedEnergy::DenTuple>()) {}

      template<typename MassUnitRet, typename LengthUnitRet1, typename LengthUnitRet2, typename TimeUnitRet1, typename TimeUnitRet2>
      requires IsEnergyUnits<MassUnitRet, LengthUnitRet1, LengthUnitRet2, TimeUnitRet1, TimeUnitRet2>
      [[deprecated("Use the free function getEnergy() instead.")]]
      double GetEnergy() const
      {
         return getEnergy<MassUnitRet, LengthUnitRet1, LengthUnitRet2, TimeUnitRet1, TimeUnitRet2>(*this);
      }

      template<typename NamedEnergyUnit>
      requires IsNamedEnergyUnit<NamedEnergyUnit>
      [[deprecated("Use the free function getEnergy() instead.")]]
      double GetEnergy() const
      {
         return getEnergy<NamedEnergyUnit>(*this);
      }
   };

   template<typename MassUnit, typename LengthUnit1, typename LengthUnit2, typename TimeUnit1, typename TimeUnit2>
   requires IsEnergyUnits<MassUnit, LengthUnit1, LengthUnit2, TimeUnit1, TimeUnit2>
   Energy(MassUnit, LengthUnit1, LengthUnit2, TimeUnit1, TimeUnit2) -> Energy<MassUnit, LengthUnit1, LengthUnit2, TimeUnit1, TimeUnit2>;

   template<typename NamedEnergy>
   requires IsNamedEnergyUnit<NamedEnergy>
   Energy(NamedEnergy) -> Energy<NamedEnergy>;

   template<typename MassUnit, typename LengthUnit1, typename LengthUnit2, typename TimeUnit1, typename TimeUnit2>
   requires IsEnergyUnits<MassUnit, LengthUnit1, LengthUnit2, TimeUnit1, TimeUnit2>
   Energy(BaseDimension<std::tuple<MassUnit, LengthUnit1, LengthUnit2>, std::tuple<TimeUnit1, TimeUnit2>>) -> Energy<MassUnit, LengthUnit1, LengthUnit2, TimeUnit1, TimeUnit2>;
}

#endif //STATIC_DIMENSION_ENERGY_IMPL_H
