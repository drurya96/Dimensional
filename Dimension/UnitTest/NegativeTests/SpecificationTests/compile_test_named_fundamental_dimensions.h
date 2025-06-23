#ifndef DIMENSIONAL_COMPILE_TEST_named_fundamental_dimensions
#define DIMENSIONAL_COMPILE_TEST_named_fundamental_dimensions

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct named_fundamental_dimensions_fundamental_dimension_structure_fundamental_dimensions_wrap_base_dimension {
   static constexpr const char* id = "named_fundamental_dimensions:fundamental_dimension_structure:fundamental_dimensions_wrap_base_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A named fundamental dimension must wrap a base_dimension with exactly one unit_exponent whose exponent is +1.";

   template<typename = void>
      static void run() {

   }
};

struct named_fundamental_dimensions_fundamental_dimension_structure_fundamental_dimensions_must_use_fundamental_units {
   static constexpr const char* id = "named_fundamental_dimensions:fundamental_dimension_structure:fundamental_dimensions_must_use_fundamental_units";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "The unit used in a named fundamental dimension must be a fundamental unit, not a named derived unit or composite.";

   template<typename = void>
      static void run() {

   }
};

struct named_fundamental_dimensions_dimension_forward_usability_fundamental_dimensions_may_infer_from_expression {
   static constexpr const char* id = "named_fundamental_dimensions:dimension_forward_usability:fundamental_dimensions_may_infer_from_expression";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Implementations may allow expressions involving named dimensions to infer the resulting dimension type automatically, reducing the need for unit specification in user code. This feature is optional and does not affect correctness semantics.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_named_fundamental_dimensions
