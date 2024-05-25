#ifndef LENGTH_EXTENSION_H
#define LENGTH_EXTENSION_H

#include "StaticLength.h"

namespace StaticDimension
{
   class Yards : public LengthUnit<Yards> { public: using LengthUnit::LengthUnit; };
   /*
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
   */


   template<>
   inline double ConvertDouble<Yards, Meters>(double input)
   {
      return input / 1.0936132983;
   }

   template<>
   inline double ConvertDouble<Meters, Yards>(double input)
   {
      return input * 1.0936132983;
   }
}

#endif // LENGTH_EXTENSION_H
