#ifndef STATIC_DIMENSION_CURRENT_IMPL_H
#define STATIC_DIMENSION_CURRENT_IMPL_H

#include "../../TimeDimension.h"
#include "../../ChargeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Current unit.
   /// @tparam NamedCurrent The type to be checked as a named Current unit.
   template<typename NamedCurrent>
   concept IsNamedCurrentUnit = requires {
      typename NamedCurrent::NumTuple;
      typename NamedCurrent::DenTuple;
   };

   /// @brief Concept for a Current dimension.
   /// @details Checks if the provided types satisfy the Current dimension requirements.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Charge1,typename Time1>
   concept IsCurrentUnits = 
      std::is_same_v<typename Charge1::Dim, ChargeType> &&
        std::is_same_v<typename Time1::Dim, TimeType>;

   /// @brief Concept for a Current type.
   /// @details Ensures that the type meets Current type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsCurrentType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 1 &&
   IsCurrentUnits<typename std::tuple_element_t<0, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>>;

   /// @brief Retrieves the value of a Current object.
   /// @details Provides access to the underlying value represented by a Current object.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam CurrentType The type of the object being accessed.
   /// @param obj The Current object.
   /// @return The underlying value as `PrecisionType`
   template<typename Charge1,typename Time1, typename CurrentType>
   requires IsCurrentUnits<Charge1,Time1> && IsCurrentType<CurrentType>
   constexpr PrecisionType getCurrent(const CurrentType& obj)
   {
      return obj.template GetVal<std::tuple<Charge1>, std::tuple<Time1>>();
   }

   /// @brief Retrieves the value of a named Current object.
   /// @details Provides access to the value represented by a named Current object.
   /// @tparam NamedCurrent The named unit type.
   /// @tparam CurrentType The type of the object being accessed.
   /// @param obj The Current object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedCurrent, typename CurrentType>
   requires IsNamedCurrentUnit<NamedCurrent> && IsCurrentType<CurrentType>
   constexpr PrecisionType getCurrent(const CurrentType& obj)
   {
      return obj.template GetVal<typename NamedCurrent::NumTuple, typename NamedCurrent::DenTuple>();
   }

   template<typename... Ts>
   class Current;

   /// @brief Represents a Current.
   /// @details Defines operations and data storage for Current dimensions.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Charge1,typename Time1>
   requires IsCurrentUnits<Charge1,Time1>
   class Current<Charge1,Time1> : public BaseDimension<std::tuple<Charge1>, std::tuple<Time1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Charge1>, std::tuple<Time1>>;
      using Base::Base;

      /// @brief Constructs a Current object with a value.
      /// @param val The value of the Current.
      constexpr Current(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Current object from a named unit.
      /// @tparam NamedCurrent The named unit type.
      /// @param base The base unit object.
      template<typename NamedCurrent>
      requires IsNamedCurrentUnit<NamedCurrent>
      constexpr Current(const NamedCurrent& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Current.
      /// @details Prefer using the free function `getCurrent()` instead.
      /// @return The value of the Current.
      template<typename Charge1T,typename Time1T>
      requires IsCurrentUnits<Charge1T,Time1T>
      [[deprecated("Use the free function getCurrent() instead.")]]
      double GetCurrent() const
      {
         return getCurrent<Charge1T,Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of Current.
      /// @details Prefer using the free function `getCurrent()` instead.
      /// @return The value of the Current.
      template<typename NamedCurrent>
      requires IsNamedCurrentUnit<NamedCurrent>
      [[deprecated("Use the free function getCurrent() instead.")]]
      double GetCurrent() const
      {
         return getCurrent<NamedCurrent>(*this);
      }
   };

   /// @brief Represents a named Current class.
   /// @details Provides functionality for named Current units.
   /// @tparam NamedCurrent The named unit type.
   template<typename NamedCurrent>
   requires IsNamedCurrentUnit<NamedCurrent>
   class Current<NamedCurrent> : public BaseDimension<typename NamedCurrent::NumTuple, typename NamedCurrent::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedCurrent::NumTuple, typename NamedCurrent::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Current object with a value.
      /// @param val The value of the Current.
      constexpr Current(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Current object from another Current object.
      /// @tparam OtherCurrent The other Current type.
      /// @param base The base Current object.
      template<typename OtherCurrent>
      requires IsCurrentType<OtherCurrent>
      constexpr Current(const OtherCurrent& base)
         : Base(base.template GetVal<typename NamedCurrent::NumTuple, typename NamedCurrent::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Current.
      /// @details Prefer using the free function `getCurrent()` instead.
      /// @return The value of the Current.
      template<typename Charge1T,typename Time1T>
      requires IsCurrentUnits<Charge1T,Time1T>
      [[deprecated("Use the free function getCurrent() instead.")]]
      double GetCurrent() const
      {
         return getCurrent<Charge1T,Time1T>(*this);
      }

      /// @brief Deprecated function to get the value of Current.
      /// @details Prefer using the free function `getCurrent()` instead.
      /// @return The value of the Current.
      template<typename NamedCurrentUnit>
      requires IsNamedCurrentUnit<NamedCurrentUnit>
      [[deprecated("Use the free function getCurrent() instead.")]]
      double GetCurrent() const
      {
         return getCurrent<NamedCurrentUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Current.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Charge1,typename Time1>
   requires IsCurrentUnits<Charge1,Time1>
   Current(Charge1,Time1) -> Current<Charge1,Time1>;

   /// @brief Template deduction guide for Current.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename NamedCurrent>
   requires IsNamedCurrentUnit<NamedCurrent>
   Current(NamedCurrent) -> Current<NamedCurrent>;

   /// @brief Template deduction guide for Current.
   /// @tparam Charge1 Numerator Charge1 type
   /// @tparam Time1 Denominator Time1 type
   template<typename Charge1,typename Time1>
   requires IsCurrentUnits<Charge1,Time1>
   Current(BaseDimension<std::tuple<Charge1>, std::tuple<Time1>>) -> Current<Charge1,Time1>;

}

#endif // STATIC_DIMENSION_CURRENT_IMPL_H