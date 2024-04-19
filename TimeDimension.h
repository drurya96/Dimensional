
#ifndef DIMENSION_TIME_H
#define DIMENSION_TIME_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

namespace Dimension
{
   /// @brief Time unit, derived from BaseUnit
   template<typename ... is_inverse>
   class TimeUnit : public BaseUnit<is_inverse...>
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
      TimeUnit<>* GetPrimaryUnit() const override { return &TimeUnits::Seconds; }

   private:

   };


   /// @brief Time dimension, derived from BaseDimension
   /// @details This dimension is a specialization using
   ///    Time as the unit.
   /// @todo Add some convenience methods to retrieve Time by name
   template<typename ... is_inverse>
   class Time : public BaseDimension<std::tuple<TimeUnit<>>, std::tuple<>>
   {
   public:

      /// @brief Constructor using value and time units
      /// @param[in] value The value to set
      /// @param[in] time Pointer to the time unit
      explicit Time(double value, TimeUnit<is_inverse...>* unit)
         : BaseDimension<std::tuple<TimeUnit<is_inverse...>>, std::tuple<>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(unit) }, std::vector<BaseUnit<>*>{})
      {}

      /// @brief Cast operator from a BaseDimension
      Time(const BaseDimension<std::tuple<TimeUnit<is_inverse...>>, std::tuple<>>& base) : BaseDimension<std::tuple<TimeUnit<is_inverse...>>, std::tuple<>>(base)
      {}

   };

   namespace TimeUnits
   {
      /// @brief Seconds, a TimeUnit global variable for creating Dimension objects
      inline TimeUnit<> Seconds("Seconds");

      /// @brief Minutes, a TimeUnit global variable for creating Dimension objects
      inline TimeUnit<> Minutes("Minutes");
   }
   
   /// @brief Static vector of all TimeUnits
   /// @details This vector of all TimeUnits is stored in each
   ///    TimeUnit object for use in simplification and other
   ///    helper functions
   /// @todo Consider other ways to handle this
   static std::vector<BaseUnit<>*> TimeUnitVector;

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
      
      return BaseUnit<>::ValidateConversions(TimeUnitVector, TimeUnits::Seconds);
   }

   /// @brief Simplify TimeUnit types
   /// @details Given a set of types, simplify all TimeUnit types.
   ///    This means for each pair of TimeUnits (one templated on Inverse
   ///    and the other not), both will be removed from the type list,
   ///    and the value will be adjusted accordingly
   /// @tparam Ts The typelist to simplify. Note in the current implementation
   ///    this list can contain any BaseUnits, not just TimeUnits.
   /// @todo Attach this to AllSimplifier directly
   template <typename NumTuple, typename DenTuple>
   struct TimeUnitSimplifier : SimplifierInterface<NumTuple, DenTuple> {

      static constexpr size_t TimeCount = count_type<TimeUnit<>, NumTuple>();
      static constexpr size_t InverseTimeCount = count_type<TimeUnit<>, DenTuple>();

      using type = std::conditional_t<(TimeCount > InverseTimeCount),
         typename Repeat<TimeCount - InverseTimeCount, TimeUnit<>>::type,
         std::conditional_t<(TimeCount < InverseTimeCount),
         typename Repeat<InverseTimeCount - TimeCount, TimeUnit<Inverse>>::type,
         std::tuple<>>>;
   };
}

#endif // DIMENSION_TIME_H