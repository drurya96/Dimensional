#ifndef LENGTH_EXTENSION_H
#define LENGTH_EXTENSION_H

#include "LengthDimension.h"

namespace Dimension
{
   class Yards : public LengthUnit<Yards> { public: using LengthUnit::LengthUnit; };

   template<>
   inline Meters ConvertValue<Yards, Meters>(const Yards& obj)
   {
      return Meters(obj.GetValue() / 1.0936132983);
   }

   template<>
   inline Yards ConvertValue<Meters, Yards>(const Meters& obj)
   {
      return Yards(obj.GetValue() * 1.0936132983);
   }
}

#endif // LENGTH_EXTENSION_H
