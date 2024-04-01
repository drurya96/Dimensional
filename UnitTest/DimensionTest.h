#ifndef DIMENSION_TEST_H
#define DIMENSION_TEST_H

#include <gtest/gtest.h>

class DimensionTest : public ::testing::Test {
protected:
   // You can put setup code here if needed
   virtual void SetUp() override;

   // You can put cleanup code here if needed
   virtual void TearDown() override {}
};

#endif // DIMENSION_TEST_H
