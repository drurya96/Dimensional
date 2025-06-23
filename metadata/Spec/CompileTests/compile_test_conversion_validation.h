#ifndef DIMENSIONAL_COMPILE_TEST_conversion_validation
#define DIMENSIONAL_COMPILE_TEST_conversion_validation

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct conversion_validation_conversion_legality_convertible_units_enable_dimension_casting {
   static constexpr const char* id = "conversion_validation:conversion_legality:convertible_units_enable_dimension_casting";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Two dimensioned values may be cast between each other if a valid conversion exists between their constituent unit types, even if the unit structures differ.";

   template<typename = void>
      static void run() {

   }
};

struct conversion_validation_conversion_legality_missing_conversion_disables_casting {
   static constexpr const char* id = "conversion_validation:conversion_legality:missing_conversion_disables_casting";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "If no conversion is defined between any required pair of units, casting between the dimensioned values is disallowed at compile time.";

   template<typename = void>
      static void run() {

   }
};

struct conversion_validation_conversion_legality_incompatible_physical_dimensions_disallow_conversion {
   static constexpr const char* id = "conversion_validation:conversion_legality:incompatible_physical_dimensions_disallow_conversion";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "If the aggregated physical dimensions of the source and target base_dimensions differ, casting is disallowed regardless of any defined conversions.";

   template<typename = void>
      static void run() {

   }
};

struct conversion_validation_conversion_routing_routing_via_direct_conversion {
   static constexpr const char* id = "conversion_validation:conversion_routing:routing_via_direct_conversion";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "When a conversion exists from source to target unit, it is used directly to enable conversion between dimensioned values.";

   template<typename = void>
      static void run() {

   }
};

struct conversion_validation_conversion_routing_routing_via_inverse_conversion {
   static constexpr const char* id = "conversion_validation:conversion_routing:routing_via_inverse_conversion";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "When a conversion exists from target to source unit, it is used in inverse to convert between dimensioned values.";

   template<typename = void>
      static void run() {

   }
};

struct conversion_validation_conversion_routing_routing_via_anchor_unit {
   static constexpr const char* id = "conversion_validation:conversion_routing:routing_via_anchor_unit";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "When both source and target units are convertible to a common anchor unit, that anchor is used as an intermediate to convert between dimensioned values.";

   template<typename = void>
      static void run() {

   }
};

struct conversion_validation_runtime_conversion_behavior_unit_conversion_scales_scalar {
   static constexpr const char* id = "conversion_validation:runtime_conversion_behavior:unit_conversion_scales_scalar";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "When a unit conversion is performed between dimensioned values, the scalar component is transformed by applying the associated conversion rule.";

   template<typename = void>
      static void run() {

   }
};

struct conversion_validation_runtime_conversion_behavior_conversion_yields_deterministic_scalar {
   static constexpr const char* id = "conversion_validation:runtime_conversion_behavior:conversion_yields_deterministic_scalar";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "The scalar value resulting from a unit conversion must be uniquely determined by the definition of the conversion, regardless of whether the operation occurs at compile time or runtime.";

   template<typename = void>
      static void run() {

   }
};

struct conversion_validation_runtime_conversion_behavior_conversion_may_occur_at_runtime {
   static constexpr const char* id = "conversion_validation:runtime_conversion_behavior:conversion_may_occur_at_runtime";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Conversions are permitted to occur at runtime if the host language or environment does not support compile-time evaluation. The semantics must remain consistent regardless of when the conversion is executed.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_conversion_validation
