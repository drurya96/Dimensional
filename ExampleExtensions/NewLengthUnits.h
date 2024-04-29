#ifndef LENGTH_EXTENSION_H
#define LENGTH_EXTENSION_H

#include "LengthDimension.h"

namespace Dimension
{
   ConversionMap LengthUnit::ExternalMap() {
      ConversionMap map =
      {
         {
            "Feet",
            {
               { "Yard", [](double val) { return val / 3.0; }},
            }
         },
      };
      return map;
   };
}

#endif // LENGTH_EXTENSION_H
