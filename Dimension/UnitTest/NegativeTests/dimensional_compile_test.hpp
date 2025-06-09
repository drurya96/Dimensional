#ifndef DIMENSIONAL_COMPILE_TEST_HPP
#define DIMENSIONAL_COMPILE_TEST_HPP

#include "LengthDimension.h"
#include "TimeDimension.h"

using namespace Dimension;

struct simple_cast_test {
   static constexpr const char* id = "simple_cast";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Cast a length to a base_dimension length";

   template<typename = void>
   static void run() {
      Length<Meters> myLength{10.0};
      BaseDimension<UnitExponent<Meters>> other = myLength;
   }
};

struct invalid_cast_test {
   static constexpr const char* id = "invalid_cast";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Fail to cast from one dimension to another";

   template<typename = void>
   static void run() {
      Length<Meters> myLength{10.0};
      Time myTime = myLength;
   }
};

#endif // DIMENSIONAL_COMPILE_TEST_HPP
