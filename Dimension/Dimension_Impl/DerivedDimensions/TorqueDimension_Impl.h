#ifndef STATIC_DIMENSION_TORQUE_IMPL_H
#define STATIC_DIMENSION_TORQUE_IMPL_H

#include "../../LengthDimension.h"
#include "../../TimeDimension.h"
#include "../../MassDimension.h"
#include "../../AngleDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Torque unit.
   /// @tparam NamedTorque The type to be checked as a named Torque unit.
   template<typename NamedTorque>
   concept IsNamedTorqueUnit = requires {
      typename NamedTorque::NumTuple;
      typename NamedTorque::DenTuple;
   };

   /// @brief Concept for a Torque dimension.
   /// @details Checks if the provided types satisfy the Torque dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Angle1 Denominator Angle1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Angle1>
   concept IsTorqueUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType> &&
        std::is_same_v<typename Angle1::Dim, AngleType>;

   /// @brief Concept for a Torque type.
   /// @details Ensures that the type meets Torque type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsTorqueType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsTorqueUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Torque object.
   /// @details Provides access to the underlying value represented by a Torque object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Angle1 Denominator Angle1 type
   /// @tparam TorqueType The type of the object being accessed.
   /// @param obj The Torque object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Angle1, typename TorqueType>
   requires IsTorqueUnits<Mass1, Length1, Length2,Time1, Time2, Angle1> && IsTorqueType<TorqueType>
   constexpr PrecisionType getTorque(const TorqueType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Angle1>>();
   }

   /// @brief Retrieves the value of a named Torque object.
   /// @details Provides access to the value represented by a named Torque object.
   /// @tparam NamedTorque The named unit type.
   /// @tparam TorqueType The type of the object being accessed.
   /// @param obj The Torque object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedTorque, typename TorqueType>
   requires IsNamedTorqueUnit<NamedTorque> && IsTorqueType<TorqueType>
   constexpr PrecisionType getTorque(const TorqueType& obj)
   {
      return obj.template GetVal<typename NamedTorque::NumTuple, typename NamedTorque::DenTuple>();
   }

   template<typename... Ts>
   class Torque;

   /// @brief Represents a default Torque.
   /// @details This Torque is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "Torque" type, without regard for the underlying units.
   template<>
   class Torque<> : public BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryAngle>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryAngle>>;
      using Base::Base;

      /// @brief Constructs a Torque object with a value.
      /// @param val The value of the Torque.
      explicit constexpr Torque(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Torque object from another Torque object.
      /// @tparam OtherTorque The other Torque type.
      /// @param base The base Torque object.
      template<typename OtherTorque>
      requires IsTorqueType<OtherTorque>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Torque(const OtherTorque& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass, PrimaryLength, PrimaryLength>, std::tuple<PrimaryTime, PrimaryTime, PrimaryAngle>>()) {}
   };

   /// @brief Represents a Torque.
   /// @details Defines operations and data storage for Torque dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Angle1 Denominator Angle1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Angle1>
   requires IsTorqueUnits<Mass1, Length1, Length2,Time1, Time2, Angle1>
   class Torque<Mass1, Length1, Length2,Time1, Time2, Angle1> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Angle1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Angle1>>;
      using Base::Base;

      /// @brief Constructs a Torque object with a value.
      /// @param val The value of the Torque.
      explicit constexpr Torque(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Torque object from a named unit.
      /// @tparam NamedTorque The named unit type.
      /// @param base The base unit object.
      template<typename NamedTorque>
      requires IsNamedTorqueUnit<NamedTorque>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Torque(const NamedTorque& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Torque.
      /// @details Prefer using the free function `getTorque()` instead.
      /// @return The value of the Torque.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Time2T, typename Angle1T>
      requires IsTorqueUnits<Mass1T, Length1T, Length2T,Time1T, Time2T, Angle1T>
      [[deprecated("Use the free function getTorque() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetTorque() const
      {
         return getTorque<Mass1T, Length1T, Length2T,Time1T, Time2T, Angle1T>(*this);
      }

      /// @brief Deprecated function to get the value of Torque.
      /// @details Prefer using the free function `getTorque()` instead.
      /// @return The value of the Torque.
      template<typename NamedTorque>
      requires IsNamedTorqueUnit<NamedTorque>
      [[deprecated("Use the free function getTorque() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetTorque() const
      {
         return getTorque<NamedTorque>(*this);
      }
   };

   /// @brief Represents a named Torque class.
   /// @details Provides functionality for named Torque units.
   /// @tparam NamedTorque The named unit type.
   template<typename NamedTorque>
   requires IsNamedTorqueUnit<NamedTorque>
   class Torque<NamedTorque> : public BaseDimension<typename NamedTorque::NumTuple, typename NamedTorque::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedTorque::NumTuple, typename NamedTorque::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Torque object with a value.
      /// @param val The value of the Torque.
      explicit constexpr Torque(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Torque object from another Torque object.
      /// @tparam OtherTorque The other Torque type.
      /// @param base The base Torque object.
      template<typename OtherTorque>
      requires IsTorqueType<OtherTorque>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr Torque(const OtherTorque& base)
         : Base(base.template GetVal<typename NamedTorque::NumTuple, typename NamedTorque::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Torque.
      /// @details Prefer using the free function `getTorque()` instead.
      /// @return The value of the Torque.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Time2T, typename Angle1T>
      requires IsTorqueUnits<Mass1T, Length1T, Length2T,Time1T, Time2T, Angle1T>
      [[deprecated("Use the free function getTorque() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetTorque() const
      {
         return getTorque<Mass1T, Length1T, Length2T,Time1T, Time2T, Angle1T>(*this);
      }

      /// @brief Deprecated function to get the value of Torque.
      /// @details Prefer using the free function `getTorque()` instead.
      /// @return The value of the Torque.
      template<typename NamedTorqueUnit>
      requires IsNamedTorqueUnit<NamedTorqueUnit>
      [[deprecated("Use the free function getTorque() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetTorque() const
      {
         return getTorque<NamedTorqueUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Torque.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Angle1 Denominator Angle1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Angle1>
   requires IsTorqueUnits<Mass1, Length1, Length2,Time1, Time2, Angle1>
   Torque(Mass1, Length1, Length2,Time1, Time2, Angle1) -> Torque<Mass1, Length1, Length2,Time1, Time2, Angle1>;

   /// @brief Template deduction guide for Torque.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Angle1 Denominator Angle1 type
   template<typename NamedTorque>
   requires IsNamedTorqueUnit<NamedTorque>
   Torque(NamedTorque) -> Torque<NamedTorque>;

   /// @brief Template deduction guide for Torque.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Angle1 Denominator Angle1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Time2, typename Angle1>
   requires IsTorqueUnits<Mass1, Length1, Length2,Time1, Time2, Angle1>
   Torque(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Time2, Angle1>>) -> Torque<Mass1, Length1, Length2,Time1, Time2, Angle1>;

}

#endif // STATIC_DIMENSION_TORQUE_IMPL_H