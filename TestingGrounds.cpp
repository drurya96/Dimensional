#include <type_traits>
#include <TimeDimension.h>
#include <LengthDimension.h>

namespace Dimension {
   /*
   // Forward declarations of myClass templates
   template<typename ... T_Flag>
   class myClass1 {};

   template<typename ... T_Flag>
   class myClass2 {};

   template<typename ... T_Flag>
   class myClass3 {};

   */
}

int main() {
   BaseDimension<myUnitTypes<>, myUnitTypes<myClass3<bool>>, myUnitTypes<myClass1<bool>>, myUnitTypes<myClass2<>>> obj1;
   BaseDimension<myUnitTypes<myClass1<bool>>, myUnitTypes<myClass3<>>, myUnitTypes<myClass1<>>, myUnitTypes<myClass2<bool>>> newObj1 = myMethod(obj1);

   return 0;
}