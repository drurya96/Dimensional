#ifndef STATIC_DIMENSION_MAGNETICFLUX_IMPL_H
#define STATIC_DIMENSION_MAGNETICFLUX_IMPL_H

#include "../../MassDimension.h"
#include "../../TimeDimension.h"
#include "../../ChargeDimension.h"
#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept for a named MagneticFlux unit.
   /// @tparam NamedMagneticFlux The type to be checked as a named MagneticFlux unit.
   template<typename NamedMagneticFlux>
   concept IsNamedMagneticFluxUnit = requires {
      typename NamedMagneticFlux::NumTuple;
      typename NamedMagneticFlux::DenTuple;
   };

   /// @brief Concept for a MagneticFlux dimension.
   /// @details Checks if the provided types satisfy the MagneticFlux dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1>
   concept IsMagneticFluxUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Charge1::Dim, ChargeType>;

   /// @brief Concept for a MagneticFlux type.
   /// @details Ensures that the type meets MagneticFlux type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsMagneticFluxType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 2 &&
   IsMagneticFluxUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>>;

   /// @brief Retrieves the value of a MagneticFlux object.
   /// @details Provides access to the underlying value represented by a MagneticFlux object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   /// @tparam MagneticFluxType The type of the object being accessed.
   /// @param obj The MagneticFlux object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1, typename MagneticFluxType>
   requires IsMagneticFluxUnits<Mass1, Length1, Length2,Time1, Charge1> && IsMagneticFluxType<MagneticFluxType>
   constexpr PrecisionType getMagneticFlux(const MagneticFluxType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Charge1>>();
   }

   /// @brief Retrieves the value of a named MagneticFlux object.
   /// @details Provides access to the value represented by a named MagneticFlux object.
   /// @tparam NamedMagneticFlux The named unit type.
   /// @tparam MagneticFluxType The type of the object being accessed.
   /// @param obj The MagneticFlux object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedMagneticFlux, typename MagneticFluxType>
   requires IsNamedMagneticFluxUnit<NamedMagneticFlux> && IsMagneticFluxType<MagneticFluxType>
   constexpr PrecisionType getMagneticFlux(const MagneticFluxType& obj)
   {
      return obj.template GetVal<typename NamedMagneticFlux::NumTuple, typename NamedMagneticFlux::DenTuple>();
   }

   template<typename... Ts>
   class MagneticFlux;

   /// @brief Represents a MagneticFlux.
   /// @details Defines operations and data storage for MagneticFlux dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1>
   requires IsMagneticFluxUnits<Mass1, Length1, Length2,Time1, Charge1>
   class MagneticFlux<Mass1, Length1, Length2,Time1, Charge1> : public BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Charge1>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Charge1>>;
      using Base::Base;

      /// @brief Constructs a MagneticFlux object with a value.
      /// @param val The value of the MagneticFlux.
      constexpr MagneticFlux(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MagneticFlux object from a named unit.
      /// @tparam NamedMagneticFlux The named unit type.
      /// @param base The base unit object.
      template<typename NamedMagneticFlux>
      requires IsNamedMagneticFluxUnit<NamedMagneticFlux>
      constexpr MagneticFlux(const NamedMagneticFlux& base) : Base(base) {}

      /// @brief Deprecated function to get the value of MagneticFlux.
      /// @details Prefer using the free function `getMagneticFlux()` instead.
      /// @return The value of the MagneticFlux.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Charge1T>
      requires IsMagneticFluxUnits<Mass1T, Length1T, Length2T,Time1T, Charge1T>
      [[deprecated("Use the free function getMagneticFlux() instead.")]]
      double GetMagneticFlux() const
      {
         return getMagneticFlux<Mass1T, Length1T, Length2T,Time1T, Charge1T>(*this);
      }

      /// @brief Deprecated function to get the value of MagneticFlux.
      /// @details Prefer using the free function `getMagneticFlux()` instead.
      /// @return The value of the MagneticFlux.
      template<typename NamedMagneticFlux>
      requires IsNamedMagneticFluxUnit<NamedMagneticFlux>
      [[deprecated("Use the free function getMagneticFlux() instead.")]]
      double GetMagneticFlux() const
      {
         return getMagneticFlux<NamedMagneticFlux>(*this);
      }
   };

   /// @brief Represents a named MagneticFlux class.
   /// @details Provides functionality for named MagneticFlux units.
   /// @tparam NamedMagneticFlux The named unit type.
   template<typename NamedMagneticFlux>
   requires IsNamedMagneticFluxUnit<NamedMagneticFlux>
   class MagneticFlux<NamedMagneticFlux> : public BaseDimension<typename NamedMagneticFlux::NumTuple, typename NamedMagneticFlux::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedMagneticFlux::NumTuple, typename NamedMagneticFlux::DenTuple>;
      using Base::Base;

      /// @brief Constructs a MagneticFlux object with a value.
      /// @param val The value of the MagneticFlux.
      constexpr MagneticFlux(PrecisionType val) : Base(val) {}

      /// @brief Constructs a MagneticFlux object from another MagneticFlux object.
      /// @tparam OtherMagneticFlux The other MagneticFlux type.
      /// @param base The base MagneticFlux object.
      template<typename OtherMagneticFlux>
      requires IsMagneticFluxType<OtherMagneticFlux>
      constexpr MagneticFlux(const OtherMagneticFlux& base)
         : Base(base.template GetVal<typename NamedMagneticFlux::NumTuple, typename NamedMagneticFlux::DenTuple>()) {}

      /// @brief Deprecated function to get the value of MagneticFlux.
      /// @details Prefer using the free function `getMagneticFlux()` instead.
      /// @return The value of the MagneticFlux.
      template<typename Mass1T, typename Length1T, typename Length2T,typename Time1T, typename Charge1T>
      requires IsMagneticFluxUnits<Mass1T, Length1T, Length2T,Time1T, Charge1T>
      [[deprecated("Use the free function getMagneticFlux() instead.")]]
      double GetMagneticFlux() const
      {
         return getMagneticFlux<Mass1T, Length1T, Length2T,Time1T, Charge1T>(*this);
      }

      /// @brief Deprecated function to get the value of MagneticFlux.
      /// @details Prefer using the free function `getMagneticFlux()` instead.
      /// @return The value of the MagneticFlux.
      template<typename NamedMagneticFluxUnit>
      requires IsNamedMagneticFluxUnit<NamedMagneticFluxUnit>
      [[deprecated("Use the free function getMagneticFlux() instead.")]]
      double GetMagneticFlux() const
      {
         return getMagneticFlux<NamedMagneticFluxUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for MagneticFlux.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1>
   requires IsMagneticFluxUnits<Mass1, Length1, Length2,Time1, Charge1>
   MagneticFlux(Mass1, Length1, Length2,Time1, Charge1) -> MagneticFlux<Mass1, Length1, Length2,Time1, Charge1>;

   /// @brief Template deduction guide for MagneticFlux.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename NamedMagneticFlux>
   requires IsNamedMagneticFluxUnit<NamedMagneticFlux>
   MagneticFlux(NamedMagneticFlux) -> MagneticFlux<NamedMagneticFlux>;

   /// @brief Template deduction guide for MagneticFlux.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Charge1 Denominator Charge1 type
   template<typename Mass1, typename Length1, typename Length2,typename Time1, typename Charge1>
   requires IsMagneticFluxUnits<Mass1, Length1, Length2,Time1, Charge1>
   MagneticFlux(BaseDimension<std::tuple<Mass1, Length1, Length2>, std::tuple<Time1, Charge1>>) -> MagneticFlux<Mass1, Length1, Length2,Time1, Charge1>;

}

#endif // STATIC_DIMENSION_MAGNETICFLUX_IMPL_H