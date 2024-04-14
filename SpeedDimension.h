
#ifndef DIMENSION_SPEED_H
#define DIMENSION_SPEED_H

#include "BaseDimension.h"

#include "LengthDimension.h"
#include "TimeDimension.h"

namespace Dimension
{
   class Speed : public BaseDimension<LengthUnit<>, TimeUnit<Inverse>>
   {
   public:

      explicit Speed(double value, LengthUnit<>* length, TimeUnit<>* time)
         : BaseDimension<LengthUnit<>, TimeUnit<Inverse>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(length) }, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(time) })//, numList({ unit }), denList({})
      {}

      Speed(const BaseDimension<LengthUnit<>, TimeUnit<Inverse>>& base) : BaseDimension<LengthUnit<>, TimeUnit<Inverse>>(base.value, base.numList, base.denList)
      {}
      
   };

}

#endif // DIMENSION_SPEED_H