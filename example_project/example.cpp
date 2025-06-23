#include "length_dimension.h"
#include "timespan_dimension.h"
#include "speed_dimension.h"

#include <iostream>

int main()
{

   using namespace dimension;

   length<meters> length(5.0);
   Time<seconds> time(10.0);

   speed speed = length / time;

   std::cout << speed << std::endl;

}
