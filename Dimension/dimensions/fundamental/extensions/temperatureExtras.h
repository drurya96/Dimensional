#ifndef DIMENSION_TEMPERATURE_EXTRAS_H
#define DIMENSION_TEMPERATURE_EXTRAS_H

#include "Dimension_Core/internal_temp/point/frame_traits.h"

namespace dimension
{
   struct celsius{};
   struct fahrenheit{};

   template<> struct frame_traits<celsius> {
      using unit = kelvin;
      static constexpr double offset = 273.15;   // K = C + 273.15
   };

   template<> struct frame_traits<fahrenheit> {
      using unit = rankine;
      static constexpr double offset = 459.67;   // R = F + 459.67
   };

}

#endif //DIMENSION_TEMPERATURE_EXTRAS_H