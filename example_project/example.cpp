#include "LengthDimension.h"
#include "TimeDimension.h"
#include "SpeedDimension.h"

#include <iostream>

int main()
{

   using namespace Dimension;

   Length<Meters> length(5.0);
   Time<Seconds> time(10.0);

   Speed speed = length / time;

   std::cout << speed << std::endl;

}
