
#ifndef DIMENSION_TIME_H
#define DIMENSION_TIME_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

namespace Dimension
{
   // Forward declaration
   class TimeUnit;

   /// @brief UnitFactory templated on TimeUnit
   class TimeUnitFactory : public UnitFactory<TimeUnit, TimeUnitFactory> {};

   /// @brief Time unit, derived from BaseUnit
   class TimeUnit : public BaseUnit<TimeUnit>
   {
   public:
      /// @brief Constructor only giving name, primary constructor
      TimeUnit(const std::string& name) : BaseUnit<TimeUnit>(name) {}

      /// @brief Default constructor
      /// @details This default constructor is necessary
      ///    for some template metaprogramming on BaseUnit
      TimeUnit() : BaseUnit<TimeUnit>() {}

      TimeUnit& operator()() {
         static TimeUnit instance;
         return instance;
      }

      /// @brief Default destructor
      ~TimeUnit() {}

      /// @brief Override for GetPrimaryUnit
      /// @details Note the definition must appear after
      ///    the global units are defined.
      TimeUnit* GetPrimaryUnit() const override;

      /// @brief Get user-defined conversion map
      /// @details (WIP) The conversion map returned by this function
      ///    will be added to the included conversion map below.
      ///    The default implementation returns an empty map, but a 
      ///    new implementation may be provided in a standalone header.
      ///    This requires passing the compiler definition EXTERNAL_TIME_MAP. 
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
         static const ConversionMap map =
         {
            {
               "Seconds",
               {
                  { "Minutes", [](double val) { return val / 60.0;   }},
                  { "Hours"  , [](double val) { return val / 3600.0; }},
                  { "Day"  , [](double val) { return val / 86400.0; }},
                  { "Week"  , [](double val) { return val / 604800.0; }},
                  { "Year"  , [](double val) { return val / 31536000.0; }},
                  { "AstroYear"  , [](double val) { return val / 31557600.0; }},
               }
            },
            {
               "Minutes",
               {
                  { "Seconds", [](double val) { return val * 60.0; }},
                  { "Hours", [](double val) { return val / 60.0; }},
               }
            },
            {
               "Hours",
               {
                  { "Seconds", [](double val) { return val * 3600.0; } },
                  { "Day", [](double val) { return val / 24.0; } },
               }
            },
            {
               "Day",
               {
                  { "Seconds", [](double val) { return val * 86400.0; } },
                  { "Hours", [](double val) { return val * 24.0; } },
               }
            },
            {
               "Week",
               {
                  { "Seconds", [](double val) { return val * 604800.0; } },
                  { "Hours", [](double val) { return val * 168.0; } },
                  { "Day", [](double val) { return val * 7.0; } },
               }
            },
            {
               "Year",
               {
                  { "Seconds", [](double val) { return val * 31536000.0; } },
               }
            },
            {
               "AstroYear",
               {
                  { "Seconds", [](double val) { return val * 31557600.0; } },
               }
            },
         };

         #ifdef EXTERNAL_TIME_MAP
            ConversionMap externalMap = ExternalMap();
            mergeConversionMaps(map, externalMap);
         #endif

         return map;
      }

      /// @brief Call GetInstance using the corresponding UnitFactory
      static TimeUnit& GetInstance(const std::string& name, bool initialize = true)
      {
         return TimeUnitFactory::GetInstance(name, initialize);
      }

   private:

   };

   using TimeTup = std::tuple<TimeUnit*>;

   /// @brief Time dimension, derived from BaseDimension
   /// @details This dimension is a specialization using
   ///    Time as the unit.
   /// @todo Add some convenience methods to retrieve Time by name
   class Time : public BaseDimension<TimeTup, std::tuple<>>
   {
   public:

      /// @brief Constructor using value and time units
      /// @param[in] value The value to set
      /// @param[in] time Pointer to the time unit
      explicit Time(double value, TimeUnit* unit)
         : BaseDimension<TimeTup, std::tuple<>>(value, TimeTup{ unit }, std::tuple<>{})
      {}

      /// @brief Cast operator from a BaseDimension
      Time(const BaseDimension<TimeTup, std::tuple<>>& base) : BaseDimension<TimeTup, std::tuple<>>(base)
      {}

   };

   namespace TimeUnits
   {
      /// @brief Seconds singleton
      inline static TimeUnit& Seconds() {
         TimeUnit& seconds = TimeUnitFactory::GetInstance("Seconds");
         return seconds;
      };

      /// @brief Minutes singleton
      inline static TimeUnit& Minutes() {
         TimeUnit& minutes = TimeUnitFactory::GetInstance("Minutes");
         return minutes;
      };

      /// @brief Hours singleton
      inline static TimeUnit& Hours() {
         TimeUnit& hours = TimeUnitFactory::GetInstance("Hours");
         return hours;
      };

      /// @brief Day singleton
      /// @details This represents a calendar day (24 hours)
      inline static TimeUnit& Day() {
         TimeUnit& day = TimeUnitFactory::GetInstance("Day");
         return day;
      };

      /// @brief Week singleton
      inline static TimeUnit& Week() {
         TimeUnit& week = TimeUnitFactory::GetInstance("Week");
         return week;
      };

      /// @brief Year singleton
      /// @details This represents a calendar year (365 days)
      inline static TimeUnit& Year() {
         TimeUnit& year = TimeUnitFactory::GetInstance("Year");
         return year;
      };

      /// @brief Year singleton
      /// @details This represents a astronomical year (365.25 days)
      inline static TimeUnit& AstroYear() {
         TimeUnit& year = TimeUnitFactory::GetInstance("Year");
         return year;
      };

      // TODO: Add leap year
   };

   /// @brief Return a pointer to the primary unit
   /// @details Return a pointer to Seconds, the primary unit
   /// @return Pointer to Seconds
   inline TimeUnit* TimeUnit::GetPrimaryUnit() const { return &TimeUnits::Seconds(); }
   

   /// @brief Static vector of all TimeUnits
   /// @details This vector of all TimeUnits is stored in each
   ///    TimeUnit object for use in simplification and other
   ///    helper functions
   /// @todo Consider other ways to handle this
   static std::vector<BaseUnit<TimeUnit>*> TimeUnitVector;

   /// @brief default implementation if one is not provided
   #ifndef EXTERNAL_TIME_MAP
      inline ConversionMap LengthUnit::ExternalMap() { return ConversionMap{}; };
   #endif 
}

#endif // DIMENSION_TIME_H
