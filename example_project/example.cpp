#include "dimensional.h"

#include <iostream>

int main()
{

   using namespace dimension;

   length<meters> length(5.0);
   timespan<seconds> time(10.0);

   speed speed = length / time;

   std::cout << speed << std::endl;

}
