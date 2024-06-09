#ifndef PHYSICS_PROBLEMS_H
#define PHYSICS_PROBLEMS_H

#include <gtest/gtest.h>

class PhysicsProblems : public ::testing::Test {
protected:

   virtual void SetUp() override;

   virtual void TearDown() override {}
};

const double TOLERANCE = 0.01;

#endif // PHYSICS_PROBLEMS_H
