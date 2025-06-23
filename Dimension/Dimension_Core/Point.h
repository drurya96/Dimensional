
#ifndef DIMENSION_POINT_H
#define DIMENSION_POINT_H

#include <type_traits>
#include <utility>
//#include "base_dimension.h"
//#include "unit_traits.h"
#include <concepts>
#include "Conversion.h"


//#include "temperatureDimension.h" // Just for testing

namespace dimension {

   struct FundamentalUnitTag;


   template<typename T>
   struct reference_frame
   {
      using unit = T;
   };



   template<typename Target, typename Source>
   struct get_point_as_impl;

   // Entry point
   template<typename Target, typename P>
   constexpr double get_point_as(const P& obj) {
      return get_point_as_impl<Target, typename P::frame_type>::convert(obj);
   }


   template<typename Frame, typename Dim>
   class point {
   public:
      using frame_type = Frame;
      using dimension = Dim;
      //using unit = typename Frame::unit;

      constexpr explicit point(double val) : value_(val) {}

      template<typename T>
      //requires true; // Add a real constraint
      constexpr point(point<T, Dim> obj) : value_(get_point_as<Frame>(obj)) {}

      //constexpr double value() const { return value_; }

      // Convert to raw quantity in base dimension
      //constexpr operator base_dimension<unit_exponent<unit>> () const {
         // Apply affine transform from Frame
      //   return base_dimension<unit_exponent<unit>>(Frame::slope * value_ + Frame::offset);
      //}

   private:
      double value_;

      friend struct get_point_as_impl<Frame, Frame>;
   };















   // ===================== Conversions =====================


   template<typename T>
   concept HasDoubleOffset = requires {
      { T::offset } -> std::convertible_to<double>;
   };

   template<HasDoubleOffset T>
   constexpr double point_offset()
   {
      return T::offset;
   }

   template<typename T>
   requires (std::is_base_of_v<FundamentalUnitTag, T>)
   constexpr double point_offset()
   {
      return 0.0;
   }

   // Case 1: Same frame
   template<typename Frame>
   struct get_point_as_impl<Frame, Frame> {
       template<typename P>
       static constexpr double convert(const P& obj) {
           return obj.value_;
       }
   };
   
   // Case 2: Target is the unit of the source's frame
   template<typename Unit, typename Frame>
   requires std::is_same_v<Unit, typename Frame::unit>
   struct get_point_as_impl<Unit, Frame> {
       template<typename P>
       static constexpr double convert(const P& obj) {
           return get_point_as_impl<Frame, Frame>::convert(obj) + point_offset<Frame>();
       }
   };
   
   // Case 3: Target is a different unit entirely
   template<typename TargetUnit, typename SourceFrame>
   requires (
       std::is_base_of_v<FundamentalUnitTag, TargetUnit> &&
       !std::is_same_v<TargetUnit, typename SourceFrame::unit>
   )
   struct get_point_as_impl<TargetUnit, SourceFrame> {
      template<typename P>
      static constexpr double convert(const P& obj) {
         double raw = get_point_as_impl<SourceFrame, SourceFrame>::convert(obj) + point_offset<SourceFrame>();
         return DoConversion<TargetUnit,  unit_exponent<typename SourceFrame::unit>>(raw);
      }
   };
   
   // Case 4: Target is a different frame
   template<typename TargetFrame, typename SourceFrame>
   requires (
       !std::is_base_of_v<FundamentalUnitTag, TargetFrame> &&
       !std::is_base_of_v<FundamentalUnitTag, SourceFrame> &&
       !std::is_same_v<TargetFrame, SourceFrame>
   )
   struct get_point_as_impl<TargetFrame, SourceFrame> {
      template<typename P>
      static constexpr double convert(const P& obj) {
         double raw = get_point_as_impl<SourceFrame, SourceFrame>::convert(obj) + point_offset<SourceFrame>();
         double in_target_unit = DoConversion<typename TargetFrame::unit, unit_exponent<typename SourceFrame::unit>>(raw);
         return in_target_unit - point_offset<TargetFrame>();
      }
   };
   
   // Case 5: Target is a frame of this unit
   template<typename TargetFrame, typename SourceUnit>
   requires (
       std::is_base_of_v<FundamentalUnitTag, SourceUnit> &&
       std::is_same_v<SourceUnit, typename TargetFrame::unit>
   )
   struct get_point_as_impl<TargetFrame, SourceUnit> {
      template<typename P>
      static constexpr double convert(const P& obj) {
         double raw = get_point_as_impl<SourceUnit, SourceUnit>::convert(obj);
         return raw - point_offset<TargetFrame>();
      }
   };

   // Case 6: Target is a frame of a different unit
   template<typename TargetFrame, typename SourceUnit>
   requires (
       std::is_base_of_v<FundamentalUnitTag, SourceUnit> &&
       !std::is_same_v<SourceUnit, typename TargetFrame::unit>
   )
   struct get_point_as_impl<TargetFrame, SourceUnit> {
      template<typename P>
      static constexpr double convert(const P& obj) {
         double raw = get_point_as_impl<SourceUnit, SourceUnit>::convert(obj);
         double in_target_unit = DoConversion<typename TargetFrame::unit, unit_exponent<typename SourceUnit::unit>>(raw);
         return in_target_unit - point_offset<TargetFrame>();
      }
   };














   // ===================== Addition/Subtraction =====================

   // unit - point -> INVALID
   // point + point -> INVALID


   // point + unit -> point
   template<typename T, typename U, typename Dim>
   requires true // Requires a basedimension which matches lhs
   constexpr point<T, Dim> operator+(point<T, Dim> lhs, base_dimension<unit_exponent<U>> rhs)
   {
      return point<T, Dim>(get_point_as<T>(lhs) + get_dimension_as<unit_exponent<typename T::unit>>(rhs));
   }

   // unit + point -> point
   template<typename T, typename U, typename Dim>
   requires true // Requires a basedimension which matches lhs
   constexpr point<T, Dim> operator+(base_dimension<unit_exponent<U>> lhs, point<T, Dim> rhs)
   {
      return point<T, Dim>(get_point_as<T>(rhs) + get_dimension_as<unit_exponent<typename T::unit>>(lhs));
   }

   // point - unit -> point
   template<typename T, typename U, typename Dim>
   requires true // Requires a basedimension which matches lhs
   constexpr point<T, Dim> operator-(point<T, Dim> lhs, base_dimension<unit_exponent<U>> rhs)
   {
      return point<T, Dim>(get_point_as<T>(lhs) - get_dimension_as<unit_exponent<typename T::unit>>(rhs));
   }

   // point - point
   template<typename T, typename U, typename Dim>
   requires true // Requires a basedimension which matches lhs
   constexpr base_dimension<unit_exponent<typename T::unit>> operator-(point<T, Dim> lhs, point<U, Dim> rhs)
   {
      return base_dimension<unit_exponent<typename T::unit>>(get_point_as<typename T::unit>(lhs) - get_point_as<typename T::unit>(rhs));
   }














   // Need to figure out the return type, probably using auto
   // ===================== Multiplication/Division =====================
   template<typename T, is_base_dimension Rhs, typename Dim>
   requires true // Requires a basedimension which matches lhs
   constexpr auto operator*(point<T, Dim> lhs, Rhs rhs)
   {
      return base_dimension<unit_exponent<typename T::unit>>(get_point_as<typename T::unit>(lhs)) * rhs;
   }

   template<typename T, is_base_dimension Lhs, typename Dim>
   requires true // Requires a basedimension which matches lhs
   constexpr auto operator*(Lhs lhs, point<T, Dim> rhs)
   {
      return lhs * base_dimension<unit_exponent<typename T::unit>>(get_point_as<typename T::unit>(rhs));
   }

   template<typename T, is_base_dimension Rhs, typename Dim>
   requires true // Requires a basedimension which matches lhs
   constexpr auto operator/(point<T, Dim> lhs, Rhs rhs)
   {
      return base_dimension<unit_exponent<typename T::unit>>(get_point_as<typename T::unit>(lhs)) / rhs;
   }

   template<typename T, is_base_dimension Lhs, typename Dim>
   requires true // Requires a basedimension which matches lhs
   constexpr auto operator/(Lhs lhs, point<T, Dim> rhs)
   {
      return lhs / base_dimension<unit_exponent<typename T::unit>>(get_point_as<typename T::unit>(rhs));
   }




}

#endif // DIMENSION_POINT_H
