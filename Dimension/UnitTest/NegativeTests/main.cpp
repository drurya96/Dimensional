#define USE_DOUBLE
#include "dimensional_compile_test.hpp"

using test_struct = TEST_STRUCT_NAME;

int main() {
    test_struct::run<>();
    return 0;
}