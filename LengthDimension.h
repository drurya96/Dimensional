
#ifndef DIMENSION_LENGTH_H
#define DIMENSION_LENGTH_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

namespace Dimension
{
   // Forward declaration
   class LengthUnit;

   /// @brief UnitFactory templated on LengthUnit
   class LengthUnitFactory : public UnitFactory<LengthUnit, LengthUnitFactory> {};

   /// @brief Length unit, derived from BaseUnit
   class LengthUnit : public BaseUnit<LengthUnit>
   {
   public:
      /// @brief Constructor only giving name, primary constructor
      LengthUnit(const std::string& name) : BaseUnit<LengthUnit>(name) {}

      /// @brief Default constructor
      /// @details This default constructor is necessary
      ///    for some template metaprogramming on BaseUnit
      LengthUnit() : BaseUnit<LengthUnit>() {}

      LengthUnit& operator()() {
         static LengthUnit instance;
         return instance;
      }

      /// @brief Default destructor
      ~LengthUnit() {}

      /// @brief Override for GetPrimaryUnit
      /// @details Note the definition must appear after
      ///    the global units are defined.
      LengthUnit* GetPrimaryUnit() const override;

      /// @brief Defines a static conversion map
      /// @details This static conversion map represents the operations
      ///    needed to convert from one unit to another.
      ///    All units **MUST** have a conversion to the Primary unit
      ///    and the primary unit **MUST** have a conversion to each unit.
      /// @return The conversion map
      static ConversionMap GetConversionMap()
      {
         static const ConversionMap map =
         {
            { 
               "Meters", 
               {
                  { "Feet", [](double val) { return val * 3.28084; }}, 
               }
            },
            { 
               "Feet", 
               {
                  { "Meters", [](double val) { return val / 3.28084; }},
               }
            },
         };
         return map;
      }

      /// @brief Call GetInstance using the corresponding UnitFactory
      static LengthUnit& GetInstance(const std::string& name, bool initialize = true)
      {
         return LengthUnitFactory::GetInstance(name, initialize);
      }

   private:

   };

   using LengthTup = std::tuple<LengthUnit*>;

   /// @brief Length dimension, derived from BaseDimension
   /// @details This dimension is a specialization using
   ///    Length as the unit.
   /// @todo Add some convenience methods to retrieve Length by name
   class Length : public BaseDimension<LengthTup, std::tuple<>>
   {
   public:

      /// @brief Constructor using value and Length units
      /// @param[in] value The value to set
      /// @param[in] Length Pointer to the Length unit
      explicit Length(double value, LengthUnit* unit) 
         : BaseDimension<LengthTup, std::tuple<>>(value, LengthTup{ unit }, std::tuple<>{})
      {}

      /// @brief Cast operator from a BaseDimension
      Length(const BaseDimension<LengthTup, std::tuple<>>& base) : BaseDimension<LengthTup, std::tuple<>>(base)
      {}

   };

   namespace LengthUnits
   {
      /// @brief Meters singleton
      inline static LengthUnit& Meters() {
         LengthUnit& meters = LengthUnitFactory::GetInstance("Meters");
         return meters;
      }

      /// @brief Feet singleton
      inline static LengthUnit& Feet() {
         LengthUnit& feet = LengthUnitFactory::GetInstance("Feet");
         return feet;
      }
   }

   /// @brief Return a pointer to the primary unit
   /// @details Return a pointer to Seconds, the primary unit
   /// @return Pointer to Seconds
   inline LengthUnit* LengthUnit::GetPrimaryUnit() const { return &LengthUnits::Meters(); }
   
   /// @brief Static vector of all LengthUnits
   /// @details This vector of all LengthUnits is stored in each
   ///    LengthUnit object for use in simplification and other
   ///    helper functions
   /// @todo Consider other ways to handle this
   static std::vector<BaseUnit<LengthUnit>*> LengthUnitVector;
}

#endif // DIMENSION_LENGTH_H