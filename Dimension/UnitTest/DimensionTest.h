#ifndef DIMENSION_TEST_H
#define DIMENSION_TEST_H

#include <gtest/gtest.h>

class DimensionTest : public ::testing::Test {
protected:

   virtual void SetUp() override;

   virtual void TearDown() override;
};

const double TOLERANCE = 0.01;

#endif // DIMENSION_TEST_H
