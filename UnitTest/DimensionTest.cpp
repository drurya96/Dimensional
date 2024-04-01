#include "DimensionTest.h"


#include "TimeDimension.h"
#include "LengthDimension.h"

void DimensionTest::SetUp()
{
   using namespace Dimension;
   initializeTimeUnits();
   //Length::initializeLengthUnits();
   initializeLengthUnits();
}
