

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

      //LengthUnit<Inverse> Inverse_Meters = LengthUnit<Inverse>("Meters");
      //LengthUnit<Inverse> Inverse_Feet = LengthUnit<Inverse>("Feet");
   }

   namespace SpeedUnits
   {
      SpeedUnit<> MetersPerSecond = SpeedUnit<>(LengthUnits::Meters, TimeUnits::Seconds, "MetersPerSecond");
      SpeedUnit<> FeetPerSecond = SpeedUnit<>(LengthUnits::Feet, TimeUnits::Seconds, "FeetPerSecond");
   }
}