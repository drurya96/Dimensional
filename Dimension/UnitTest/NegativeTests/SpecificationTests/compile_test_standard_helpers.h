#ifndef DIMENSIONAL_COMPILE_TEST_standard_helpers
#define DIMENSIONAL_COMPILE_TEST_standard_helpers

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct standard_helpers_dimension_preserving_scalar_helpers_helpers_affect_scalar_only {
   static constexpr const char* id = "standard_helpers:dimension_preserving_scalar_helpers:helpers_affect_scalar_only";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Helper functions in this category must return a value with the same dimensional structure and unit composition as the input, modifying only the scalar component.";

   template<typename = void>
      static void run() {

   }
};

struct standard_helpers_angle_limited_math_helpers_trig_functions_require_angle_input {
   static constexpr const char* id = "standard_helpers:angle_limited_math_helpers:trig_functions_require_angle_input";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Trigonometric functions may only accept inputs whose dimension corresponds to angle. Inputs with incompatible dimensions must be rejected at compile time.";

   template<typename = void>
      static void run() {

   }
};

struct standard_helpers_angle_limited_math_helpers_trig_functions_return_dimensionless {
   static constexpr const char* id = "standard_helpers:angle_limited_math_helpers:trig_functions_return_dimensionless";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Trigonometric functions must return dimensionless values regardless of the input angle unit.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_standard_helpers
