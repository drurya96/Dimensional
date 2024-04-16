
#ifndef DIMENSION_LENGTH_H
#define DIMENSION_LENGTH_H

#include <string>
#include "BaseDimension.h"
#include <unordered_map>
#include <functional>

namespace Dimension
{
   /// @brief Length unit, derived from BaseUnit
   template<typename ... is_inverse>
   class LengthUnit : public BaseUnit<is_inverse...>
   {
   public:
      /// @brief Constructor only giving name, primary constructor
      LengthUnit(std::string name) : BaseUnit<is_inverse...>(name) {}

      /// @brief Default constructor
      /// @details This default constructor is necessary
      ///    for some template metaprogramming on BaseUnit
      LengthUnit() : BaseUnit<is_inverse...>() {}

      /// @brief Default destructor
      ~LengthUnit() {}

      /// @brief Override for GetDimName
      std::string GetDimName() const override { return "Length"; }

      /// @brief Override for GetPrimaryUnit
      LengthUnit<>* GetPrimaryUnit() const override { return &LengthUnits::Meters; }

   private:

   };

   /// @brief Length dimension, derived from BaseDimension
   /// @details This dimension is a specialization using
   ///    Length as the unit.
   /// @todo Add some convenience methods to retrieve Length by name
   template<typename ... is_inverse>
   class Length : public BaseDimension<LengthUnit<is_inverse...>>
   {
   public:

      /// @brief Constructor using value and Length units
      /// @param[in] value The value to set
      /// @param[in] Length Pointer to the Length unit
      explicit Length(double value, LengthUnit<is_inverse...>* unit) 
         : BaseDimension<LengthUnit<is_inverse...>>(value, std::vector<BaseUnit<>*>{ static_cast<BaseUnit<>*>(unit) }, std::vector<BaseUnit<>*>{})
      {}

      /// @brief Cast operator from a BaseDimension
      Length(const BaseDimension<LengthUnit<is_inverse...>>& base) : BaseDimension<LengthUnit<is_inverse...>>(base)
      {}

   };

   namespace LengthUnits
   {
      /// @brief Meters, a LengthUnit global variable for creating Dimension objects
      inline LengthUnit<> Meters("Meters");

      /// @brief Feet, a LengthUnit global variable for creating Dimension objects
      inline LengthUnit<> Feet("Feet");
   }
   
   /// @brief Static vector of all LengthUnits
   /// @details This vector of all LengthUnits is stored in each
   ///    LengthUnit object for use in simplification and other
   ///    helper functions
   /// @todo Consider other ways to handle this
   static std::vector<BaseUnit<>*> LengthUnitVector;

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

      return BaseUnit<>::ValidateConversions(LengthUnitVector, LengthUnits::Meters);
   }

   /// @brief Simplify LengthUnit types
   /// @details Given a set of types, simplify all LengthUnit types.
   ///    This means for each pair of LengthUnits (one templated on Inverse
   ///    and the other not), both will be removed from the type list,
   ///    and the value will be adjusted accordingly
   /// @tparam Ts The typelist to simplify. Note in the current implementation
   ///    this list can contain any BaseUnits, not just LengthUnits.
   /// @todo Attach this to AllSimplifier directly
   template <typename... Ts>
   struct LengthUnitSimplifier : SimplifierInterface<Ts...> {

      static constexpr size_t LengthCount = count_type<LengthUnit<>, Ts...>();
      static constexpr size_t InverseLengthCount = count_type<LengthUnit<Inverse>, Ts...>();

      using type = std::conditional_t<(LengthCount > InverseLengthCount),
         typename Repeat<LengthCount - InverseLengthCount, LengthUnit<>>::type,
         std::conditional_t<(LengthCount < InverseLengthCount),
         typename Repeat<InverseLengthCount - LengthCount, LengthUnit<Inverse>>::type,
         std::tuple<>>>;

   };
}

#endif // DIMENSION_LENGTH_H