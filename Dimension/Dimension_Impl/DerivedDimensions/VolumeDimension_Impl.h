#ifndef STATIC_DIMENSION_VOLUME_IMPL_H
#define STATIC_DIMENSION_VOLUME_IMPL_H

#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Volume unit.
   /// @tparam NamedVolume The type to be checked as a named Volume unit.
   template<typename NamedVolume>
   concept IsNamedVolumeUnit = requires {
      typename NamedVolume::NumTuple;
      typename NamedVolume::DenTuple;
   };

   /// @brief Concept for a Volume dimension.
   /// @details Checks if the provided types satisfy the Volume dimension requirements.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   template<typename Length1, typename Length2, typename Length3>
   concept IsVolumeUnits = 
      std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType> &&
        std::is_same_v<typename Length3::Dim, LengthType>;

   /// @brief Concept for a Volume type.
   /// @details Ensures that the type meets Volume type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsVolumeType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 3 && std::tuple_size_v<typename T::DenTuple> == 0 &&
   IsVolumeUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>, typename std::tuple_element_t<2, typename T::NumTuple>>;

   /// @brief Retrieves the value of a Volume object.
   /// @details Provides access to the underlying value represented by a Volume object.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   /// @tparam VolumeType The type of the object being accessed.
   /// @param obj The Volume object.
   /// @return The underlying value as `PrecisionType`
   template<typename Length1, typename Length2, typename Length3, typename VolumeType>
   requires IsVolumeUnits<Length1, Length2, Length3> && IsVolumeType<VolumeType>
   constexpr PrecisionType getVolume(const VolumeType& obj)
   {
      return obj.template GetVal<std::tuple<Length1, Length2, Length3>, std::tuple<>>();
   }

   /// @brief Retrieves the value of a named Volume object.
   /// @details Provides access to the value represented by a named Volume object.
   /// @tparam NamedVolume The named unit type.
   /// @tparam VolumeType The type of the object being accessed.
   /// @param obj The Volume object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedVolume, typename VolumeType>
   requires IsNamedVolumeUnit<NamedVolume> && IsVolumeType<VolumeType>
   constexpr PrecisionType getVolume(const VolumeType& obj)
   {
      return obj.template GetVal<typename NamedVolume::NumTuple, typename NamedVolume::DenTuple>();
   }

   template<typename... Ts>
   class Volume;

   /// @brief Represents a Volume.
   /// @details Defines operations and data storage for Volume dimensions.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   template<typename Length1, typename Length2, typename Length3>
   requires IsVolumeUnits<Length1, Length2, Length3>
   class Volume<Length1, Length2, Length3> : public BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<>>
   {
   public:
      using Base = BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<>>;
      using Base::Base;

      /// @brief Constructs a Volume object with a value.
      /// @param val The value of the Volume.
      constexpr Volume(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Volume object from a named unit.
      /// @tparam NamedVolume The named unit type.
      /// @param base The base unit object.
      template<typename NamedVolume>
      requires IsNamedVolumeUnit<NamedVolume>
      constexpr Volume(const NamedVolume& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Volume.
      /// @details Prefer using the free function `getVolume()` instead.
      /// @return The value of the Volume.
      template<typename Length1T, typename Length2T, typename Length3T>
      requires IsVolumeUnits<Length1T, Length2T, Length3T>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<Length1T, Length2T, Length3T>(*this);
      }

      /// @brief Deprecated function to get the value of Volume.
      /// @details Prefer using the free function `getVolume()` instead.
      /// @return The value of the Volume.
      template<typename NamedVolume>
      requires IsNamedVolumeUnit<NamedVolume>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<NamedVolume>(*this);
      }
   };

   /// @brief Represents a named Volume class.
   /// @details Provides functionality for named Volume units.
   /// @tparam NamedVolume The named unit type.
   template<typename NamedVolume>
   requires IsNamedVolumeUnit<NamedVolume>
   class Volume<NamedVolume> : public BaseDimension<typename NamedVolume::NumTuple, typename NamedVolume::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedVolume::NumTuple, typename NamedVolume::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Volume object with a value.
      /// @param val The value of the Volume.
      constexpr Volume(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Volume object from another Volume object.
      /// @tparam OtherVolume The other Volume type.
      /// @param base The base Volume object.
      template<typename OtherVolume>
      requires IsVolumeType<OtherVolume>
      constexpr Volume(const OtherVolume& base)
         : Base(base.template GetVal<typename NamedVolume::NumTuple, typename NamedVolume::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Volume.
      /// @details Prefer using the free function `getVolume()` instead.
      /// @return The value of the Volume.
      template<typename Length1T, typename Length2T, typename Length3T>
      requires IsVolumeUnits<Length1T, Length2T, Length3T>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<Length1T, Length2T, Length3T>(*this);
      }

      /// @brief Deprecated function to get the value of Volume.
      /// @details Prefer using the free function `getVolume()` instead.
      /// @return The value of the Volume.
      template<typename NamedVolumeUnit>
      requires IsNamedVolumeUnit<NamedVolumeUnit>
      [[deprecated("Use the free function getVolume() instead.")]]
      double GetVolume() const
      {
         return getVolume<NamedVolumeUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Volume.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   template<typename Length1, typename Length2, typename Length3>
   requires IsVolumeUnits<Length1, Length2, Length3>
   Volume(Length1, Length2, Length3) -> Volume<Length1, Length2, Length3>;

   /// @brief Template deduction guide for Volume.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   template<typename NamedVolume>
   requires IsNamedVolumeUnit<NamedVolume>
   Volume(NamedVolume) -> Volume<NamedVolume>;

   /// @brief Template deduction guide for Volume.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam Length3 Numerator Length3 type
   template<typename Length1, typename Length2, typename Length3>
   requires IsVolumeUnits<Length1, Length2, Length3>
   Volume(BaseDimension<std::tuple<Length1, Length2, Length3>, std::tuple<>>) -> Volume<Length1, Length2, Length3>;

}

#endif // STATIC_DIMENSION_VOLUME_IMPL_H