#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

/// These are definitions of singleton BaseUnit objects.
/// @todo Revisit these definitions to attempt moving them
///    into headers.

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