#ifndef DIMENSIONAL_COMPILE_TEST_Other
#define DIMENSIONAL_COMPILE_TEST_Other

#include "length_dimension.h"
#include "timespan_dimension.h"

using namespace dimension;

struct test_struct {
   static constexpr const char* id = "test";
   static constexpr bool expect_error = false;
   static constexpr const char* description = "test";

   template<typename = void>
      static void run() {
         
   }
};


#endif // DIMENSIONAL_COMPILE_TEST_Other
