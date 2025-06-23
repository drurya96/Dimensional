#ifndef DIMENSIONAL_COMPILE_TEST_point_semantics
#define DIMENSIONAL_COMPILE_TEST_point_semantics

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct point_semantics_construction_and_validity_points_must_use_reference_frames {
   static constexpr const char* id = "point_semantics:construction_and_validity:points_must_use_reference_frames";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A point may only be constructed from a valid reference_frame type and a fundamental dimension tag.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_construction_and_validity_points_only_allowed_for_fundamentals {
   static constexpr const char* id = "point_semantics:construction_and_validity:points_only_allowed_for_fundamentals";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Points may not be constructed for compound dimensions or participate in base_dimension expressions.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_construction_and_validity_point_scalar_value_may_be_negative {
   static constexpr const char* id = "point_semantics:construction_and_validity:point_scalar_value_may_be_negative";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A point may be constructed with a scalar value less than zero relative to its reference frame's anchor unit. This is structurally valid regardless of whether such values are physically meaningful.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_arithmetic_behavior_point_plus_atomic_dimension_yields_point {
   static constexpr const char* id = "point_semantics:arithmetic_behavior:point_plus_atomic_dimension_yields_point";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Adding an atomic_dimension to a point yields a point on the same reference frame as the original point.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_arithmetic_behavior_point_minus_atomic_dimension_yields_point {
   static constexpr const char* id = "point_semantics:arithmetic_behavior:point_minus_atomic_dimension_yields_point";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Subtracting an atomic_dimension from a point yields a point on the same reference frame as the original point.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_arithmetic_behavior_point_minus_point_yields_atomic_dimension {
   static constexpr const char* id = "point_semantics:arithmetic_behavior:point_minus_point_yields_atomic_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Subtracting one point from another yields an atomic_dimension representing the scalar offset between them, expressed in the reference unit of the lhs point's reference frame.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_arithmetic_behavior_point_plus_point_is_invalid {
   static constexpr const char* id = "point_semantics:arithmetic_behavior:point_plus_point_is_invalid";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Adding two points is a compile-time error, as the operation is not defined in affine space.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_arithmetic_behavior_atomic_dimension_minus_point_is_invalid {
   static constexpr const char* id = "point_semantics:arithmetic_behavior:atomic_dimension_minus_point_is_invalid";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Subtracting a point from an atomic_dimension is a compile-time error.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_casting_and_conversion_point_cast_to_other_frame_applies_offset {
   static constexpr const char* id = "point_semantics:casting_and_conversion:point_cast_to_other_frame_applies_offset";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Casting a point from one reference frame to another applies the appropriate offset and unit conversion.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_casting_and_conversion_point_used_in_expression_converts_to_unit {
   static constexpr const char* id = "point_semantics:casting_and_conversion:point_used_in_expression_converts_to_unit";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "When a point is used in an arithmetic expression that requires a base_dimension, it automatically converts to its reference unit, applying the associated reference frame offset.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_reference_frame_behavior_reference_frame_must_define_anchor_unit {
   static constexpr const char* id = "point_semantics:reference_frame_behavior:reference_frame_must_define_anchor_unit";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A reference_frame must declare a single anchor unit, representing the canonical unit of the physical dimension it anchors. All offset and unit conversions are performed relative to this anchor.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_reference_frame_behavior_point_frame_and_dimension_must_match {
   static constexpr const char* id = "point_semantics:reference_frame_behavior:point_frame_and_dimension_must_match";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A point may only be constructed if the reference_frame's physical dimension matches the fundamental dimension tag used by the point.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_reference_frame_behavior_reference_frame_offset_must_be_defined_relative_to_anchor {
   static constexpr const char* id = "point_semantics:reference_frame_behavior:reference_frame_offset_must_be_defined_relative_to_anchor";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "The offset applied when converting between two points on different reference frames is computed using the difference between their respective positions relative to the shared anchor unit.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_reference_frame_behavior_reference_frame_without_conversion_is_invalid {
   static constexpr const char* id = "point_semantics:reference_frame_behavior:reference_frame_without_conversion_is_invalid";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "A reference_frame may not use a unit unless that unit has a conversion path to its declared anchor unit. Frames that do not satisfy this constraint are ill-formed.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_comparison_point_comparison_requires_same_dimension {
   static constexpr const char* id = "point_semantics:comparison:point_comparison_requires_same_dimension";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Points may only be compared using equality or ordering operators if their associated dimension tags are identical.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_comparison_point_comparison_applies_conversion_between_frames {
   static constexpr const char* id = "point_semantics:comparison:point_comparison_applies_conversion_between_frames";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Points may be compared even if they are defined on different reference frames, as long as both reference frames are valid for the shared dimension. Appropriate conversion and offset adjustments are applied before comparison.";

   template<typename = void>
      static void run() {

   }
};

struct point_semantics_comparison_point_comparison_yields_dimensionless_result {
   static constexpr const char* id = "point_semantics:comparison:point_comparison_yields_dimensionless_result";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Comparison operations between valid point operands yield a dimensionless boolean result.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_point_semantics
