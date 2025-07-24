#ifndef DIMENSIONAL_COMPILE_TEST_HPP
#define DIMENSIONAL_COMPILE_TEST_HPP

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct simple_cast_test {
   static constexpr const char* id = "simple_cast";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "Cast a length to a base_dimension length";

   template<typename = void>
   static void run() {
      //length<meters> mylength{10.0};
      //base_dimension<unit_exponent<meters>> other = mylength;
      time<seconds> a{5};
   }
};

struct invalid_cast_test {
   static constexpr const char* id = "invalid_cast";
   static constexpr bool expect_error = true;
   static constexpr const char* description = "Fail to cast from one dimension to another";

   template<typename = void>
   static void run() {
      //length<meters> mylength{10.0};
      //Time myTime = mylength;
      static_assert(false);
   }
};

#endif // DIMENSIONAL_COMPILE_TEST_HPP
