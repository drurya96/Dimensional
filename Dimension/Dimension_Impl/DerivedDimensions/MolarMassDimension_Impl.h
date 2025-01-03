#ifndef STATIC_DIMENSION_MOLARMASS_IMPL_H
#define STATIC_DIMENSION_MOLARMASS_IMPL_H

#include "../../MassDimension.h"
#include "../../AmountDimension.h"

namespace Dimension
{
   // Concept for a named MolarMass unit
   template<typename NamedMolarMass>
   concept IsNamedMolarMassUnit = requires {
      typename NamedMolarMass::NumTuple;
      typename NamedMolarMass::DenTuple;
   };

   // Concept for a MolarMass dimension (basic units)
   template<typename MassUnit, typename AmountUnit>
   concept IsMolarMassUnits = 
      std::is_same_v<typename MassUnit::Dim, MassType> &&
      std::is_same_v<typename AmountUnit::Dim, AmountType>;

   // General concept for an MolarMass type
   template<typename T>
   concept IsMolarMassType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && IsMolarMassUnits<typename std::tuple_element_t<0, typename T::NumTuple>,
                         typename std::tuple_element_t<0, typename T::DenTuple>>;

   // Free function for retrieving MolarMass values
   template<typename MassT, typename AmountT, typename MolarMassType>
   requires IsMolarMassUnits<MassT, AmountT> && IsMolarMassType<MolarMassType>
   constexpr PrecisionType getMolarMass(const MolarMassType& obj)
   {
      return obj.template GetVal<std::tuple<MassT>, std::tuple<AmountT>>();
   }

   template<typename NamedMolarMass, typename MolarMassType>
   requires IsNamedMolarMassUnit<NamedMolarMass> && IsMolarMassType<MolarMassType>
   constexpr PrecisionType getMolarMass(const MolarMassType& obj)
   {
      return obj.template GetVal<typename NamedMolarMass::NumTuple, typename NamedMolarMass::DenTuple>();
   }

   template<typename... Ts>
   class MolarMass;

   template<typename MassUnit, typename AmountUnit>
   requires IsMolarMassUnits<MassUnit, AmountUnit>
   class MolarMass<MassUnit, AmountUnit> : public BaseDimension<std::tuple<MassUnit>, std::tuple<AmountUnit>>
   {
   public:
      using Base = BaseDimension<std::tuple<MassUnit>, std::tuple<AmountUnit>>;
      using Base::Base;

      constexpr MolarMass(double val) : Base(val) {}

      template<typename NamedMolarMass>
      requires IsNamedMolarMassUnit<NamedMolarMass>
      constexpr MolarMass(const NamedMolarMass& base) : Base(base) {}

      template<typename MassUnitRet, typename AmountUnitRet>
      requires IsMolarMassUnits<MassUnitRet, AmountUnitRet>
      [[deprecated("Use the free function getMolarMass() instead.")]]
      double GetMolarMass() const
      {
         return getMolarMass<MassUnitRet, AmountUnitRet>(*this);
      }

      template<typename NamedMolarMass>
      requires IsNamedMolarMassUnit<NamedMolarMass>
      [[deprecated("Use the free function getMolarMass() instead.")]]
      double GetMolarMass() const
      {
         return getMolarMass<NamedMolarMass>(*this);
      }
   };

   // MolarMass class for named units
   template<typename NamedMolarMass>
   requires IsNamedMolarMassUnit<NamedMolarMass>
   class MolarMass<NamedMolarMass> : public BaseDimension<typename NamedMolarMass::NumTuple, typename NamedMolarMass::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedMolarMass::NumTuple, typename NamedMolarMass::DenTuple>;
      using Base::Base;

      constexpr MolarMass(double val) : Base(val) {}

      template<typename OtherMolarMass>
      requires IsMolarMassType<OtherMolarMass>
      constexpr MolarMass(const OtherMolarMass& base)
         : Base(base.template GetVal<typename NamedMolarMass::NumTuple, typename NamedMolarMass::DenTuple>()) {}

      template<typename MassUnitRet, typename AmountUnitRet>
      requires IsMolarMassUnits<MassUnitRet, AmountUnitRet>
      [[deprecated("Use the free function getMolarMass() instead.")]]
      double GetMolarMass() const
      {
         return getMolarMass<MassUnitRet, AmountUnitRet>(*this);
      }

      template<typename NamedMolarMassUnit>
      requires IsNamedMolarMassUnit<NamedMolarMassUnit>
      [[deprecated("Use the free function getMolarMass() instead.")]]
      double GetMolarMass() const
      {
         return getMolarMass<NamedMolarMassUnit>(*this);
      }
   };

   template<typename MassUnit, typename AmountUnit>
   requires IsMolarMassUnits<MassUnit, AmountUnit>
   MolarMass(MassUnit, AmountUnit) -> MolarMass<MassUnit, AmountUnit>;

   template<typename NamedMolarMass>
   requires IsNamedMolarMassUnit<NamedMolarMass>
   MolarMass(NamedMolarMass) -> MolarMass<NamedMolarMass>;

   template<typename MassUnit, typename AmountUnit>
   requires IsMolarMassUnits<MassUnit, AmountUnit>
   MolarMass(BaseDimension<std::tuple<MassUnit>, std::tuple<AmountUnit>>) -> MolarMass<MassUnit, AmountUnit>;
}

#endif //STATIC_DIMENSION_MOLARMASS_IMPL_H
