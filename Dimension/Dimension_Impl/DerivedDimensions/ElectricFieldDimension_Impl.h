#ifndef STATIC_DIMENSION_ELECTRICFIELD_IMPL_H
#define STATIC_DIMENSION_ELECTRICFIELD_IMPL_H

#include "../../TimeDimension.h"
#include "../../LengthDimension.h"
#include "../../ChargeDimension.h"
#include "../../MassDimension.h"

namespace Dimension
{
   /// @brief Concept for a named ElectricField unit.
   /// @tparam NamedElectricField The type to be checked as a named ElectricField unit.
   template<typename NamedElectricField>
   concept IsNamedElectricFieldUnit = requires {
      typename NamedElectricField::NumTuple;
      typename NamedElectricField::DenTuple;
   };

   /// @brief Concept for a ElectricField dimension.
   /// @details Checks if the provided types satisfy the ElectricField dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1,typename Time1, typename Time2, typename Charge1>
   concept IsElectricFieldUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType> &&
        std::is_same_v<typename Charge1::Dim, ChargeType>;

   /// @brief Concept for a ElectricField type.
   /// @details Ensures that the type meets ElectricField type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsElectricFieldType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 2 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsElectricFieldUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a ElectricField object.
   /// @details Provides access to the underlying value represented by a ElectricField object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam ElectricFieldType The type of the object being accessed.
   /// @param obj The ElectricField object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1,typename Time1, typename Time2, typename Charge1, typename ElectricFieldType>
   requires IsElectricFieldUnits<Mass1, Length1,Time1, Time2, Charge1> && IsElectricFieldType<ElectricFieldType>
   constexpr PrecisionType getElectricField(const ElectricFieldType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1>, std::tuple<Time1, Time2, Charge1>>();
   }

   /// @brief Retrieves the value of a named ElectricField object.
   /// @details Provides access to the value represented by a named ElectricField object.
   /// @tparam NamedElectricField The named unit type.
   /// @tparam ElectricFieldType The type of the object being accessed.
   /// @param obj The ElectricField object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedElectricField, typename ElectricFieldType>
   requires IsNamedElectricFieldUnit<NamedElectricField> && IsElectricFieldType<ElectricFieldType>
   constexpr PrecisionType getElectricField(const ElectricFieldType& obj)
   {
      return obj.template GetVal<typename NamedElectricField::NumTuple, typename NamedElectricField::DenTuple>();
   }

   template<typename... Ts>
   class ElectricField;

   /// @brief Represents a default ElectricField.
   /// @details This ElectricField is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "ElectricField" type, without regard for the underlying units.
   template<>
   class ElectricField<> : public BaseDimension<std::tuple<PrimaryMass, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryCharge>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryCharge>>;
      using Base::Base;

      /// @brief Constructs a ElectricField object with a value.
      /// @param val The value of the ElectricField.
      explicit constexpr ElectricField(PrecisionType val) : Base(val) {}

      /// @brief Constructs a ElectricField object from another ElectricField object.
      /// @tparam OtherElectricField The other ElectricField type.
      /// @param base The base ElectricField object.
      template<typename OtherElectricField>
      requires IsElectricFieldType<OtherElectricField>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr ElectricField(const OtherElectricField& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryCharge>>()) {}
   };

   /// @brief Represents a ElectricField.
   /// @details Defines operations and data storage for ElectricField dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1,typename Time1, typename Time2, typename Charge1>
   requires IsElectricFieldUnits<Mass1, Length1,Time1, Time2, Charge1>
   class ElectricField<Mass1, Length1,Time1, Time2, Charge1> : public BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1, Time2, Charge1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1, Time2, Charge1>>;
      using Base::Base;

      /// @brief Constructs a ElectricField object with a value.
      /// @param val The value of the ElectricField.
      explicit constexpr ElectricField(PrecisionType val) : Base(val) {}

      /// @brief Constructs a ElectricField object from a named unit.
      /// @tparam NamedElectricField The named unit type.
      /// @param base The base unit object.
      template<typename NamedElectricField>
      requires IsNamedElectricFieldUnit<NamedElectricField>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr ElectricField(const NamedElectricField& base) : Base(base) {}

      /// @brief Deprecated function to get the value of ElectricField.
      /// @details Prefer using the free function `getElectricField()` instead.
      /// @return The value of the ElectricField.
      template<typename Mass1T, typename Length1T,typename Time1T, typename Time2T, typename Charge1T>
      requires IsElectricFieldUnits<Mass1T, Length1T,Time1T, Time2T, Charge1T>
      [[deprecated("Use the free function getElectricField() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetElectricField() const
      {
         return getElectricField<Mass1T, Length1T,Time1T, Time2T, Charge1T>(*this);
      }

      /// @brief Deprecated function to get the value of ElectricField.
      /// @details Prefer using the free function `getElectricField()` instead.
      /// @return The value of the ElectricField.
      template<typename NamedElectricField>
      requires IsNamedElectricFieldUnit<NamedElectricField>
      [[deprecated("Use the free function getElectricField() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetElectricField() const
      {
         return getElectricField<NamedElectricField>(*this);
      }
   };

   /// @brief Represents a named ElectricField class.
   /// @details Provides functionality for named ElectricField units.
   /// @tparam NamedElectricField The named unit type.
   template<typename NamedElectricField>
   requires IsNamedElectricFieldUnit<NamedElectricField>
   class ElectricField<NamedElectricField> : public BaseDimension<typename NamedElectricField::NumTuple, typename NamedElectricField::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedElectricField::NumTuple, typename NamedElectricField::DenTuple>;
      using Base::Base;

      /// @brief Constructs a ElectricField object with a value.
      /// @param val The value of the ElectricField.
      explicit constexpr ElectricField(PrecisionType val) : Base(val) {}

      /// @brief Constructs a ElectricField object from another ElectricField object.
      /// @tparam OtherElectricField The other ElectricField type.
      /// @param base The base ElectricField object.
      template<typename OtherElectricField>
      requires IsElectricFieldType<OtherElectricField>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr ElectricField(const OtherElectricField& base)
         : Base(base.template GetVal<typename NamedElectricField::NumTuple, typename NamedElectricField::DenTuple>()) {}

      /// @brief Deprecated function to get the value of ElectricField.
      /// @details Prefer using the free function `getElectricField()` instead.
      /// @return The value of the ElectricField.
      template<typename Mass1T, typename Length1T,typename Time1T, typename Time2T, typename Charge1T>
      requires IsElectricFieldUnits<Mass1T, Length1T,Time1T, Time2T, Charge1T>
      [[deprecated("Use the free function getElectricField() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetElectricField() const
      {
         return getElectricField<Mass1T, Length1T,Time1T, Time2T, Charge1T>(*this);
      }

      /// @brief Deprecated function to get the value of ElectricField.
      /// @details Prefer using the free function `getElectricField()` instead.
      /// @return The value of the ElectricField.
      template<typename NamedElectricFieldUnit>
      requires IsNamedElectricFieldUnit<NamedElectricFieldUnit>
      [[deprecated("Use the free function getElectricField() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetElectricField() const
      {
         return getElectricField<NamedElectricFieldUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for ElectricField.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1,typename Time1, typename Time2, typename Charge1>
   requires IsElectricFieldUnits<Mass1, Length1,Time1, Time2, Charge1>
   ElectricField(Mass1, Length1,Time1, Time2, Charge1) -> ElectricField<Mass1, Length1,Time1, Time2, Charge1>;

   /// @brief Template deduction guide for ElectricField.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename NamedElectricField>
   requires IsNamedElectricFieldUnit<NamedElectricField>
   ElectricField(NamedElectricField) -> ElectricField<NamedElectricField>;

   /// @brief Template deduction guide for ElectricField.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1,typename Time1, typename Time2, typename Charge1>
   requires IsElectricFieldUnits<Mass1, Length1,Time1, Time2, Charge1>
   ElectricField(BaseDimension<std::tuple<Mass1, Length1>, std::tuple<Time1, Time2, Charge1>>) -> ElectricField<Mass1, Length1,Time1, Time2, Charge1>;

}

#endif // STATIC_DIMENSION_ELECTRICFIELD_IMPL_H