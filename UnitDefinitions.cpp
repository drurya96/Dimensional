

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

namespace Dimension
{
   namespace TimeUnits
   {
      TimeUnit<> Seconds = TimeUnit<>("Seconds");
      TimeUnit<> Minutes = TimeUnit<>("Minutes");
   }

   namespace LengthUnits
   {
      LengthUnit<> Meters = LengthUnit<>("Meters");
      LengthUnit<> Feet = LengthUnit<>("Feet");
   }
}