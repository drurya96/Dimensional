#include <iostream>
#include "StaticTime.h"

using namespace StaticDimension;

int main() {
    std::cout << "Testing Dimensional package!" << std::endl;
    // Add code here to test any functionality from your library

    Time<Seconds> myTime{};

    std::cout << myTime.GetTime<Minutes>() << std::endl;

    return 0;
}