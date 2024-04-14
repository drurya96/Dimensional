
#ifndef DIMENSION_LENGTH_H
#define DIMENSION_LENGTH_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

#include <cassert> // Necessary for assert in initializeLengthUnits. TODO: Remove if logic changes

namespace Dimension
{
   template<typename ... is_inverse>
   class LengthUnit : public BaseUnit<is_inverse...>
   {
   public:
      LengthUnit(std::string name) : BaseUnit<is_inverse...>(name) {}

      LengthUnit() : BaseUnit<is_inverse...>() {}

      std::string GetDimName() override { return "Length"; }
      std::string GetUnitName() override { return unitName; }
      LengthUnit<>* GetBaseUnit() override { return &LengthUnits::Meters; }

      // TODO: Add logic ensureing conversion is successfully added
      bool add_conversion(LengthUnit toUnit, std::function<double(double)> conversion)
      {
         conversions[toUnit.unitName] = conversion;
         return true;
      };

   private:

   };

   template<typename ... is_inverse>
   class Length : public BaseDimension<LengthUnit<is_inverse...>>
   {
   public:
      explicit Length(double value, LengthUnit<is_inverse...>* unit) 
         : BaseDimension<LengthUnit<is_inverse...>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(unit) }, std::vector<BaseUnit<>*>{})
      {}

      Length(const BaseDimension<LengthUnit<is_inverse...>>& base) : BaseDimension<LengthUnit<is_inverse...>>(base.value, base.numList, base.denList)
      {}

   };

   namespace LengthUnits
   {
      extern LengthUnit<> Meters;
      extern LengthUnit<> Feet;
   }
   
   static std::vector<BaseUnit<>*> LengthUnitVector;

   inline bool initializeLengthUnits()
   {
      LengthUnits::Meters.add_conversion(LengthUnits::Feet, [](double val) {return val * 3.28084; });
      LengthUnits::Feet.add_conversion(LengthUnits::Meters, [](double val) {return val / 3.28084; });

      LengthUnits::Meters.baseUnitVector = &LengthUnitVector;
      LengthUnits::Feet.baseUnitVector = &LengthUnitVector;

      LengthUnitVector.push_back(& LengthUnits::Meters);
      LengthUnitVector.push_back(& LengthUnits::Feet);


      // Validate there is a conversion to Meters from each unit
      // TODO: Consider returning false instead of asserting
      for (auto unit : LengthUnitVector)
      {
         auto findit = unit->conversions.find(LengthUnits::Meters.GetUnitName());
         auto findit2 = LengthUnits::Meters.conversions.find(unit->GetUnitName());
         assert((findit != unit->conversions.end()) || (unit == &LengthUnits::Meters));
         assert((findit2 != LengthUnits::Meters.conversions.end()) || (unit == &LengthUnits::Meters));
      }

      // TODO: Return false when necessary
      return true;
   }

}

#endif // DIMENSION_LENGTH_H