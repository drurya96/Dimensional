#ifndef DIMENSION_BASE_H
#define DIMENSION_BASE_H

#include <vector>
#include <type_traits>
#include <tuple>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "DimensionUtilities.h"

namespace Dimension
{
   template<typename Unit, typename DerivedFactory>
   class UnitFactory;

   /// @brief A base class representing a unit
   /// @details This abstract class represents a Unit,
   ///    such as Meters, Seconds, Grams, etc.
   /// @tparam Unit The derived unit, using CRTP
   template<typename Unit>
   class BaseUnit
   {
   public:
      /// @brief Constructor setting name
      BaseUnit(const std::string& name) : unitName(name), conversions(InitializeMap(name, Unit::GetConversionMap()))
      {}


      /// @brief Pure virtual destructor
      virtual ~BaseUnit() = 0;

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
      virtual BaseUnit<Unit>* GetPrimaryUnit() const = 0;

      /// @brief Get the conversion functor from this unit to the provided unit
      /// @param[in] unit The unit to retrieve a conversion to
      /// @return The conversion functor
      /// @todo Validate successful conversion retrieval
      const std::function<double(double)>& getConversion(const BaseUnit<Unit>& unit) const
      {
         return conversions.at(unit.GetUnitName());
      }

      /// @brief Validate all conversions
      /// @details This function should be called as part of the initialization step of each
      ///    derived Unit type. It should be given the vector of all Units for the 
      ///    Unit type at hand, as well as the Primary Unit of that Dimension
      /// @param[in] UnitVector The vector of all Units within this dimension
      /// @param[in] PrimaryUnit The "primary" unit for this dimension.
      ///    Every unit must have a conversion to and from this unit. While other conversions
      ///    are allowed, these conversions are mandatory.
      /// @return A bool indicating successful validation
      /// @todo Update this since the vectors aren't used anymore
      static bool ValidateConversions(const std::vector<BaseUnit<Unit>*>& UnitVector, const BaseUnit<Unit>& PrimaryUnit)
      {
         return std::all_of(UnitVector.begin(), UnitVector.end(), [&](BaseUnit<Unit>* unit)
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
      ///    Conversions are inverted if isNumerator is false.
      /// @tparam isNumerator Bool indicating if the operation is a numerator.
      ///    Denominator operations are inverted relative to numerator operations
      /// @param[in] input The value to convert
      /// @param[in] toUnit Pointer to the unit object to convert to
      /// @return The converted value as a double
      template<bool isNumerator>
      double ConvertValue(const double input, const BaseUnit<Unit>* toUnit) const
      {
         if (conversions.find(toUnit->GetUnitName()) != conversions.end())
         {
            if constexpr (isNumerator) {
               return getConversion(*toUnit)(input);
            }
            else {
               return toUnit->getConversion(*this)(input);
            }
         }
         else
         {
            if constexpr (isNumerator)
            {
               return GetPrimaryUnit()->getConversion(*toUnit)(
                  this->getConversion(*GetPrimaryUnit())(input)
                  );
            }
            else
            {
               return toUnit->getConversion(*GetPrimaryUnit())(
                  GetPrimaryUnit()->getConversion(*this)(input)
                  );
            }
         }
      }
      
   private:
      /// @brief The name of the unit
      std::string unitName = "";

      /// @brief A map of unit names to conversion functors
      /// @todo Revisit keying on unit objects rather than strings
      std::unordered_map<std::string, std::function<double(double)>>& conversions;

      std::unordered_map<std::string, std::function<double(double)>>& InitializeMap(const std::string& name, ConversionMap& map) const
      {
         auto it = map.find(GetUnitName());
         if (it != map.end()) {
            return it->second;
         }
         else
         {
            // This should throw some kind of error
            // Can also do error handling here to ensure conversion to primary exists
            // Further, if this is the primary, can do checks to ensure conversions to other units
            static std::unordered_map<std::string, std::function<double(double)>> emptyMap;
            return emptyMap;
         }
      }
   };

   /// @brief Destructor implementation
   template<typename Unit>
   BaseUnit<Unit>::~BaseUnit() {}

   /// @brief Base factory class to produce unit objects
   /// @tparam Unit The derived unit class to instantiate
   /// @tparam DerivedFactory Derived class for CRTP
   template<typename Unit, typename DerivedFactory>
   class UnitFactory {
   public:
      
      /// @brief Return an instance of Unit with the given name
      /// @details If a Unit with the given name already exists, return it.
      ///    Otherwise, create a new instance and add it to the static
      ///    instances map. Pass the initialize flag along when creating the
      ///    new instance to determine whether the new instance should be initialized.
      ///    Creating an unitialized Unit is useful to avoid circular dependencies
      ///    between Unit instantiations.
      /// @param[in] name The name of the Unit to retrieve or create
      /// @param[in] initialize Flag indicating whether newly created instances
      ///    should be initialized.
      /// @return An instance of Unit corresponding to name
      static Unit& GetInstance(const std::string& name, bool initialize = true) {
         static std::unordered_map<std::string, std::unique_ptr<Unit>> instances;
         auto it = instances.find(name);
         
         if (it == instances.end())
         {
            Unit instance = CreateInstance(name);
            instances[name] = std::make_unique<Unit>(instance);
         }

         return *instances[name];
      }
   private:

      /// @brief Create a new instance of Unit
      /// @param[in] name The name of the Unit to create
      /// @return The instantiated unit
      static Unit CreateInstance(const std::string& name)
      {
         return Unit(name);
      }
   };

   /// @brief A generic Dimension class
   /// @details This class represents a Dimension,
   ///    such as Length, Time, Speed, etc.
   ///    Its templates are used to set the dimensions of the numerator
   ///    and denominator, respectively. For example, a BaseDimension
   ///    may be templated on a tuple<LengthUnit> and a tuple<TimeUnit>.
   ///    This would still be a BaseDimension, but can be treated as "Speed".
   ///    This library may be used with BaseDimension alone, but Dimensions
   ///    may also be derived from BaseDimension for more readible code,
   ///    for example the Length, Time, and Speed classes provided.
   /// @tparam NumTuple A tuple of BaseUnits describing the dimension's numerator.
   /// @tparam DenTuple A tuple of BaseUnits describing the dimension's denominator.
   template<typename NumTuple, typename DenTuple>
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
         numList(std::move(dimension.numList)),
         denList(std::move(dimension.denList))
      {
      }

      /// @brief Constructor explicitly given all values
      /// @details A constructor given all needed information.
      ///    This constructor should typically be used for creating new objects.
      /// @param[in] newValue The value to set
      /// @param[in] newNumList A tuple of BaseUnits to set as numerators
      /// @param[in] newDenList A tuple of BaseUnits to set as denominators
      BaseDimension(double newValue, NumTuple newNumList, DenTuple newDenList) :
         value(newValue),
         numList(newNumList),
         denList(newDenList)
      {
      }

      // TODO: Consider copy operator

      /// @brief Tuple of units corresponding to the Dimension numerator
      NumTuple numList;

      /// @brief Tuple of units corresponding to the Dimension denominator
      DenTuple denList;

      // TODO: UPDATE THIS DOXY
      /// @brief Return the internal value as a double in terms of the provided units
      /// @details Return the internal value after converting to the provided units.
      /// @param[in] i_numList tuple of BaseUnit pointers to convert to for the numerator
      /// @param[in] i_denList tuple of BaseUnit pointers to convert to for the denominator
      /// @return A double representing the value in terms of the given units
      template<typename ... NumTupleTypes, typename ... DenTupleTypes>
      double GetVal(const std::tuple<NumTupleTypes...>& i_numList, const std::tuple<DenTupleTypes...>& i_denList) const
      {
         double result = value;

         if (numList == i_numList && denList == i_denList) { return value; }

         GetConvertedValue(numList, i_numList, result, ConvertFunctor<true>());
         GetConvertedValue(denList, i_denList, result, ConvertFunctor<false>());

         return result;
      }

      /// @brief += operator overload for another Dimension
      BaseDimension<NumTuple, DenTuple>& operator+=(const BaseDimension<NumTuple, DenTuple>& rhs)
      {
         value += rhs.GetVal(numList, denList);
         return *this;
      }

      /// @brief -= operator overload for another Dimension
      BaseDimension<NumTuple, DenTuple>& operator-=(const BaseDimension<NumTuple, DenTuple>& rhs)
      {
         value -= rhs.GetVal(numList, denList);
         return *this;
      }

      /// @brief *= operator overload for a scalar
      BaseDimension<NumTuple, DenTuple>& operator*=(double rhs)
      {
         value *= rhs;
         return *this;
      }

      /// @brief /= operator overload for a scalar
      BaseDimension<NumTuple, DenTuple>& operator/=(double rhs)
      {
         value /= rhs;
         return *this;
      }

      // The following operators are explicitly deleted
      BaseDimension<NumTuple, DenTuple>& operator*=(const BaseDimension<NumTuple, DenTuple>& rhs) = delete; // Multiplication results in a different type
      BaseDimension<NumTuple, DenTuple>& operator/=(const BaseDimension<NumTuple, DenTuple>& rhs) = delete; // Division results in a different type
      BaseDimension<NumTuple, DenTuple>& operator+=(double rhs) = delete; // Addition cannot be performed between a Dimension and a scalar
      BaseDimension<NumTuple, DenTuple>& operator-=(double rhs) = delete; // Subtraction cannot be performed between a Dimension and a scalar

      // Comparison Operators
      bool operator>(const BaseDimension<NumTuple, DenTuple>& rhs) const { return value > rhs.GetVal(numList, denList); }
      bool operator<(const BaseDimension<NumTuple, DenTuple>& rhs) const { return value < rhs.GetVal(numList, denList); }
      bool operator>=(const BaseDimension<NumTuple, DenTuple>& rhs) const { return value >= rhs.GetVal(numList, denList); }
      bool operator<=(const BaseDimension<NumTuple, DenTuple>& rhs) const { return value <= rhs.GetVal(numList, denList); }
      bool operator==(const BaseDimension<NumTuple, DenTuple>& rhs) const { return value == rhs.GetVal(numList, denList); } // I need to add a small tolerance to this
      bool operator!=(const BaseDimension<NumTuple, DenTuple>& rhs) const { return !(*this == rhs); }

      // TODO: Define a NearlyEqual method with custom tolerance

   protected:

   private:
      /// @brief The value of the given object
      double value;

      /// @brief Private getter to retrieve the raw value
      /// @return The raw value
      double GetRawValue() const { return value; }

      // Declare operator overloads as friends of this class
      // This is to directly access the raw value for efficiency

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      friend auto operator/(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);

      template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
      friend auto operator*(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator*(const BaseDimension<NumTuple, DenTuple>& obj, double scalar);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator*(double scalar, const BaseDimension<NumTuple, DenTuple>& obj);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator/(const BaseDimension<NumTuple, DenTuple>& obj, double scalar);

      template<typename NumTuple, typename DenTuple>
      friend auto operator/(double scalar, const BaseDimension<NumTuple, DenTuple>& obj)->BaseDimension<DenTuple, NumTuple>;

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator+(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2);

      template<typename NumTuple, typename DenTuple>
      friend BaseDimension<NumTuple, DenTuple> operator-(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2);
   };

   /// @brief Division operator for two Dimensions
   /// @tparam NumTuple1 Tuple of numerator units of obj1
   /// @tparam DenTuple1 Tuple of denominator units of obj1
   /// @tparam NumTuple2 Tuple of numerator units of obj2
   /// @tparam DenTuple2 Tuple of denominator units of obj2
   /// @param[in] obj1 The numerator BaseDimension object
   /// @param[in] obj2 The denominator BaseDimension object
   /// @return A base dimension object templated on the numerator types and
   ///    the denominator types, then simplified.
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   auto operator/(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      using NumResultTuple = decltype(std::tuple_cat(std::declval<NumTuple1>(), std::declval<DenTuple2>()));
      using DenResultTuple = decltype(std::tuple_cat(std::declval<DenTuple1>(), std::declval<NumTuple2>()));

      using ResultType = BaseDimension<NumResultTuple, DenResultTuple>;
      auto result = ResultType(
         obj1.GetRawValue() / obj2.GetRawValue(),
         std::tuple_cat(obj1.numList, obj2.denList),
         std::tuple_cat(obj1.denList, obj2.numList));
      return SimplifyBaseDimension(result);
   }

   /// @brief Multiplication operator for two Dimensions
   /// @tparam NumTuple1 Tuple of numerator units of obj1
   /// @tparam DenTuple1 Tuple of denominator units of obj1
   /// @tparam NumTuple2 Tuple of numerator units of obj2
   /// @tparam DenTuple2 Tuple of denominator units of obj2
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object templated on the types of both
   ///    input objects, then simplified.
   template<typename NumTuple1, typename DenTuple1, typename NumTuple2, typename DenTuple2>
   auto operator*(const BaseDimension<NumTuple1, DenTuple1>& obj1, const BaseDimension<NumTuple2, DenTuple2>& obj2)
   {
      using NumResultTuple = decltype(std::tuple_cat(std::declval<NumTuple1>(), std::declval<NumTuple2>()));
      using DenResultTuple = decltype(std::tuple_cat(std::declval<DenTuple1>(), std::declval<DenTuple2>()));

      using ResultType = BaseDimension<NumResultTuple, DenResultTuple>;
      auto result = ResultType(
         obj1.GetRawValue() * obj2.GetRawValue(),
         std::tuple_cat(obj1.numList, obj2.numList),
         std::tuple_cat(obj1.denList, obj2.denList)
      );
      return SimplifyBaseDimension(result);
   }

   // Scalar Math

   /// @brief Multiplication operator for a Dimension and scalar
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj The BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @return A BaseDimension object of type matching obj, with value multiplied by scalar
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator*(const BaseDimension<NumTuple, DenTuple>& obj, double scalar)
   {
      return BaseDimension<NumTuple, DenTuple>(obj.GetRawValue() * scalar, obj.numList, obj.denList);
   }

   /// @brief Multiplication operator for a scalar and Dimension
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] scalar The scalar value as a double
   /// @param[in] obj The BaseDimension object
   /// @return A BaseDimension object of type matching obj, with value multiplied by scalar
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator*(double scalar, const BaseDimension<NumTuple, DenTuple>& obj)
   {
      return obj * scalar;
   }

   /// @brief Division operator for a Dimension and scalar
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj The BaseDimension object
   /// @param[in] scalar The scalar value as a double
   /// @return A BaseDimension object of type matching obj, with value divided by scalar
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator/(const BaseDimension<NumTuple, DenTuple>& obj, double scalar)
   {
      return BaseDimension<NumTuple, DenTuple>(obj.GetRawValue() / scalar, obj.numList, obj.denList);
   }

   /// @brief Division operator for a scalar and Dimension
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] scalar The scalar value as a double
   /// @param[in] obj The BaseDimension object
   /// @return A BaseDimension object with Unit parameters inverted relative
   ///    to obj, and with scalar divided by obj value as the new value
   template<typename NumTuple, typename DenTuple>
   auto operator/(double scalar, const BaseDimension<NumTuple, DenTuple>& obj) -> BaseDimension<DenTuple, NumTuple>
   {
      return BaseDimension<DenTuple, NumTuple>(scalar / obj.GetRawValue(), obj.denList, obj.numList);
   }

   /// @brief Addition operator for two Dimensions
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the values of obj1 and obj2 added, after converting
   ///    obj2 to the same units as obj1
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator+(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2)
   {
      return BaseDimension<NumTuple, DenTuple>(obj1.GetRawValue() + obj2.GetVal(obj1.numList, obj1.denList), obj1.numList, obj1.denList);
   }

   /// @brief Subtraction operator for two Dimensions
   /// @tparam NumTuple Tuple of units in the numerator
   /// @tparam DenTuple Tuple of units in the denominator
   /// @param[in] obj1 The first BaseDimension object
   /// @param[in] obj2 The second BaseDimension object
   /// @return A base dimension object of type matching the inputs.
   ///    The value is the difference of values of obj1 and obj2, after 
   ///    converting obj2 to the same units as obj1
   template<typename NumTuple, typename DenTuple>
   BaseDimension<NumTuple, DenTuple> operator-(const BaseDimension<NumTuple, DenTuple>& obj1, const BaseDimension<NumTuple, DenTuple>& obj2)
   {
      return BaseDimension<NumTuple, DenTuple>(obj1.GetRawValue() - obj2.GetVal(obj1.numList, obj1.denList), obj1.numList, obj1.denList);
   }

}

// TODO: Implement SI conversions
// Keep this attempt as a starting point

/*
// Macro for SI prefixes

// Macro definition to create a prefixed factory function
#define SI_PREFIX(baseName, UnitType, Prefix) \
   inline static UnitType& Prefix##baseName() { \
      static std::string fullName = std::string(#Prefix) + #baseName; \
      UnitType& unit UnitTypeFactory::GetInstance(fullName); \
      return unit; \
    }

#define ALL_SI_PREFIXES(baseName, UnitType) \
SI_PREFIX(baseName, UnitType, Pico); \
SI_PREFIX(baseName, UnitType, Nano); \
SI_PREFIX(baseName, UnitType, Micro); \
SI_PREFIX(baseName, UnitType, Milli); \
SI_PREFIX(baseName, UnitType, Centi); \
SI_PREFIX(baseName, UnitType, Deci); \
SI_PREFIX(baseName, UnitType, Deca); \
SI_PREFIX(baseName, UnitType, Hepta); \
SI_PREFIX(baseName, UnitType, Kilo); \
SI_PREFIX(baseName, UnitType, Mega); \
SI_PREFIX(baseName, UnitType, Giga); \
SI_PREFIX(baseName, UnitType, Tera);

*/

#endif // DIMENSION_BASE_H