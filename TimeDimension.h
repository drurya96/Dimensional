
#ifndef DIMENSION_TIME_H
#define DIMENSION_TIME_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

namespace Dimension
{

   /// @brief Time unit, derived from BaseUnit
   class TimeUnit : public BaseUnit
   {
   public:
      /// @brief Constructor only giving name, primary constructor
      TimeUnit(const std::string& name) : BaseUnit(name) {}

      /// @brief Default constructor
      /// @details This default constructor is necessary
      ///    for some template metaprogramming on BaseUnit
      TimeUnit() : BaseUnit() {}

      /// @brief Default destructor
      ~TimeUnit() {}

      /// @brief Override for GetPrimaryUnit
      /// @details Note the definition must appear after
      ///    the global units are defined.
      TimeUnit* GetPrimaryUnit() const override;

   private:

   };

   /// @brief Time dimension, derived from BaseDimension
   /// @details This dimension is a specialization using
   ///    Time as the unit.
   /// @todo Add some convenience methods to retrieve Time by name
   class Time : public BaseDimension<std::tuple<TimeUnit>, std::tuple<>>
   {
   public:

      /// @brief Constructor using value and time units
      /// @param[in] value The value to set
      /// @param[in] time Pointer to the time unit
      explicit Time(double value, TimeUnit* unit)
         : BaseDimension<std::tuple<TimeUnit>, std::tuple<>>(value, std::vector<BaseUnit*>{ static_cast<BaseUnit*>(unit) }, std::vector<BaseUnit*>{})
      {}

      /// @brief Cast operator from a BaseDimension
      Time(const BaseDimension<std::tuple<TimeUnit>, std::tuple<>>& base) : BaseDimension<std::tuple<TimeUnit>, std::tuple<>>(base)
      {}

   };
   
   namespace TimeUnits
   {
      /// @brief Seconds, a TimeUnit global variable for creating Dimension objects
      inline TimeUnit Seconds("Seconds");

      /// @brief Minutes, a TimeUnit global variable for creating Dimension objects
      inline TimeUnit Minutes("Minutes");
   }

   inline TimeUnit* TimeUnit::GetPrimaryUnit() const { return &TimeUnits::Seconds; }
   

   /// @brief Static vector of all TimeUnits
   /// @details This vector of all TimeUnits is stored in each
   ///    TimeUnit object for use in simplification and other
   ///    helper functions
   /// @todo Consider other ways to handle this
   static std::vector<BaseUnit*> TimeUnitVector;

   /// @brief Add conversions to each TimeUnit
   /// @details Adds conversions to all time units and assigns the 
   ///    TimeUnitVector to each. This method also validates each 
   ///    TimeUnit as a conversion to the "primary" TimeUnit, Seconds,
   ///    and that Seconds has a conversion to each TimeUnit.
   ///    Note this must be called at the start of the program.
   /// @return bool indicating success
   inline bool initializeTimeUnits()
   {
      TimeUnits::Seconds.add_conversion(TimeUnits::Minutes, [](double val) {return val / 60.0; });
      TimeUnits::Minutes.add_conversion(TimeUnits::Seconds, [](double val) {return val * 60.0; });

      TimeUnitVector.push_back(&TimeUnits::Seconds);
      TimeUnitVector.push_back(&TimeUnits::Minutes);
      
      return BaseUnit::ValidateConversions(TimeUnitVector, TimeUnits::Seconds);
   }
}

#endif // DIMENSION_TIME_H