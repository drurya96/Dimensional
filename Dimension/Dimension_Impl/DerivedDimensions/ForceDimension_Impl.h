#ifndef STATIC_DIMENSION_FORCE_IMPL_H
#define STATIC_DIMENSION_FORCE_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{

   // Concept for a named force unit (e.g., Newtons)
   template<typename NamedForce>
   concept IsNamedForceUnit = requires {
      typename NamedForce::NumTuple;
      typename NamedForce::DenTuple;
   };

   // Concept for a force dimension (basic units)
   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   concept IsForceUnits = 
      std::is_same_v<typename MassUnit::Dim, MassType> &&
      std::is_same_v<typename LengthUnit::Dim, LengthType> &&
      std::is_same_v<typename TimeUnit1::Dim, TimeType> &&
      std::is_same_v<typename TimeUnit2::Dim, TimeType>;

   // General concept for a force type
   template<typename T>
   concept IsForceType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && IsForceUnits<typename std::tuple_element_t<0, typename T::NumTuple>,
                     typename std::tuple_element_t<1, typename T::NumTuple>,
                     typename std::tuple_element_t<0, typename T::DenTuple>,
                     typename std::tuple_element_t<1, typename T::DenTuple>>;

   // Free function for retrieving Force values
   template<typename MassT, typename LengthT, typename TimeT1, typename TimeT2, typename ForceType>
   requires IsForceUnits<MassT, LengthT, TimeT1, TimeT2> && IsForceType<ForceType>
   constexpr PrecisionType getForce(const ForceType& obj)
   {
      return obj.template GetVal<std::tuple<MassT, LengthT>, std::tuple<TimeT1, TimeT2>>();
   }

   template<typename NamedForce, typename ForceType>
   requires IsNamedForceUnit<NamedForce> && IsForceType<ForceType>
   constexpr PrecisionType getForce(const ForceType& obj)
   {
      return obj.template GetVal<typename NamedForce::NumTuple, typename NamedForce::DenTuple>();
   }

   template<typename... Ts>
   class Force;

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsForceUnits<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>
   class Force<MassUnit, LengthUnit, TimeUnit1, TimeUnit2> : public BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>
   {
   public:
      using Base = BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>;
      using Base::Base;

      constexpr Force(double val) : Base(val) {}

      template<typename NamedForce>
      requires IsNamedForceUnit<NamedForce>
      constexpr Force(const NamedForce& base) : Base(base) {}

      template<typename MassUnitRet, typename LengthUnitRet, typename TimeUnitRet1, typename TimeUnitRet2>
      requires IsForceUnits<MassUnitRet, LengthUnitRet, TimeUnitRet1, TimeUnitRet2>
      [[deprecated("Use the free function getForce() instead.")]]
      double GetForce() const
      {
         return getForce<MassUnitRet, LengthUnitRet, TimeUnitRet1, TimeUnitRet2>(*this);
      }

      template<typename NamedForce>
      requires IsNamedForceUnit<NamedForce>
      [[deprecated("Use the free function getForce() instead.")]]
      double GetForce() const
      {
         return getForce<NamedForce>(*this);
      }
   };

   // Force class for named units
   template<typename NamedForce>
   requires IsNamedForceUnit<NamedForce>
   class Force<NamedForce> : public BaseDimension<typename NamedForce::NumTuple, typename NamedForce::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedForce::NumTuple, typename NamedForce::DenTuple>;
      using Base::Base;

      constexpr Force(double val) : Base(val) {}

      template<typename OtherForce>
      requires IsForceType<OtherForce>
      constexpr Force(const OtherForce& base)
         : Base(base.template GetVal<typename NamedForce::NumTuple, typename NamedForce::DenTuple>()) {}

      template<typename MassUnitRet, typename LengthUnitRet, typename TimeUnitRet1, typename TimeUnitRet2>
      requires IsForceUnits<MassUnitRet, LengthUnitRet, TimeUnitRet1, TimeUnitRet2>
      [[deprecated("Use the free function getForce() instead.")]]
      double GetForce() const
      {
         return getForce<MassUnitRet, LengthUnitRet, TimeUnitRet1, TimeUnitRet2>(*this);
      }

      template<typename NamedForceUnit>
      requires IsNamedForceUnit<NamedForceUnit>
      [[deprecated("Use the free function getForce() instead.")]]
      double GetForce() const
      {
         return getForce<NamedForceUnit>(*this);
      }
   };

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsForceUnits<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>
   Force(MassUnit, LengthUnit, TimeUnit1, TimeUnit2) -> Force<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>;

   template<typename NamedForce>
   requires IsNamedForceUnit<NamedForce>
   Force(NamedForce) -> Force<NamedForce>;

   template<typename MassUnit, typename LengthUnit, typename TimeUnit1, typename TimeUnit2>
   requires IsForceUnits<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>
   Force(BaseDimension<std::tuple<MassUnit, LengthUnit>, std::tuple<TimeUnit1, TimeUnit2>>) -> Force<MassUnit, LengthUnit, TimeUnit1, TimeUnit2>;

}

#endif //STATIC_DIMENSION_FORCE_IMPL_H
