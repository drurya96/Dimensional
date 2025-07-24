#ifndef DIMENSION_TEMPERATURE_EXTRAS_H
#define DIMENSION_TEMPERATURE_EXTRAS_H

namespace dimension
{

   struct celsius : public reference_frame<kelvin>{ static constexpr double offset = 273.15; };
   struct fahrenheit : public reference_frame<rankine>{ static constexpr double offset = 459.67; };

}

#endif //DIMENSION_TEMPERATURE_EXTRAS_H