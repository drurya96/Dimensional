#ifndef DIMENSIONAL_COMPILE_TEST_dimension_comparison
#define DIMENSIONAL_COMPILE_TEST_dimension_comparison

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct dimension_comparison_comparison_base_dimension_comparison_requires_same_dimension {
   static constexpr const char* id = "dimension_comparison:comparison:base_dimension_comparison_requires_same_dimension";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Comparison operations between base_dimensions are only permitted if their dimensional compositions are equivalent after unit conversion.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_comparison_comparison_base_dimension_comparison_applies_unit_conversion {
   static constexpr const char* id = "dimension_comparison:comparison:base_dimension_comparison_applies_unit_conversion";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "When comparing two base_dimension values, unit conversions are applied as needed to align scalar representations before comparison.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_comparison_comparison_base_dimension_comparison_returns_boolean {
   static constexpr const char* id = "dimension_comparison:comparison:base_dimension_comparison_returns_boolean";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Comparison between two compatible base_dimensions yields a dimensionless boolean result.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_dimension_comparison
