#ifndef STATIC_DIMENSION_AREA_IMPL_H
#define STATIC_DIMENSION_AREA_IMPL_H

#include "../../LengthDimension.h"

namespace Dimension
{
   /// @brief Concept for a named Area unit.
   /// @tparam NamedArea The type to be checked as a named Area unit.
   template<typename NamedArea>
   concept IsNamedAreaUnit = requires {
      typename NamedArea::NumTuple;
      typename NamedArea::DenTuple;
   };

   /// @brief Concept for a Area dimension.
   /// @details Checks if the provided types satisfy the Area dimension requirements.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename Length1, typename Length2>
   concept IsAreaUnits = 
      std::is_same_v<typename Length1::Dim, LengthType> &&
        std::is_same_v<typename Length2::Dim, LengthType>;

   /// @brief Concept for a Area type.
   /// @details Ensures that the type meets Area type requirements, based on numerator and denominator types.
   /// @tparam T The type to validate.
   template<typename T>
   concept IsAreaType = requires {
      typename T::NumTuple;
      typename T::DenTuple;
   } && std::tuple_size_v<typename T::NumTuple> == 2 && std::tuple_size_v<typename T::DenTuple> == 0 &&
   IsAreaUnits<typename std::tuple_element_t<0, typename T::NumTuple>, typename std::tuple_element_t<1, typename T::NumTuple>>;

   /// @brief Retrieves the value of a Area object.
   /// @details Provides access to the underlying value represented by a Area object.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   /// @tparam AreaType The type of the object being accessed.
   /// @param obj The Area object.
   /// @return The underlying value as `PrecisionType`
   template<typename Length1, typename Length2, typename AreaType>
   requires IsAreaUnits<Length1, Length2> && IsAreaType<AreaType>
   constexpr PrecisionType getArea(const AreaType& obj)
   {
      return obj.template GetVal<std::tuple<Length1, Length2>, std::tuple<>>();
   }

   /// @brief Retrieves the value of a named Area object.
   /// @details Provides access to the value represented by a named Area object.
   /// @tparam NamedArea The named unit type.
   /// @tparam AreaType The type of the object being accessed.
   /// @param obj The Area object.
   /// @return The underlying value as `PrecisionType`.
   template<typename NamedArea, typename AreaType>
   requires IsNamedAreaUnit<NamedArea> && IsAreaType<AreaType>
   constexpr PrecisionType getArea(const AreaType& obj)
   {
      return obj.template GetVal<typename NamedArea::NumTuple, typename NamedArea::DenTuple>();
   }

   template<typename... Ts>
   class Area;

   /// @brief Represents a Area.
   /// @details Defines operations and data storage for Area dimensions.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename Length1, typename Length2>
   requires IsAreaUnits<Length1, Length2>
   class Area<Length1, Length2> : public BaseDimension<std::tuple<Length1, Length2>, std::tuple<>>
   {
   public:
      using Base = BaseDimension<std::tuple<Length1, Length2>, std::tuple<>>;
      using Base::Base;

      /// @brief Constructs a Area object with a value.
      /// @param val The value of the Area.
      constexpr Area(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Area object from a named unit.
      /// @tparam NamedArea The named unit type.
      /// @param base The base unit object.
      template<typename NamedArea>
      requires IsNamedAreaUnit<NamedArea>
      constexpr Area(const NamedArea& base) : Base(base) {}

      /// @brief Deprecated function to get the value of Area.
      /// @details Prefer using the free function `getArea()` instead.
      /// @return The value of the Area.
      template<typename Length1T, typename Length2T>
      requires IsAreaUnits<Length1T, Length2T>
      [[deprecated("Use the free function getArea() instead.")]]
      double GetArea() const
      {
         return getArea<Length1T, Length2T>(*this);
      }

      /// @brief Deprecated function to get the value of Area.
      /// @details Prefer using the free function `getArea()` instead.
      /// @return The value of the Area.
      template<typename NamedArea>
      requires IsNamedAreaUnit<NamedArea>
      [[deprecated("Use the free function getArea() instead.")]]
      double GetArea() const
      {
         return getArea<NamedArea>(*this);
      }
   };

   /// @brief Represents a named Area class.
   /// @details Provides functionality for named Area units.
   /// @tparam NamedArea The named unit type.
   template<typename NamedArea>
   requires IsNamedAreaUnit<NamedArea>
   class Area<NamedArea> : public BaseDimension<typename NamedArea::NumTuple, typename NamedArea::DenTuple>
   {
   public:
      using Base = BaseDimension<typename NamedArea::NumTuple, typename NamedArea::DenTuple>;
      using Base::Base;

      /// @brief Constructs a Area object with a value.
      /// @param val The value of the Area.
      constexpr Area(PrecisionType val) : Base(val) {}

      /// @brief Constructs a Area object from another Area object.
      /// @tparam OtherArea The other Area type.
      /// @param base The base Area object.
      template<typename OtherArea>
      requires IsAreaType<OtherArea>
      constexpr Area(const OtherArea& base)
         : Base(base.template GetVal<typename NamedArea::NumTuple, typename NamedArea::DenTuple>()) {}

      /// @brief Deprecated function to get the value of Area.
      /// @details Prefer using the free function `getArea()` instead.
      /// @return The value of the Area.
      template<typename Length1T, typename Length2T>
      requires IsAreaUnits<Length1T, Length2T>
      [[deprecated("Use the free function getArea() instead.")]]
      double GetArea() const
      {
         return getArea<Length1T, Length2T>(*this);
      }

      /// @brief Deprecated function to get the value of Area.
      /// @details Prefer using the free function `getArea()` instead.
      /// @return The value of the Area.
      template<typename NamedAreaUnit>
      requires IsNamedAreaUnit<NamedAreaUnit>
      [[deprecated("Use the free function getArea() instead.")]]
      double GetArea() const
      {
         return getArea<NamedAreaUnit>(*this);
      }         
   };

   /// @brief Template deduction guide for Area.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename Length1, typename Length2>
   requires IsAreaUnits<Length1, Length2>
   Area(Length1, Length2) -> Area<Length1, Length2>;

   /// @brief Template deduction guide for Area.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename NamedArea>
   requires IsNamedAreaUnit<NamedArea>
   Area(NamedArea) -> Area<NamedArea>;

   /// @brief Template deduction guide for Area.
   /// @tparam Length1 Numerator Length1 type
   /// @tparam Length2 Numerator Length2 type
   template<typename Length1, typename Length2>
   requires IsAreaUnits<Length1, Length2>
   Area(BaseDimension<std::tuple<Length1, Length2>, std::tuple<>>) -> Area<Length1, Length2>;

}

#endif // STATIC_DIMENSION_AREA_IMPL_H