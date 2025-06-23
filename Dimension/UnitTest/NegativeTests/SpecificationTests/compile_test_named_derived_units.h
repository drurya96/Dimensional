#ifndef DIMENSIONAL_COMPILE_TEST_named_derived_units
#define DIMENSIONAL_COMPILE_TEST_named_derived_units

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct named_derived_units_composition_and_equivalence_derived_unit_composition_must_be_explicit {
   static constexpr const char* id = "named_derived_units:composition_and_equivalence:derived_unit_composition_must_be_explicit";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A named derived unit must explicitly specify a composition consisting of one or more fundamental unit types, each paired with a rational exponent.";

   template<typename = void>
      static void run() {

   }
};

struct named_derived_units_composition_and_equivalence_derived_unit_composition_must_use_valid_units {
   static constexpr const char* id = "named_derived_units:composition_and_equivalence:derived_unit_composition_must_use_valid_units";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Each unit type used in the composition of a named derived unit must be a valid unit defined in the unit registry.";

   template<typename = void>
      static void run() {

   }
};

struct named_derived_units_composition_and_equivalence_derived_unit_exponents_must_be_normalized {
   static constexpr const char* id = "named_derived_units:composition_and_equivalence:derived_unit_exponents_must_be_normalized";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "The unit exponents within a named derived unit must be reduced to their canonical rational representation and must not include duplicate unit types.";

   template<typename = void>
      static void run() {

   }
};

struct named_derived_units_integration_with_dimension_system_derived_units_construct_base_dimension {
   static constexpr const char* id = "named_derived_units:integration_with_dimension_system:derived_units_construct_base_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A named derived unit may be used in the construction of a base_dimension, and its internal composition is treated as contributing to the dimensional structure.";

   template<typename = void>
      static void run() {

   }
};

struct named_derived_units_integration_with_dimension_system_derived_units_preserve_arithmetic_behavior {
   static constexpr const char* id = "named_derived_units:integration_with_dimension_system:derived_units_preserve_arithmetic_behavior";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "The use of a named derived unit in dimensional composition does not change the arithmetic semantics of the resulting base_dimension.";

   template<typename = void>
      static void run() {

   }
};

struct named_derived_units_integration_with_dimension_system_derived_units_conversion_requires_component_paths {
   static constexpr const char* id = "named_derived_units:integration_with_dimension_system:derived_units_conversion_requires_component_paths";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A conversion between two named derived units is valid only if a conversion path exists between each unit in their compositions.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_named_derived_units
