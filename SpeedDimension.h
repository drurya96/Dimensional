
#ifndef DIMENSION_SPEED_H
#define DIMENSION_SPEED_H

#include "BaseDimension.h"

#include "LengthDimension.h"
#include "TimeDimension.h"

namespace Dimension
{
   /// @brief Speed dimension, derived from BaseDimension
   /// @details This dimension is a specialization using
   ///    Length and Inverse Time as units.
   /// @todo Add some convenience methods to retrieve Speed by name,
   ///    such as MetersPerSecond, MilesPerHour, etc.
   class Speed : public BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>
   {
   public:
      /// @brief Constructor value, length, and time units
      /// @param[in] value The value to set
      /// @param[in] length Pointer to the length unit
      /// @param[in] time Pointer to the time unit
      explicit Speed(double value, LengthUnit* length, TimeUnit* time)
         : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(value, std::vector<BaseUnit*>{ static_cast<BaseUnit*>(length) }, std::vector<BaseUnit*>{ static_cast<BaseUnit*>(time) })
      {}

      /// @brief Cast operator from a BaseDimension
      Speed(const BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>& base) : BaseDimension<std::tuple<LengthUnit>, std::tuple<TimeUnit>>(base)
      {}
      
   };
}

#endif // DIMENSION_SPEED_H