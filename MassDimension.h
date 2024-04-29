
#ifndef DIMENSION_MASS_H
#define DIMENSION_MASS_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

namespace Dimension
{
   // Forward declaration
   class MassUnit;

   /// @brief UnitFactory templated on MassUnit
   class MassUnitFactory : public UnitFactory<MassUnit, MassUnitFactory> {};

   /// @brief Mass unit, derived from BaseUnit
   class MassUnit : public BaseUnit<MassUnit>
   {
   public:
      /// @brief Constructor only giving name, primary constructor
      MassUnit(const std::string& name) : BaseUnit<MassUnit>(name) {}

      /// @brief Default constructor
      /// @details This default constructor is necessary
      ///    for some template metaprogramming on BaseUnit
      MassUnit() : BaseUnit<MassUnit>() {}

      MassUnit& operator()() {
         static MassUnit instance;
         return instance;
      }

      /// @brief Default destructor
      ~MassUnit() {}

      /// @brief Override for GetPrimaryUnit
      /// @details Note the definition must appear after
      ///    the global units are defined.
      MassUnit* GetPrimaryUnit() const override;

      /// @brief Get user-defined conversion map
      /// @details (WIP) The conversion map returned by this function
      ///    will be added to the included conversion map below.
      ///    The default implementation returns an empty map, but a 
      ///    new implementation may be provided in a standalone header.
      ///    This requires passing the compiler definition EXTERNAL_MASS_MAP. 
      /// @todo This is a crude solution, but works for now. Revisit ASAP
      static ConversionMap ExternalMap();

      /// @brief Defines a static conversion map
      /// @details This static conversion map represents the operations
      ///    needed to convert from one unit to another.
      ///    All units **MUST** have a conversion to the Primary unit
      ///    and the primary unit **MUST** have a conversion to each unit.
      /// @return The conversion map
      static ConversionMap GetConversionMap()
      {
         static ConversionMap map =
         {
            { 
               "Gram", 
               {
                  { "Pound", [](double val) { return val / 453.59237; }},
                  { "Ounce", [](double val) { return val / 28.34952; }},
                  { "Ton", [](double val) { return val / 907200.0; }}, 
                  { "Tonne", [](double val) { return val / 1000000.0; }},
               }
            },
            { 
               "Pound", 
               {
                  { "Gram", [](double val) { return val * 453.6; }},
               }
            },
            {
               "Ounce",
               {
                  { "Gram", [](double val) { return val * 28.35; }},
               }
            },
            {
               "Ton",
               {
                  { "Gram", [](double val) { return val * 907200.0; }},
               }
            },
            {
               "Tonne",
               {
                  { "Gram", [](double val) { return val * 1000000.0; }},
               }
            },
         };

         #ifdef EXTERNAL_MASS_MAP
            ConversionMap externalMap = ExternalMap();
            mergeConversionMaps(map, externalMap);
         #endif

         return map;
      }

      /// @brief Call GetInstance using the corresponding UnitFactory
      static MassUnit& GetInstance(const std::string& name, bool initialize = true)
      {
         return MassUnitFactory::GetInstance(name, initialize);
      }

   private:

   };

   using MassTup = std::tuple<MassUnit*>;

   /// @brief Mass dimension, derived from BaseDimension
   /// @details This dimension is a specialization using
   ///    Mass as the unit.
   /// @todo Add some convenience methods to retrieve Mass by name
   class Mass : public BaseDimension<MassTup, std::tuple<>>
   {
   public:

      /// @brief Constructor using value and Mass units
      /// @param[in] value The value to set
      /// @param[in] Mass Pointer to the Mass unit
      explicit Mass(double value, MassUnit* unit) 
         : BaseDimension<MassTup, std::tuple<>>(value, MassTup{ unit }, std::tuple<>{})
      {}

      /// @brief Cast operator from a BaseDimension
      Mass(const BaseDimension<MassTup, std::tuple<>>& base) : BaseDimension<MassTup, std::tuple<>>(base)
      {}

   };

   namespace MassUnits
   {
      /// @brief Gram singleton
      inline static MassUnit& Gram() {
         MassUnit& gram = MassUnitFactory::GetInstance("Gram");
         return gram;
      }

      /// @brief Pound singleton
      /// @details Using Pound as a mass is for simplicity and assumes
      ///    standard earth gravity
      /// @todo Revisit this usage, consider making a tight relationship
      ///    between Mass and Weight
      inline static MassUnit& Pound() {
         MassUnit& pound = MassUnitFactory::GetInstance("Pound");
         return pound;
      }

      /// @brief Ounce singleton
      /// @details Using Ounce as a mass is for simplicity and assumes
      ///    standard earth gravity
      /// @todo Revisit this usage, consider making a tight relationship
      ///    between Mass and Weight
      inline static MassUnit& Ounce() {
         MassUnit& ounce = MassUnitFactory::GetInstance("Ounce");
         return ounce;
      }

      /// @brief Ton singleton
      /// @details Using Ton as a mass is for simplicity and assumes
      ///    standard earth gravity
      /// @todo Revisit this usage, consider making a tight relationship
      ///    between Mass and Weight
      inline static MassUnit& Ton() {
         MassUnit& ton = MassUnitFactory::GetInstance("Ton");
         return ton;
      }

      /// @brief Tonne singleton
      inline static MassUnit& Tonne() {
         MassUnit& tonne = MassUnitFactory::GetInstance("Tonne");
         return tonne;
      }
   }

   /// @brief Return a pointer to the primary unit
   /// @details Return a pointer to Seconds, the primary unit
   /// @return Pointer to Seconds
   inline MassUnit* MassUnit::GetPrimaryUnit() const { return &MassUnits::Gram(); }
   
   /// @brief Static vector of all MassUnits
   /// @details This vector of all MassUnits is stored in each
   ///    MassUnit object for use in simplification and other
   ///    helper functions
   /// @todo Consider other ways to handle this
   static std::vector<BaseUnit<MassUnit>*> MassUnitVector;

   /// @brief default implementation if one is not provided
   #ifndef EXTERNAL_MASS_MAP
      inline ConversionMap LengthUnit::ExternalMap() { return ConversionMap{}; };
   #endif 
}

#endif // DIMENSION_MASS_H
