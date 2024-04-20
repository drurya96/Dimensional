
#ifndef DIMENSION_LENGTH_H
#define DIMENSION_LENGTH_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

namespace Dimension
{
   /// @brief Length unit, derived from BaseUnit
   class LengthUnit : public BaseUnit
   {
   public:
      /// @brief Constructor only giving name, primary constructor
      LengthUnit(const std::string& name) : BaseUnit(name) {}

      /// @brief Default constructor
      /// @details This default constructor is necessary
      ///    for some template metaprogramming on BaseUnit
      LengthUnit() : BaseUnit() {}

      /// @brief Default destructor
      ~LengthUnit() {}

      /// @brief Override for GetPrimaryUnit
      /// @details Note the definition must appear after
      ///    the global units are defined.
      LengthUnit* GetPrimaryUnit() const override;

   private:

   };

   /// @brief Length dimension, derived from BaseDimension
   /// @details This dimension is a specialization using
   ///    Length as the unit.
   /// @todo Add some convenience methods to retrieve Length by name
   class Length : public BaseDimension<std::tuple<LengthUnit>, std::tuple<>>
   {
   public:

      /// @brief Constructor using value and Length units
      /// @param[in] value The value to set
      /// @param[in] Length Pointer to the Length unit
      explicit Length(double value, LengthUnit* unit) 
         : BaseDimension<std::tuple<LengthUnit>, std::tuple<>>(value, std::vector<BaseUnit*>{ static_cast<BaseUnit*>(unit) }, std::vector<BaseUnit*>{})
      {}

      /// @brief Cast operator from a BaseDimension
      Length(const BaseDimension<std::tuple<LengthUnit>, std::tuple<>>& base) : BaseDimension<std::tuple<LengthUnit>, std::tuple<>>(base)
      {}

   };

   namespace LengthUnits
   {
      /// @brief Meters, a LengthUnit global variable for creating Dimension objects
      inline LengthUnit Meters("Meters");

      /// @brief Feet, a LengthUnit global variable for creating Dimension objects
      inline LengthUnit Feet("Feet");
   }

   inline LengthUnit* LengthUnit::GetPrimaryUnit() const { return &LengthUnits::Meters; }
   
   /// @brief Static vector of all LengthUnits
   /// @details This vector of all LengthUnits is stored in each
   ///    LengthUnit object for use in simplification and other
   ///    helper functions
   /// @todo Consider other ways to handle this
   static std::vector<BaseUnit*> LengthUnitVector;

   /// @brief Add conversions to each LengthUnit
   /// @details Adds conversions to all Length units and assigns the 
   ///    LengthUnitVector to each. This method also validates each 
   ///    LengthUnit as a conversion to the "primary" LengthUnit, Meters,
   ///    and that Meters has a conversion to each LengthUnit.
   ///    Note this must be called at the start of the program.
   /// @return bool indicating success
   inline bool initializeLengthUnits()
   {
      LengthUnits::Meters.add_conversion(LengthUnits::Feet, [](double val) {return val * 3.28084; });
      LengthUnits::Feet.add_conversion(LengthUnits::Meters, [](double val) {return val / 3.28084; });

      LengthUnitVector.push_back(& LengthUnits::Meters);
      LengthUnitVector.push_back(& LengthUnits::Feet);

      return BaseUnit::ValidateConversions(LengthUnitVector, LengthUnits::Meters);
   }
}

#endif // DIMENSION_LENGTH_H