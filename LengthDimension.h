
#ifndef DIMENSION_LENGTH_H
#define DIMENSION_LENGTH_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

namespace Dimension
{
   template<typename ... is_inverse>
   class LengthUnit : public BaseUnit<is_inverse...>
   {
   public:
      explicit LengthUnit(std::string name) :
         unitName(name)
      {
         //numList.push_back(this);
      };

      LengthUnit() :
         unitName("")
      {};

      std::string unitName;

      // The intention is to store a map of conversion functions to each known unit, minimally the "default" unit of this dimension
      std::unordered_map<std::string, std::function<double(double)>> conversions;

      bool add_conversion(LengthUnit toUnit, std::function<double(double)> conversion)
      {
         conversions[toUnit.unitName] = conversion;
         return true;
      };

   private:
      constexpr double GetStandardID() override { return 3.0; };
      constexpr double GetInverseID() override { return 1 / 3.0; };

   };

   template<typename ... is_inverse>
   class Length : public BaseDimension<LengthUnit<is_inverse...>>
   {
   public:
      explicit Length(double value, LengthUnit<is_inverse...>* unit) 
         : BaseDimension<LengthUnit<is_inverse...>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(unit) }, std::vector<BaseUnit<>*>{})//, numList({ unit }), denList({})
      {

      };

      /*
      operator BaseDimension<LengthUnit<is_inverse...>>() const {
         std::cout << "Length Cast to BaseDimension" << std::endl;
         return BaseDimension<LengthUnit<is_inverse...>>(value);
      }
      */
      


      //double value;
      //LengthUnit<is_inverse...>* unit;

      double GetVal(LengthUnit<is_inverse...>*);

      //static bool initializeLengthUnits();
   };

   namespace LengthUnits
   {
      extern LengthUnit<> Meters;
      extern LengthUnit<> Feet;
      //extern LengthUnit<Inverse> Inverse_Meters;
      //extern LengthUnit<Inverse> Inverse_Feet;
   }
   
   inline bool initializeLengthUnits()
   {
      LengthUnits::Meters.add_conversion(LengthUnits::Feet, [](double val) {return val * 3.28084; });
      LengthUnits::Feet.add_conversion(LengthUnits::Meters, [](double val) {return val / 3.28084; });

      //LengthUnits::Meters.add_conversion(LengthUnits::Feet, [](Length& length) {return length.value * 3.28084; });
      //LengthUnits::Feet.add_conversion(LengthUnits::Meters, [](Length& length) {return length.value / 3.28084; });
      return true;
   }


   template<typename... is_inverse>
   inline double Length<is_inverse...>::GetVal(LengthUnit<is_inverse...>* getUnit)
   {
      /*
      if (getUnit == unit)
      {
         //return value;
         return 0;
      }
      else
      {
         //return unit->conversions[getUnit->unitName](this->value);
         return 0;
      }
      */
      return 0; // temporarily disabling this logic to debug constructors
   }

}

#endif // DIMENSION_LENGTH_H