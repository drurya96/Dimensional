#ifndef LENGTH_EXTENSION_H
#define LENGTH_EXTENSION_H

#include "LengthDimension.h"

namespace Dimension
{
   class Initialize_LengthExternalMap
   {
   public:

      static inline bool initialized = false;

      Initialize_LengthExternalMap()
      {
         if (!initialized)
         {
            ConversionMap map =
            {
               {
                  "Feet",
                  {
                     { "Yard", [](double val) { return val / 3.0; }},
                  }
               },
            };

            mergeConversionMaps(LengthUnit::GetConversionMap(), map);

            initialized = true;
         }
      }
   };

   Initialize_LengthExternalMap init_lengthExternalMap;

}

#endif // LENGTH_EXTENSION_H
