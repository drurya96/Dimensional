#ifndef STATIC_DIMENSION_ENERGY_IMPL_H
#define STATIC_DIMENSION_ENERGY_IMPL_H

#include "../../TimeDimension.h"
#include "../../LengthDimension.h"
#include "../../MassDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Energy unit.
   /// @tparam NamedEnergy The type to be checked as a named Energy unit.
   template<typename NamedEnergy>
   concept IsNamedEnergyUnit = requires {
      typename NamedEnergy::NumTuple;
      typename NamedEnergy::DenTuple;
   };

   /// @brief Concept for a Energy dimension.
   /// @details Checks if the provided types satisfy the Energy dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2>
   concept IsEnergyUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType>;

   /// @brief Concept for a Energy type.
   /// @details Ensures that the type meets Energy type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsEnergyType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 2 &&
   IsEnergyUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Energy object.
   /// @details Provides access to the underlying value represented by a Energy object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam EnergyType The type of the object being accessed.
   /// @param obj The Energy object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename EnergyType>
   requires IsEnergyUnits<Mass1, Length1, Length2,Time1, Time2> && IsEnergyType<EnergyType>
   constexpr PrecisionType getEnergy(const EnergyType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2>>();
   }

   /// @brief Retrieves the value of a named Energy object.
   /// @details Provides access to the value represented by a named Energy object.
   /// @tparam NamedEnergy The named unit type.
   /// @tparam EnergyType The type of the object being accessed.
   /// @param obj The Energy object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedEnergy, typename EnergyType>
   requires IsNamedEnergyUnit<NamedEnergy> && IsEnergyType<EnergyType>
   constexpr PrecisionType getEnergy(const EnergyType& obj)
   {
      return obj.template GetVal<typename NamedEnergy::NumTuple, typename NamedEnergy::DenTuple>();
   }

   template<typename... Ts>
   class Energy;

   /// @brief Represents a Energy.
   /// @details Defines operations and data storage for Energy dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2>
   requires IsEnergyUnits<Mass1, Length1, Length2,Time1, Time2>
   class Energy<Mass1, Length1, Length2,Time1, Time2> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2>>;
      using Base::Base;

      /// @brief Constructs a Energy object with a value.
      /// @param val The value of the Energy.
      constexpr Energy(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Energy object from a named unit.
      /// @tparam NamedEnergy The named unit type.
      /// @param base The base unit object.
      template<typename NamedEnergy>
      requires IsNamedEnergyUnit<NamedEnergy>
      constexpr Energy(const NamedEnergy& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Energy.
      /// @details Prefer using the free function `getEnergy()` instead.
      /// @return The value of the Energy.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Time2T>
      requires IsEnergyUnits<Mass1T, Length1T, Length2T,Time1T, Time2T>
      [[deprecated("Use the free function getEnergy() instead.")]]
      double GetEnergy() const
      {
         return getEnergy<Mass1T, Length1T, Length2T,Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of Energy.
      /// @details Prefer using the free function `getEnergy()` instead.
      /// @return The value of the Energy.
      template<typename NamedEnergy>
      requires IsNamedEnergyUnit<NamedEnergy>
      [[deprecated("Use the free function getEnergy() instead.")]]
      double GetEnergy() const
      {
         return getEnergy<NamedEnergy>(*this);
      }
   };

   /// @brief Represents a named Energy class.
   /// @details Provides functionality for named Energy units.
   /// @tparam NamedEnergy The named unit type.
   template<typename NamedEnergy>
   requires IsNamedEnergyUnit<NamedEnergy>
   class Energy<NamedEnergy> : public BaseDimension<typename NamedEnergy::NumTuple, typename NamedEnergy::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedEnergy::NumTuple, typename NamedEnergy::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Energy object with a value.
      /// @param val The value of the Energy.
      constexpr Energy(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Energy object from another Energy object.
      /// @tparam OtherEnergy The other Energy type.
      /// @param base The base Energy object.
      template<typename OtherEnergy>
      requires IsEnergyType<OtherEnergy>
      constexpr Energy(const OtherEnergy& base)
         : Base(base.template GetVal<typename NamedEnergy::NumTuple, typename NamedEnergy::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Energy.
      /// @details Prefer using the free function `getEnergy()` instead.
      /// @return The value of the Energy.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Time2T>
      requires IsEnergyUnits<Mass1T, Length1T, Length2T,Time1T, Time2T>
      [[deprecated("Use the free function getEnergy() instead.")]]
      double GetEnergy() const
      {
         return getEnergy<Mass1T, Length1T, Length2T,Time1T, Time2T>(*this);
      }

      /// @brief Deprecated function to get the value of Energy.
      /// @details Prefer using the free function `getEnergy()` instead.
      /// @return The value of the Energy.
      template<typename NamedEnergyUnit>
      requires IsNamedEnergyUnit<NamedEnergyUnit>
      [[deprecated("Use the free function getEnergy() instead.")]]
      double GetEnergy() const
      {
         return getEnergy<NamedEnergyUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Energy.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2>
   requires IsEnergyUnits<Mass1, Length1, Length2,Time1, Time2>
   Energy(Mass1, Length1, Length2,Time1, Time2) -> Energy<Mass1, Length1, Length2,Time1, Time2>;

   /// @brief Template deduction guide for Energy.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename NamedEnergy>
   requires IsNamedEnergyUnit<NamedEnergy>
   Energy(NamedEnergy) -> Energy<NamedEnergy>;

   /// @brief Template deduction guide for Energy.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2>
   requires IsEnergyUnits<Mass1, Length1, Length2,Time1, Time2>
   Energy(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2>>) -> Energy<Mass1, Length1, Length2,Time1, Time2>;

}

#endif // STATIC_DIMENSION_ENERGY_IMPL_H