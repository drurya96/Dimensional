#ifndef DIMENSIONAL_COMPILE_TEST_dimension_arithmetic
#define DIMENSIONAL_COMPILE_TEST_dimension_arithmetic

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct dimension_arithmetic_identity_behavior_raise_base_dimension_to_zero {
   static constexpr const char* id = "dimension_arithmetic:identity_behavior:raise_base_dimension_to_zero";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Raising a base_dimension to the 0th power yields a dimensionless result.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_identity_behavior_raise_base_dimension_to_one {
   static constexpr const char* id = "dimension_arithmetic:identity_behavior:raise_base_dimension_to_one";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Raising a base_dimension to the 1st power yields the original base_dimension.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_identity_behavior_multiply_by_raw_one {
   static constexpr const char* id = "dimension_arithmetic:identity_behavior:multiply_by_raw_one";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Multiplying a base_dimension by the raw value 1 results in the original base_dimension.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_identity_behavior_divide_by_raw_one {
   static constexpr const char* id = "dimension_arithmetic:identity_behavior:divide_by_raw_one";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Dividing a base_dimension by the raw value 1 results in the original base_dimension.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_multiplication_multiply_base_dimension_by_base_dimension {
   static constexpr const char* id = "dimension_arithmetic:multiplication:multiply_base_dimension_by_base_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "When both operands contain the same unit, the resulting base_dimension includes that unit with an exponent equal to the sum of the input exponents.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_multiplication_scalar_times_base_dimension {
   static constexpr const char* id = "dimension_arithmetic:multiplication:scalar_times_base_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Multiplying a raw value by a base_dimension yields the original base_dimension with scalar multiplied by the raw value.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_multiplication_base_dimension_times_scalar {
   static constexpr const char* id = "dimension_arithmetic:multiplication:base_dimension_times_scalar";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Multiplying a base_dimension by a raw value yields the original base_dimension with scalar multiplied by the raw value.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_division_divide_base_dimension_by_base_dimension {
   static constexpr const char* id = "dimension_arithmetic:division:divide_base_dimension_by_base_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "When both operands include the same unit, the resulting base_dimension includes that unit with an exponent equal to the difference between the exponents of the numerator and denominator.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_division_base_dimension_divided_by_raw {
   static constexpr const char* id = "dimension_arithmetic:division:base_dimension_divided_by_raw";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Dividing a base_dimension by a raw value yields the original base_dimension with scalar divided by the raw value.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_division_raw_divided_by_base_dimension {
   static constexpr const char* id = "dimension_arithmetic:division:raw_divided_by_base_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Dividing a raw value by a base_dimension results in a base_dimension where each unit exponent is negated with the quotient of the raw value and the original scalar as the new scalar.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_division_division_by_zero_scalar_is_invalid {
   static constexpr const char* id = "dimension_arithmetic:division:division_by_zero_scalar_is_invalid";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Dividing a base_dimension or raw value by zero is a compile-time error.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_exponentiation_base_dimension_exponentiation {
   static constexpr const char* id = "dimension_arithmetic:exponentiation:base_dimension_exponentiation";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "The resulting base_dimension contains the same units, but with all exponents multiplied by the power.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_exponentiation_rational_power {
   static constexpr const char* id = "dimension_arithmetic:exponentiation:rational_power";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "base_dimensions can be raised to a positive rational exponent.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_exponentiation_rational_exponents_may_be_negative {
   static constexpr const char* id = "dimension_arithmetic:exponentiation:rational_exponents_may_be_negative";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "A base_dimension may be raised to a negative rational exponent.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_exponentiation_irrational_power_disallowed {
   static constexpr const char* id = "dimension_arithmetic:exponentiation:irrational_power_disallowed";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "base_dimensions cannot be raised to an irrational exponent.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_addition_and_subtraction_addition_requires_compatible_dimensions {
   static constexpr const char* id = "dimension_arithmetic:addition_and_subtraction:addition_requires_compatible_dimensions";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Adding two base_dimensions is only valid when they represent equivalent physical dimensions and are castable to each other.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_addition_and_subtraction_subtraction_requires_compatible_dimensions {
   static constexpr const char* id = "dimension_arithmetic:addition_and_subtraction:subtraction_requires_compatible_dimensions";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Subtracting two base_dimensions is only valid when they represent equivalent physical dimensions and are castable to each other.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_addition_and_subtraction_addition_incompatible_dimensions_disallowed {
   static constexpr const char* id = "dimension_arithmetic:addition_and_subtraction:addition_incompatible_dimensions_disallowed";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Adding base_dimensions with incompatible structures or dimensions results in a compile-time error.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_addition_and_subtraction_subtraction_incompatible_dimensions_disallowed {
   static constexpr const char* id = "dimension_arithmetic:addition_and_subtraction:subtraction_incompatible_dimensions_disallowed";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Subtracting base_dimensions with incompatible structures or dimensions results in a compile-time error.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_addition_and_subtraction_addition_of_raw_scalar_invalid {
   static constexpr const char* id = "dimension_arithmetic:addition_and_subtraction:addition_of_raw_scalar_invalid";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Adding a raw scalar to a base_dimension is a compile-time error.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_addition_and_subtraction_subtraction_of_raw_scalar_invalid {
   static constexpr const char* id = "dimension_arithmetic:addition_and_subtraction:subtraction_of_raw_scalar_invalid";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Subtracting a raw scalar from a base_dimension is a compile-time error.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_addition_and_subtraction_subtraction_from_raw_scalar_invalid {
   static constexpr const char* id = "dimension_arithmetic:addition_and_subtraction:subtraction_from_raw_scalar_invalid";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Subtracting a base_dimension from a raw scalar is a compile-time error.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_addition_and_subtraction_raw_value_plus_or_minus_dimension_invalid {
   static constexpr const char* id = "dimension_arithmetic:addition_and_subtraction:raw_value_plus_or_minus_dimension_invalid";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Using a raw scalar as the left-hand operand in addition or subtraction with a base_dimension is a compile-time error.";

   template<typename = void>
      static void run() {

   }
};

struct dimension_arithmetic_unary_negation_unary_negation_preserves_dimension {
   static constexpr const char* id = "dimension_arithmetic:unary_negation:unary_negation_preserves_dimension";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Negating a base_dimension yields a base_dimension with the same unit structure and the scalar negated.";

   template<typename = void>
      static void run() {

   }
};

#endif // DIMENSIONAL_COMPILE_TEST_dimension_arithmetic
