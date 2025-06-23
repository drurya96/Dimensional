#ifndef DIMENSIONAL_COMPILE_TEST_unit_definition_and_constraints
#define DIMENSIONAL_COMPILE_TEST_unit_definition_and_constraints

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct unit_definition_and_constraints_unit_structure_unit_must_have_conversion_to_anchor {
   static constexpr const char* id = "unit_definition_and_constraints:unit_structure:unit_must_have_conversion_to_anchor";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "A unit is only valid for use in a base_dimension if it has a defined conversion path to the anchor unit of its physical dimension.";

   template<typename = void>
      static void run() {

   }
};

struct unit_definition_and_constraints_unit_structure_unit_must_tag_single_physical_dimension {
   static constexpr const char* id = "unit_definition_and_constraints:unit_structure:unit_must_tag_single_physical_dimension";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Each unit must be associated with exactly one physical dimension. Units spanning multiple dimensions are invalid.";

   template<typename = void>
      static void run() {

   }
};

struct unit_definition_and_constraints_unit_structure_unit_type_must_be_globally_unique {
   static constexpr const char* id = "unit_definition_and_constraints:unit_structure:unit_type_must_be_globally_unique";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Each unit type must be uniquely identifiable across the system. Duplicate unit type definitions are disallowed.";

   template<typename = void>
      static void run() {

   }
};

struct unit_definition_and_constraints_conversion_constraints_unit_conversions_must_be_linear_and_zero_aligned {
   static constexpr const char* id = "unit_definition_and_constraints:conversion_constraints:unit_conversions_must_be_linear_and_zero_aligned";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Conversions between units must be strictly linear functions of the form y = k·x, where the origin maps to the origin. Offset conversions are not permitted for unit types.";

   template<typename = void>
      static void run() {

   }
};

struct unit_definition_and_constraints_conversion_constraints_conversions_across_dimensions_disallowed {
   static constexpr const char* id = "unit_definition_and_constraints:conversion_constraints:conversions_across_dimensions_disallowed";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Conversions may only be defined between unit types that belong to the same physical dimension. Cross-dimensional conversions are invalid.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_unit_definition_and_constraints
