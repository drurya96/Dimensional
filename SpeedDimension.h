
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
   class Speed : public BaseDimension<LengthUnit<>, TimeUnit<Inverse>>
   {
   public:
      /// @brief Constructor value, length, and time units
      /// @param[in] value The value to set
      /// @param[in] length Pointer to the length unit
      /// @param[in] time Pointer to the time unit
      explicit Speed(double value, LengthUnit<>* length, TimeUnit<>* time)
         : BaseDimension<LengthUnit<>, TimeUnit<Inverse>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(length) }, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(time) })//, numList({ unit }), denList({})
      {}

      /// @brief Cast operator from a BaseDimension
      Speed(const BaseDimension<LengthUnit<>, TimeUnit<Inverse>>& base) : BaseDimension<LengthUnit<>, TimeUnit<Inverse>>(base.value, base.numList, base.denList)
      {}
      
   };

}

#endif // DIMENSION_SPEED_H