#ifndef DIMENSIONAL_COMPILE_TEST_named_derived_dimensions
#define DIMENSIONAL_COMPILE_TEST_named_derived_dimensions

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct named_derived_dimensions_derived_dimension_structure_named_derived_dimensions_wrap_base_dimension {
   static constexpr const char* id = "named_derived_dimensions:derived_dimension_structure:named_derived_dimensions_wrap_base_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Each named derived dimension must wrap a base_dimension with a fixed unit exponent composition representing its associated physical property.";

   template<typename = void>
      static void run() {

   }
};

struct named_derived_dimensions_derived_dimension_structure_derived_dimensions_accept_fundamental_units_as_parameters {
   static constexpr const char* id = "named_derived_dimensions:derived_dimension_structure:derived_dimensions_accept_fundamental_units_as_parameters";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A named derived dimension may be parameterized with one or more fundamental units, where each position corresponds to a predefined exponent within the internal composition.";

   template<typename = void>
      static void run() {

   }
};

struct named_derived_dimensions_derived_dimension_structure_derived_dimensions_accept_named_derived_unit {
   static constexpr const char* id = "named_derived_dimensions:derived_dimension_structure:derived_dimensions_accept_named_derived_unit";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A named derived dimension may be instantiated using a single named derived unit whose internal composition matches the dimensional structure of the derived dimension.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_named_derived_dimensions
