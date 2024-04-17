#ifndef DIMENSION_BASE_H
#define DIMENSION_BASE_H

#include <vector>
#include <type_traits>
#include <tuple>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include <typeindex>

#include "DimensionUtilities.h"
#include "Simplification.h"

namespace Dimension
{
   /// @brief A base class representing a unit
   /// @details This abstract class represents a Unit,
   ///    such as Meters, Seconds, Grams, etc.
   ///    Its variadic template is used to determine whether
   ///    the unit is Inverse (i.e. in the denominator) or not.
   ///    An instance of this class with the template signature
   ///    <> indicates the unit is in the numerator, while an instance
   ///    templated on <Inverse> indicates the unit is in the denominator
   /// @tparam is_inverse This should be either empty or Inverse
   /// @todo Use CRTP to reduce code duplication and move as much
   ///    as possible into the BaseUnit
   /// @todo Attempt to implement the initialization function here
   template<typename ... is_inverse>
   class BaseUnit
   {
   public:
      /// @brief Default constructor
      /// @details This default constructor is necessary
      ///    for some template metaprogramming below
      BaseUnit() : unitName(""){}

      /// @brief Constructor setting name
      BaseUnit(std::string name) : unitName(name) {}

      /// @brief Pure virtual destructor
      virtual ~BaseUnit() = 0;

      /// @brief Getter for the Dimension name
      /// @details Get the name of the dimension this unit
      ///    belongs to. For example, a Meter type might
      ///    return "Length".
      /// @return A string indicating the dimension name
      /// @todo Consider whether this should return some
      ///    the global variable rather than string
      virtual std::string GetDimName() const
      {
         return "Base";
      };

      /// @brief Getter for the Unit name
      /// @return A string indicating the unit name
      /// @todo This is primarily used to determine which conversion to use.
      ///    If conversions change to use the existing global variables, this could be removed
      virtual std::string GetUnitName() const
      {
         return unitName;
      }

      /// @brief Getter for the "primary" unit within this units dimension
      /// @details Get a pointer to the "primary" unit within this
      ///    units dimension. For example, this functional call for the unit
      ///    Feet might return a pointer to Meters, if Meters is the primary unit.
      ///    More on the "primary" unit below
      /// @return A pointer to the "primary" unit
      /// @todo Prefer returning a reference, need to evaluate fallout
      virtual BaseUnit<>* GetPrimaryUnit() const = 0;

      /// @brief Get the conversion functor from this unit to the provided unit
      /// @param[in] unit The unit to retrieve a conversion to
      /// @return The conversion functor
      /// @todo Validate successful conversion retrieval
      const std::function<double(double)>& getConversion(const BaseUnit& unit) const
      {
         return conversions.at(unit.GetUnitName());
      }

      /// @brief Add a conversion to the map of conversions
      /// @param[in] toUnit The unit to convert to
      /// @param[in] conversion The conversion lamda to convert from this unit to toUnit
      /// @return A bool indicating success of adding the conversion
      /// @todo Check for success of adding
      bool add_conversion(const BaseUnit& toUnit, std::function<double(double)> conversion)
      {
         conversions[toUnit.GetUnitName()] = conversion;
         return true;
      };

      /// @brief Validate all conversions
      /// @details This function should be called as part of the initialization step of each
      ///    derived Unit type. It should be given the vector of all Units for the 
      ///    Unit type at hand, as well as the Primary Unit of that Dimension
      /// @param[in] UnitVector The vector of all Units within this dimension
      /// @param[in] PrimaryUnit The "primary" unit for this dimension.
      ///    Every unit must have a conversion to and from this unit. While other conversions
      ///    are allowed, these conversions are mandatory.
      /// @return A bool indicating successful validation
      static bool ValidateConversions(const std::vector<BaseUnit<>*>& UnitVector, const BaseUnit<>& PrimaryUnit)
      {
         return std::all_of(UnitVector.begin(), UnitVector.end(), [&](BaseUnit<>* unit)
            {
            auto findit = unit->conversions.find(PrimaryUnit.GetUnitName());
            auto findit2 = PrimaryUnit.conversions.find(unit->GetUnitName());
            return (findit != unit->conversions.end() || unit == &PrimaryUnit) &&
               (findit2 != PrimaryUnit.conversions.end() || unit == &PrimaryUnit);
            });
      }

      /// @brief Convert the input value from this unit to the given unit
      /// @details This will use the direct conversion if one is provided,
      ///    otherwise it will convert to the primary unit, then to the
      ///    given unit.
      /// @param[in] input The value to convert
      /// @param[in] NewUnit The unit to convert to
      /// @return The converted value
      double ConvertValueNumerator(const double input, const BaseUnit& NewUnit) const
      {
         double result;
         if (conversions.find(NewUnit.GetUnitName()) != conversions.end())
         {
            result = getConversion(NewUnit)(input);
         }
         else
         {
            result = getConversion(*GetPrimaryUnit())(input);
            result = GetPrimaryUnit()->getConversion(NewUnit)(result);
         }
         return result;
      }

      /// @brief Convert the input value from this unit to the given unit
      /// @details This will use the direct conversion if one is provided,
      ///    otherwise it will convert to the primary unit, then to the
      ///    given unit. This conversion is inverted relative to
      ///    ConvertValueNumerator since reciprocals must be used.
      /// @param[in] input The value to convert
      /// @param[in] NewUnit The unit to convert to
      /// @return The converted value
      double ConvertValueDenominator(const double input, const BaseUnit& NewUnit) const
      {
         double result;
         if (conversions.find(NewUnit.GetUnitName()) != conversions.end())
         {
            result = NewUnit.getConversion(*this)(input);
         }
         else
         {
            result = NewUnit.getConversion(*GetPrimaryUnit())(input);
            result = GetPrimaryUnit()->getConversion(*this)(result);
         }
         return result;
      }

   private:
      /// @brief The name of the unit
      std::string unitName = "";

      /// @brief A map of unit names to conversion functors
      std::unordered_map<std::string, std::function<double(double)>> conversions;
   };

   /// @brief Destructor implementation
   template<typename ... is_inverse>
   inline BaseUnit<is_inverse...>::~BaseUnit() {}

   /// @brief A generic Dimension class
   /// @details This class represents a Dimension,
   ///    such as Length, Time, Speed, etc.
   ///    Its variadic template is used to determine which BaseUnit
   ///    types make up this dimension. For example, a BaseDimension
   ///    may be templated on a LengthUnit<> and a TimeUnit<Inverse>.
   ///    This would still be a BaseUnit, but can be treated as "Speed".
   ///    This library may be used with BaseDimension alone, but Dimensions
   ///    may also be derived from BaseDimension for more readible code,
   ///    for example the Length, Time, and Speed classes provided.
   /// @tparam UnitType The BaseUnit types defining this dimension.
   template<typename ... UnitType>
   class BaseDimension
   {
   public:
      /// @brief Default constructor
      BaseDimension() : 
         value(0.0)
      {
      }

      /// @brief Constructor using existing dimension
      /// @details This constructor creates a new object matching an existing dimension
      /// @tparam T Type of given Dimension
      /// @param[in] dimension A Dimension object to copy
      /// @return A new Dimension object of type T
      template<typename T>
      explicit BaseDimension(const T& dimension) :
         value(dimension.value),
         numList(dimension.numList.begin(), dimension.numList.end()),
         denList(dimension.denList.begin(), dimension.denList.end())
      {
      }

      /// @brief Constructor explicitly given all values
      /// @details A constructor given all needed information.
      ///    This constructor should typically be used for creating new objects.
      /// @param[in] newValue The value to set
      /// @param[in] newNumList A vector of BaseUnits to set as numerators
      /// @param[in] newDenList A vector of BaseUnits to set as denominators
      ///    Note both of these vectors should use the non-templated (<>) BaseUnit
      ///    Since the numerator/denominator is handled by the vectors.
      /// @todo Investigate using references of BaseUnits instead of pointers
      BaseDimension(double newValue, std::vector<BaseUnit<>*> newNumList, std::vector<BaseUnit<>*> newDenList) :
         value(newValue),
         numList(newNumList.begin(), newNumList.end()),
         denList(newDenList.begin(), newDenList.end())
      {
      }

      // TODO: Consider copy operator



      // These are pointers for simplicity for now, but may change.
      // Consider using a tuple for immutability
      // Consider use a raw array (not std::array) to allow storage of references
      // Note a raw array requires compile-time knowledge of size
      // While this is possible since the templates hold that information,
      //    it may be challenging.
      std::vector<BaseUnit<>*> numList;
      std::vector<BaseUnit<>*> denList;

      /// @brief Return the internal value as a double in terms of the provided units
      /// @details Return the internal value after converting to the provided units.
      /// @param[in] i_numList vector of BaseUnit pointers to convert to for the numerator
      /// @param[in] i_denList vector of BaseUnit pointers to convert to for the denominator
      /// @return A double representing the value in terms of the given units
      /// @todo Substantially improve efficiency, presumed bottleneck (not tested)
      /// @todo Refactor to move some functionality out of this method
      /// @todo Consider what should happen if the parameters are inappropriate for this object
      double GetVal(const std::vector<BaseUnit<>*>& i_numList, const std::vector<BaseUnit<>*>& i_denList) const
      {
         auto temp_InputNumList = i_numList;
         auto temp_InputDenList = i_denList;
         auto temp_MyNumList = numList;
         auto temp_MyDenList = denList;

         double result = value;


         for (auto it = temp_MyNumList.begin(); it != temp_MyNumList.end(); ) {
            auto duplicateIt = std::find(temp_InputNumList.begin(), temp_InputNumList.end(), *it);
            if (duplicateIt != temp_InputNumList.end()) {
               it = temp_MyNumList.erase(it);
               temp_InputNumList.erase(duplicateIt);
            }
            else {
               ++it;
            }
         }

         for (auto it = temp_MyDenList.begin(); it != temp_MyDenList.end(); ) {
            auto duplicateIt = std::find(temp_InputDenList.begin(), temp_InputDenList.end(), *it);
            if (duplicateIt != temp_InputDenList.end()) {
               it = temp_MyDenList.erase(it);
               temp_InputDenList.erase(duplicateIt);
            }
            else {
               ++it;
            }
         }

         // Handle Numerator
         if (temp_MyNumList.size() != temp_InputNumList.size()) {
            throw std::runtime_error("Vectors must have the same size.");
         }

         bool found = false;
         for (BaseUnit<>* myUnit : temp_MyNumList)
         {
            found = false;
            for (auto it = temp_InputNumList.begin(); it != temp_InputNumList.end();)
            {
               if (myUnit->GetPrimaryUnit() == (*it)->GetPrimaryUnit())
               {
                  result = myUnit->ConvertValueNumerator(result, *(*it));

                  temp_InputNumList.erase(it);
                  found = true;
                  break;
               }
               else { ++it; }
            }

            if (!found)
            {
               // Not found, raise an exception
            }
         }


         // Handle Denominator
         if (temp_MyDenList.size() != temp_InputDenList.size()) {
            throw std::runtime_error("Vectors must have the same size.");
         }

         for (BaseUnit<>* myUnit : temp_MyDenList)
         {
            found = false;
            for (auto it = temp_InputDenList.begin(); it != temp_InputDenList.end();)
            {
               if (myUnit->GetPrimaryUnit() == (*it)->GetPrimaryUnit())
               {
                  result = myUnit->ConvertValueDenominator(result, *(*it));

                  temp_InputDenList.erase(it);
                  found = true;
                  break;
               }
               else { ++it; }
            }

            if (!found)
            {
               // Not found, raise an exception
            }
         }
         return result;
      }
      
      /// @brief += operator overload for another Dimension
      BaseDimension<UnitType...>& operator+=(const BaseDimension<UnitType...>& rhs)
      {
         value += rhs.GetVal(numList, denList);
         return *this;
      }

      /// @brief -= operator overload for another Dimension
      BaseDimension<UnitType...>& operator-=(const BaseDimension<UnitType...>& rhs)
      {
         value -= rhs.GetVal(numList, denList);
         return *this;
      }

      /// @brief *= operator overload for a scalar
      BaseDimension<UnitType...>& operator*=(double rhs)
      {
         value *= rhs;
         return *this;
      }

      /// @brief /= operator overload for a scalar
      BaseDimension<UnitType...>& operator/=(double rhs)
      {
         value /= rhs;
         return *this;
      }

      // The following operators are explicitly deleted
      BaseDimension<UnitType...>& operator*=(const BaseDimension<UnitType...>& rhs) = delete; // Multiplication results in a different type
      BaseDimension<UnitType...>& operator/=(const BaseDimension<UnitType...>& rhs) = delete; // Division results in a different type
      BaseDimension<UnitType...>& operator+=(double rhs) = delete; // Addition cannot be performed between a Dimension and a scalar
      BaseDimension<UnitType...>& operator-=(double rhs) = delete; // Subtraction cannot be performed between a Dimension and a scalar

      // Comparison Operators
      bool operator>(const BaseDimension<UnitType...>& rhs) const { return value > rhs.GetVal(numList, denList); }
      bool operator<(const BaseDimension<UnitType...>& rhs) const { return value < rhs.GetVal(numList, denList); }
      bool operator>=(const BaseDimension<UnitType...>& rhs) const { return value >= rhs.GetVal(numList, denList); }
      bool operator<=(const BaseDimension<UnitType...>& rhs) const { return value <= rhs.GetVal(numList, denList); }
      bool operator==(const BaseDimension<UnitType...>& rhs) const { return value == rhs.GetVal(numList, denList); } // I need to add a small tolerance to this
      bool operator!=(const BaseDimension<UnitType...>& rhs) const { return !(*this == rhs); }

      // TODO: Define a NearlyEqual method with custom tolerance

      /// @brief Get the raw value field
      /// @todo This really shouldn't be used, need to investigate
      double GetRawValue() const { return value; }

   protected:

   private:
      /// @brief The value of the given object
      double value;

   };

   /// @brief Division operator for two Dimensions
   /// @tparam T_Classes1 The units of the numerator BaseDimension object
   /// @tparam T_Classes2 The units of the denominator BaseDimension object
   /// @param[in] obj1 The numerator BaseDimension object
   /// @param[in] obj2 The denominator BaseDimension object
   /// @return A base dimension object templated on the numerator types and
   ///    the denominator types, then simplified.
   template<typename... T_Classes1, typename... T_Classes2>
   auto operator/(const BaseDimension<T_Classes1...>& obj1, const BaseDimension<T_Classes2...>& obj2)
      -> decltype(SimplifyBaseDimension(std::declval<BaseDimension<T_Classes1..., typename InvertReturnType<T_Classes2>::type...>>()))
   {
      using ResultType = BaseDimension<T_Classes1..., typename InvertReturnType<T_Classes2>::type...>;
      auto result = ResultType(
         obj1.GetRawValue() / obj2.GetRawValue(),
         ConcatenateUnitVectors(obj1.numList, obj2.denList),
         ConcatenateUnitVectors(obj1.denList, obj2.numList));
      return SimplifyBaseDimension(result);
   }

   /// @brief Multiplication operator for two Dimensions
   /// @tparam T_Classes1 The units of the first BaseDimension object
   /// @tparam T_Classes2 The units of the second BaseDimension object
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object templated on the types of both
   ///    input objects, then simplified.
   template<typename ... T_Classes1, typename ... T_Classes2>
   auto operator*(const BaseDimension<T_Classes1...>& obj1, const BaseDimension<T_Classes2...>& obj2) 
      -> decltype(SimplifyBaseDimension(std::declval<BaseDimension < T_Classes1..., T_Classes2... >>()))
   {
      using ResultType = BaseDimension<T_Classes1..., T_Classes2...>;
      auto result = ResultType(
         obj1.GetRawValue() * obj2.GetRawValue(),
         ConcatenateUnitVectors(obj1.numList, obj2.numList),
         ConcatenateUnitVectors(obj1.denList, obj2.denList)
      );
      return SimplifyBaseDimension(result);
   }

   // Scalar Math

   /// @brief Multiplication operator for a Dimension and scalar
   /// @tparam Ts The units of the BaseDimension object
   /// @param[in] obj The BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @return A BaseDimension object of type matching obj, with value multiplied by scalar
   template<typename ... Ts>
   BaseDimension<Ts...> operator*(const BaseDimension<Ts...>& obj, double scalar)
   {
      return BaseDimension<Ts...>(obj.GetRawValue() * scalar, obj.numList, obj.denList);
   }

   /// @brief Multiplication operator for a scalar and Dimension
   /// @tparam Ts The units of the BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @param[in] obj The BaseDimension object
   /// @return A BaseDimension object of type matching obj, with value multiplied by scalar
   template<typename ... Ts>
   BaseDimension<Ts...> operator*(double scalar, const BaseDimension<Ts...>& obj)
   {
      return obj * scalar;
   }

   /// @brief Division operator for a Dimension and scalar
   /// @tparam Ts The units of the BaseDimension object
   /// @param[in] obj The BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @return A BaseDimension object of type matching obj, with value divided by scalar
   template<typename ... Ts>
   BaseDimension<Ts...> operator/(const BaseDimension<Ts...>& obj, double scalar)
   {
      return BaseDimension<Ts...>(obj.GetRawValue() / scalar, obj.numList, obj.denList);
   }

   /// @brief Division operator for a scalar and Dimension
   /// @tparam Ts The units of the BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @param[in] obj The BaseDimension object
   /// @return A BaseDimension object with Unit parameters inverted relative
   ///    to obj, and with scalar divided by obj value as the new value
   template<typename ... Ts>
   auto operator/(double scalar, const BaseDimension<Ts...>& obj) -> BaseDimension<typename InvertReturnType<Ts>::type...>
   {
      using ResultType = BaseDimension<typename InvertReturnType<Ts>::type...>;

      return ResultType(scalar / obj.GetRawValue(), obj.denList, obj.numList);
   }

   /// @brief Addition operator for two Dimensions
   /// @tparam Ts The units of both BaseDimension objects
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the values of obj1 and obj2 added, after converting
   ///    obj2 to the same units as obj1
   template<typename ... Ts>
   BaseDimension<Ts...> operator+(const BaseDimension<Ts...>& obj1, const BaseDimension<Ts...>& obj2)
   {
      return BaseDimension<Ts...>(obj1.GetRawValue() + obj2.GetVal(obj1.numList, obj1.denList), obj1.numList, obj1.denList);
   }

   /// @brief Subtraction operator for two Dimensions
   /// @tparam Ts The units of both BaseDimension objects
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the difference of values of obj1 and obj2, after 
   ///    converting obj2 to the same units as obj1
   template<typename ... Ts>
   BaseDimension<Ts...> operator-(const BaseDimension<Ts...>& obj1, const BaseDimension<Ts...>& obj2)
   {
      return BaseDimension<Ts...>(obj1.GetRawValue() - obj2.GetVal(obj1.numList, obj1.denList), obj1.numList, obj1.denList);
   }

}
#endif // DIMENSION_BASE_H