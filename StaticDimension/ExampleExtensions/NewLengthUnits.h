#ifndef LENGTH_EXTENSION_H
#define LENGTH_EXTENSION_H

#include "StaticLength.h"

namespace StaticDimension
{
   class Yards : public LengthUnit<Yards> { public: using LengthUnit::LengthUnit; };

   template<>
   inline double Convert<Yards, Meters>(double input)
   {
      return input / 1.0936132983;
   }

   template<>
   inline double Convert<Meters, Yards>(double input)
   {
      return input * 1.0936132983;
   }
}

#endif // LENGTH_EXTENSION_H
