#ifndef STATIC_DIMENSION_ELECTRICPOTENTIAL_IMPL_H
#define STATIC_DIMENSION_ELECTRICPOTENTIAL_IMPL_H

#include "../../MassDimension.h"
#include "../../LengthDimension.h"
#include "../../ChargeDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named ElectricPotential unit.
   /// @tparam NamedElectricPotential The type to be checked as a named ElectricPotential unit.
   template<typename NamedElectricPotential>
   concept IsNamedElectricPotentialUnit = requires {
      typename NamedElectricPotential::NumTuple;
      typename NamedElectricPotential::DenTuple;
   };

   /// @brief Concept for a ElectricPotential dimension.
   /// @details Checks if the provided types satisfy the ElectricPotential dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Charge1>
   concept IsElectricPotentialUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType> &&
        std::is_same_v<typename Charge1::Dim, ChargeType>;

   /// @brief Concept for a ElectricPotential type.
   /// @details Ensures that the type meets ElectricPotential type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsElectricPotentialType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsElectricPotentialUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a ElectricPotential object.
   /// @details Provides access to the underlying value represented by a ElectricPotential object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam ElectricPotentialType The type of the object being accessed.
   /// @param obj The ElectricPotential object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Charge1, typename ElectricPotentialType>
   requires IsElectricPotentialUnits<Mass1, Length1, Length2,Time1, Time2, Charge1> && IsElectricPotentialType<ElectricPotentialType>
   constexpr PrecisionType getElectricPotential(const ElectricPotentialType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Charge1>>();
   }

   /// @brief Retrieves the value of a named ElectricPotential object.
   /// @details Provides access to the value represented by a named ElectricPotential object.
   /// @tparam NamedElectricPotential The named unit type.
   /// @tparam ElectricPotentialType The type of the object being accessed.
   /// @param obj The ElectricPotential object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedElectricPotential, typename ElectricPotentialType>
   requires IsNamedElectricPotentialUnit<NamedElectricPotential> && IsElectricPotentialType<ElectricPotentialType>
   constexpr PrecisionType getElectricPotential(const ElectricPotentialType& obj)
   {
      return obj.template GetVal<typename NamedElectricPotential::NumTuple, typename NamedElectricPotential::DenTuple>();
   }

   template<typename... Ts>
   class ElectricPotential;

   /// @brief Represents a default ElectricPotential.
   /// @details This ElectricPotential is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "ElectricPotential" type, without regard for the underlying units.
   template<>
   class ElectricPotential<> : public BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryCharge>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryCharge>>;
      using Base::Base;

      /// @brief Constructs a ElectricPotential object with a value.
      /// @param val The value of the ElectricPotential.
      explicit constexpr ElectricPotential(PrecisionType val) : Base(val) {}

      /// @brief Constructs a ElectricPotential object from another ElectricPotential object.
      /// @tparam OtherElectricPotential The other ElectricPotential type.
      /// @param base The base ElectricPotential object.
      template<typename OtherElectricPotential>
      requires IsElectricPotentialType<OtherElectricPotential>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr ElectricPotential(const OtherElectricPotential& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryCharge>>()) {}
   };

   /// @brief Represents a ElectricPotential.
   /// @details Defines operations and data storage for ElectricPotential dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Charge1>
   requires IsElectricPotentialUnits<Mass1, Length1, Length2,Time1, Time2, Charge1>
   class ElectricPotential<Mass1, Length1, Length2,Time1, Time2, Charge1> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Charge1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Charge1>>;
      using Base::Base;

      /// @brief Constructs a ElectricPotential object with a value.
      /// @param val The value of the ElectricPotential.
      explicit constexpr ElectricPotential(PrecisionType val) : Base(val) {}

      /// @brief Constructs a ElectricPotential object from a named unit.
      /// @tparam NamedElectricPotential The named unit type.
      /// @param base The base unit object.
      template<typename NamedElectricPotential>
      requires IsNamedElectricPotentialUnit<NamedElectricPotential>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr ElectricPotential(const NamedElectricPotential& base) : Base(base) {}

      /// @brief Deprecated function to get the value of ElectricPotential.
      /// @details Prefer using the free function `getElectricPotential()` instead.
      /// @return The value of the ElectricPotential.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Time2T, typename Charge1T>
      requires IsElectricPotentialUnits<Mass1T, Length1T, Length2T,Time1T, Time2T, Charge1T>
      [[deprecated("Use the free function getElectricPotential() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetElectricPotential() const
      {
         return getElectricPotential<Mass1T, Length1T, Length2T,Time1T, Time2T, Charge1T>(*this);
      }

      /// @brief Deprecated function to get the value of ElectricPotential.
      /// @details Prefer using the free function `getElectricPotential()` instead.
      /// @return The value of the ElectricPotential.
      template<typename NamedElectricPotential>
      requires IsNamedElectricPotentialUnit<NamedElectricPotential>
      [[deprecated("Use the free function getElectricPotential() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetElectricPotential() const
      {
         return getElectricPotential<NamedElectricPotential>(*this);
      }
   };

   /// @brief Represents a named ElectricPotential class.
   /// @details Provides functionality for named ElectricPotential units.
   /// @tparam NamedElectricPotential The named unit type.
   template<typename NamedElectricPotential>
   requires IsNamedElectricPotentialUnit<NamedElectricPotential>
   class ElectricPotential<NamedElectricPotential> : public BaseDimension<typename NamedElectricPotential::NumTuple, typename NamedElectricPotential::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedElectricPotential::NumTuple, typename NamedElectricPotential::DenTuple>;
      using Base::Base;

      /// @brief Constructs a ElectricPotential object with a value.
      /// @param val The value of the ElectricPotential.
      explicit constexpr ElectricPotential(PrecisionType val) : Base(val) {}

      /// @brief Constructs a ElectricPotential object from another ElectricPotential object.
      /// @tparam OtherElectricPotential The other ElectricPotential type.
      /// @param base The base ElectricPotential object.
      template<typename OtherElectricPotential>
      requires IsElectricPotentialType<OtherElectricPotential>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr ElectricPotential(const OtherElectricPotential& base)
         : Base(base.template GetVal<typename NamedElectricPotential::NumTuple, typename NamedElectricPotential::DenTuple>()) {}

      /// @brief Deprecated function to get the value of ElectricPotential.
      /// @details Prefer using the free function `getElectricPotential()` instead.
      /// @return The value of the ElectricPotential.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Time2T, typename Charge1T>
      requires IsElectricPotentialUnits<Mass1T, Length1T, Length2T,Time1T, Time2T, Charge1T>
      [[deprecated("Use the free function getElectricPotential() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetElectricPotential() const
      {
         return getElectricPotential<Mass1T, Length1T, Length2T,Time1T, Time2T, Charge1T>(*this);
      }

      /// @brief Deprecated function to get the value of ElectricPotential.
      /// @details Prefer using the free function `getElectricPotential()` instead.
      /// @return The value of the ElectricPotential.
      template<typename NamedElectricPotentialUnit>
      requires IsNamedElectricPotentialUnit<NamedElectricPotentialUnit>
      [[deprecated("Use the free function getElectricPotential() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetElectricPotential() const
      {
         return getElectricPotential<NamedElectricPotentialUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for ElectricPotential.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Charge1>
   requires IsElectricPotentialUnits<Mass1, Length1, Length2,Time1, Time2, Charge1>
   ElectricPotential(Mass1, Length1, Length2,Time1, Time2, Charge1) -> ElectricPotential<Mass1, Length1, Length2,Time1, Time2, Charge1>;

   /// @brief Template deduction guide for ElectricPotential.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename NamedElectricPotential>
   requires IsNamedElectricPotentialUnit<NamedElectricPotential>
   ElectricPotential(NamedElectricPotential) -> ElectricPotential<NamedElectricPotential>;

   /// @brief Template deduction guide for ElectricPotential.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Charge1>
   requires IsElectricPotentialUnits<Mass1, Length1, Length2,Time1, Time2, Charge1>
   ElectricPotential(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Charge1>>) -> ElectricPotential<Mass1, Length1, Length2,Time1, Time2, Charge1>;

}

#endif // STATIC_DIMENSION_ELECTRICPOTENTIAL_IMPL_H