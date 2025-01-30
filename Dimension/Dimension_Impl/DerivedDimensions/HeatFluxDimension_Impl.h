#ifndef STATIC_DIMENSION_HEATFLUX_IMPL_H
#define STATIC_DIMENSION_HEATFLUX_IMPL_H

#include "../../MassDimension.h"
#include "../../TimeDimension.h"

namespace Dimension
{
   /// @brief Concept for a named HeatFlux unit.
   /// @tparam NamedHeatFlux The type to be checked as a named HeatFlux unit.
   template<typename NamedHeatFlux>
   concept IsNamedHeatFluxUnit = requires {
      typename NamedHeatFlux::NumTuple;
      typename NamedHeatFlux::DenTuple;
   };

   /// @brief Concept for a HeatFlux dimension.
   /// @details Checks if the provided types satisfy the HeatFlux dimension requirements.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename Mass1,typename Time1, typename Time2, typename Time3>
   concept IsHeatFluxUnits = 
      std::is_same_v<typename Mass1::Dim, MassType> &&
        std::is_same_v<typename Time1::Dim, TimeType> &&
        std::is_same_v<typename Time2::Dim, TimeType> &&
        std::is_same_v<typename Time3::Dim, TimeType>;

   /// @brief Concept for a HeatFlux type.
   /// @details Ensures that the type meets HeatFlux type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsHeatFluxType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 1 && std::tuple_size_v<typename T::DenTuple> == 3 &&
   IsHeatFluxUnits<typename std::tuple_element_t<0, typename T::NumTuple>,typename std::tuple_element_t<0, typename T::DenTuple>, typename std::tuple_element_t<1, typename T::DenTuple>, typename std::tuple_element_t<2, typename T::DenTuple>>;

   /// @brief Retrieves the value of a HeatFlux object.
   /// @details Provides access to the underlying value represented by a HeatFlux object.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   /// @tparam HeatFluxType The type of the object being accessed.
   /// @param obj The HeatFlux object.
   /// @return The underlying value as `PrecisionType`
   template<typename Mass1,typename Time1, typename Time2, typename Time3, typename HeatFluxType>
   requires IsHeatFluxUnits<Mass1,Time1, Time2, Time3> && IsHeatFluxType<HeatFluxType>
   constexpr PrecisionType getHeatFlux(const HeatFluxType& obj)
   {
      return obj.template GetVal<std::tuple<Mass1>, std::tuple<Time1, Time2, Time3>>();
   }

   /// @brief Retrieves the value of a named HeatFlux object.
   /// @details Provides access to the value represented by a named HeatFlux object.
   /// @tparam NamedHeatFlux The named unit type.
   /// @tparam HeatFluxType The type of the object being accessed.
   /// @param obj The HeatFlux object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedHeatFlux, typename HeatFluxType>
   requires IsNamedHeatFluxUnit<NamedHeatFlux> && IsHeatFluxType<HeatFluxType>
   constexpr PrecisionType getHeatFlux(const HeatFluxType& obj)
   {
      return obj.template GetVal<typename NamedHeatFlux::NumTuple, typename NamedHeatFlux::DenTuple>();
   }

   template<typename... Ts>
   class HeatFlux;

   /// @brief Represents a default HeatFlux.
   /// @details This HeatFlux is templated on the primary units of the relevant dimensions.
   ///   While this is a specific type, its intended use is to treat an object or parameter as an abstract
   ///   "HeatFlux" type, without regard for the underlying units.
   template<>
   class HeatFlux<> : public BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTime>>
   {
   public:
      using Base = BaseDimension<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTime>>;
      using Base::Base;

      /// @brief Constructs a HeatFlux object with a value.
      /// @param val The value of the HeatFlux.
      explicit constexpr HeatFlux(PrecisionType val) : Base(val) {}

      /// @brief Constructs a HeatFlux object from another HeatFlux object.
      /// @tparam OtherHeatFlux The other HeatFlux type.
      /// @param base The base HeatFlux object.
      template<typename OtherHeatFlux>
      requires IsHeatFluxType<OtherHeatFlux>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr HeatFlux(const OtherHeatFlux& base)
         : Base(base.template GetVal<std::tuple<PrimaryMass>, std::tuple<PrimaryTime, PrimaryTime, PrimaryTime>>()) {}
   };

   /// @brief Represents a HeatFlux.
   /// @details Defines operations and data storage for HeatFlux dimensions.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename Mass1,typename Time1, typename Time2, typename Time3>
   requires IsHeatFluxUnits<Mass1,Time1, Time2, Time3>
   class HeatFlux<Mass1,Time1, Time2, Time3> : public BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Time2, Time3>>
   {
   public:
      using Base = BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Time2, Time3>>;
      using Base::Base;

      /// @brief Constructs a HeatFlux object with a value.
      /// @param val The value of the HeatFlux.
      explicit constexpr HeatFlux(PrecisionType val) : Base(val) {}

      /// @brief Constructs a HeatFlux object from a named unit.
      /// @tparam NamedHeatFlux The named unit type.
      /// @param base The base unit object.
      template<typename NamedHeatFlux>
      requires IsNamedHeatFluxUnit<NamedHeatFlux>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr HeatFlux(const NamedHeatFlux& base) : Base(base) {}

      /// @brief Deprecated function to get the value of HeatFlux.
      /// @details Prefer using the free function `getHeatFlux()` instead.
      /// @return The value of the HeatFlux.
      template<typename Mass1T,typename Time1T, typename Time2T, typename Time3T>
      requires IsHeatFluxUnits<Mass1T,Time1T, Time2T, Time3T>
      [[deprecated("Use the free function getHeatFlux() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetHeatFlux() const
      {
         return getHeatFlux<Mass1T,Time1T, Time2T, Time3T>(*this);
      }

      /// @brief Deprecated function to get the value of HeatFlux.
      /// @details Prefer using the free function `getHeatFlux()` instead.
      /// @return The value of the HeatFlux.
      template<typename NamedHeatFlux>
      requires IsNamedHeatFluxUnit<NamedHeatFlux>
      [[deprecated("Use the free function getHeatFlux() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetHeatFlux() const
      {
         return getHeatFlux<NamedHeatFlux>(*this);
      }
   };

   /// @brief Represents a named HeatFlux class.
   /// @details Provides functionality for named HeatFlux units.
   /// @tparam NamedHeatFlux The named unit type.
   template<typename NamedHeatFlux>
   requires IsNamedHeatFluxUnit<NamedHeatFlux>
   class HeatFlux<NamedHeatFlux> : public BaseDimension<typename NamedHeatFlux::NumTuple, typename NamedHeatFlux::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedHeatFlux::NumTuple, typename NamedHeatFlux::DenTuple>;
      using Base::Base;

      /// @brief Constructs a HeatFlux object with a value.
      /// @param val The value of the HeatFlux.
      explicit constexpr HeatFlux(PrecisionType val) : Base(val) {}

      /// @brief Constructs a HeatFlux object from another HeatFlux object.
      /// @tparam OtherHeatFlux The other HeatFlux type.
      /// @param base The base HeatFlux object.
      template<typename OtherHeatFlux>
      requires IsHeatFluxType<OtherHeatFlux>
      // Implicit conversion between dimensions of the same unit is core to Dimensional
      // cppcheck-suppress noExplicitConstructor
      constexpr HeatFlux(const OtherHeatFlux& base)
         : Base(base.template GetVal<typename NamedHeatFlux::NumTuple, typename NamedHeatFlux::DenTuple>()) {}

      /// @brief Deprecated function to get the value of HeatFlux.
      /// @details Prefer using the free function `getHeatFlux()` instead.
      /// @return The value of the HeatFlux.
      template<typename Mass1T,typename Time1T, typename Time2T, typename Time3T>
      requires IsHeatFluxUnits<Mass1T,Time1T, Time2T, Time3T>
      [[deprecated("Use the free function getHeatFlux() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetHeatFlux() const
      {
         return getHeatFlux<Mass1T,Time1T, Time2T, Time3T>(*this);
      }

      /// @brief Deprecated function to get the value of HeatFlux.
      /// @details Prefer using the free function `getHeatFlux()` instead.
      /// @return The value of the HeatFlux.
      template<typename NamedHeatFluxUnit>
      requires IsNamedHeatFluxUnit<NamedHeatFluxUnit>
      [[deprecated("Use the free function getHeatFlux() instead.")]]
      // cppcheck-suppress unusedFunction
      double GetHeatFlux() const
      {
         return getHeatFlux<NamedHeatFluxUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for HeatFlux.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename Mass1,typename Time1, typename Time2, typename Time3>
   requires IsHeatFluxUnits<Mass1,Time1, Time2, Time3>
   HeatFlux(Mass1,Time1, Time2, Time3) -> HeatFlux<Mass1,Time1, Time2, Time3>;

   /// @brief Template deduction guide for HeatFlux.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename NamedHeatFlux>
   requires IsNamedHeatFluxUnit<NamedHeatFlux>
   HeatFlux(NamedHeatFlux) -> HeatFlux<NamedHeatFlux>;

   /// @brief Template deduction guide for HeatFlux.
   /// @tparam Mass1 Numerator Mass1 type
   /// @tparam Time1 Denominator Time1 type
   /// @tparam Time2 Denominator Time2 type
   /// @tparam Time3 Denominator Time3 type
   template<typename Mass1,typename Time1, typename Time2, typename Time3>
   requires IsHeatFluxUnits<Mass1,Time1, Time2, Time3>
   HeatFlux(BaseDimension<std::tuple<Mass1>, std::tuple<Time1, Time2, Time3>>) -> HeatFlux<Mass1,Time1, Time2, Time3>;

}

#endif // STATIC_DIMENSION_HEATFLUX_IMPL_H