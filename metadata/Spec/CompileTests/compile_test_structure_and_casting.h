#ifndef DIMENSIONAL_COMPILE_TEST_structure_and_casting
#define DIMENSIONAL_COMPILE_TEST_structure_and_casting

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct structure_and_casting_structural_validation_no_duplicate_unit_types {
   static constexpr const char* id = "structure_and_casting:structural_validation:no_duplicate_unit_types";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "A base_dimension may not contain multiple unit_exponents with the same unit type, regardless of exponent sign.";

   template<typename = void>
      static void run() {

   }
};

struct structure_and_casting_structural_compatibility_structurally_compatible_base_dimensions_are_assignable {
   static constexpr const char* id = "structure_and_casting:structural_compatibility:structurally_compatible_base_dimensions_are_assignable";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Two base_dimensions are compatible for assignment when, after aggregating exponents by physical dimension, the resulting dimensional compositions are equal.";

   template<typename = void>
      static void run() {

   }
};

struct structure_and_casting_structural_compatibility_incompatible_base_dimensions_are_not_assignable {
   static constexpr const char* id = "structure_and_casting:structural_compatibility:incompatible_base_dimensions_are_not_assignable";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "If no conversion exists between units used to represent the same physical dimension, the base_dimensions are not assignable.";

   template<typename = void>
      static void run() {

   }
};

struct structure_and_casting_casting_and_simplification_cast_enables_unit_cancellation {
   static constexpr const char* id = "structure_and_casting:casting_and_simplification:cast_enables_unit_cancellation";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Explicit casting allows a base_dimension to simplify and cancel opposing units to produce a new, structurally valid type.";

   template<typename = void>
      static void run() {

   }
};

struct structure_and_casting_casting_and_simplification_cast_enables_unit_conversion {
   static constexpr const char* id = "structure_and_casting:casting_and_simplification:cast_enables_unit_conversion";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Implicitly casting a base_dimension to a different but dimensionally equivalent structure performs necessary unit conversions to match the target structure.";

   template<typename = void>
      static void run() {

   }
};

struct structure_and_casting_casting_and_simplification_casting_to_identical_structure_is_noop {
   static constexpr const char* id = "structure_and_casting:casting_and_simplification:casting_to_identical_structure_is_noop";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Casting a base_dimension to the same structure is a valid operation that produces a base_dimension of the same type with unchanged scalar value.";

   template<typename = void>
      static void run() {

   }
};

struct structure_and_casting_casting_and_simplification_base_dimension_default_constructs_to_zero {
   static constexpr const char* id = "structure_and_casting:casting_and_simplification:base_dimension_default_constructs_to_zero";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Default construction of a base_dimension produces a valid object with scalar value zero and a well-formed dimensional structure.";

   template<typename = void>
      static void run() {

   }
};

struct structure_and_casting_casting_and_simplification_empty_base_dimension_is_scalar {
   static constexpr const char* id = "structure_and_casting:casting_and_simplification:empty_base_dimension_is_scalar";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A base_dimension containing no unit_exponents is valid and represents a dimensionless scalar quantity.";

   template<typename = void>
      static void run() {

   }
};

struct structure_and_casting_casting_and_simplification_scalar_dimension_casts_to_raw {
   static constexpr const char* id = "structure_and_casting:casting_and_simplification:scalar_dimension_casts_to_raw";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A base_dimension with no units may be implicitly cast to a raw scalar value for use in non-dimensional expressions.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_structure_and_casting
