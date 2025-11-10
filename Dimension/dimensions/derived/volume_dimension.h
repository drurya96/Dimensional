#ifndef STATIC_DIMENSION_VOLUME_H
#define STATIC_DIMENSION_VOLUME_H

#include "Dimension_Impl/DerivedDimensions/volume_dimension_Impl.h"

#include "Dimension_Core/internal_temp/units/new_unit_stuff.h"
#include "Dimension_Core/internal_temp/units/factor.h"
#include "Dimension_Core/internal_temp/symbol/symbols.h"

namespace dimension
{  // Helper fundamental units used only to compose derived units
  struct fluid_ounce_length{};

  template<> struct unit_traits<fluid_ounce_length>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "FluidOuncelength";
     static constexpr std::string_view abbr   = "FluidOuncelength";
     static constexpr std::string_view dimName= "volume";
     static constexpr int id = 0;
  };

template<> struct Conversion<fluid_ounce_length, meters>
{
  using scale = factor_t<
    std::ratio<1, 1>,
    std::tuple<ratio_exponent<std::ratio<127, 1>, 1, 1>, ratio_exponent<std::ratio<2, 1>, -16, 3>, ratio_exponent<std::ratio<5, 1>, -4, 1>, ratio_exponent<std::ratio<3, 1>, 1, 3>, ratio_exponent<std::ratio<7, 1>, 1, 3>, ratio_exponent<std::ratio<11, 1>, 1, 3>>>;
};

  struct cup_length{};

  template<> struct unit_traits<cup_length>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Cuplength";
     static constexpr std::string_view abbr   = "Cuplength";
     static constexpr std::string_view dimName= "volume";
     static constexpr int id = 0;
  };

template<> struct Conversion<cup_length, meters>
{
  using scale = factor_t<
    std::ratio<1, 1>,
    std::tuple<ratio_exponent<std::ratio<127, 1>, 1, 1>, ratio_exponent<std::ratio<2, 1>, -13, 3>, ratio_exponent<std::ratio<5, 1>, -4, 1>, ratio_exponent<std::ratio<3, 1>, 1, 3>, ratio_exponent<std::ratio<7, 1>, 1, 3>, ratio_exponent<std::ratio<11, 1>, 1, 3>>>;
};

  struct pint_length{};

  template<> struct unit_traits<pint_length>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Pintlength";
     static constexpr std::string_view abbr   = "Pintlength";
     static constexpr std::string_view dimName= "volume";
     static constexpr int id = 0;
  };

template<> struct Conversion<pint_length, meters>
{
  using scale = factor_t<
    std::ratio<1, 1>,
    std::tuple<ratio_exponent<std::ratio<127, 1>, 1, 1>, ratio_exponent<std::ratio<2, 1>, -4, 1>, ratio_exponent<std::ratio<5, 1>, -4, 1>, ratio_exponent<std::ratio<3, 1>, 1, 3>, ratio_exponent<std::ratio<7, 1>, 1, 3>, ratio_exponent<std::ratio<11, 1>, 1, 3>>>;
};

  struct quart_length{};

  template<> struct unit_traits<quart_length>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Quartlength";
     static constexpr std::string_view abbr   = "Quartlength";
     static constexpr std::string_view dimName= "volume";
     static constexpr int id = 0;
  };

template<> struct Conversion<quart_length, meters>
{
  using scale = factor_t<
    std::ratio<1, 1>,
    std::tuple<ratio_exponent<std::ratio<127, 1>, 1, 1>, ratio_exponent<std::ratio<2, 1>, -11, 3>, ratio_exponent<std::ratio<5, 1>, -4, 1>, ratio_exponent<std::ratio<3, 1>, 1, 3>, ratio_exponent<std::ratio<7, 1>, 1, 3>, ratio_exponent<std::ratio<11, 1>, 1, 3>>>;
};

  struct gallon_length{};

  template<> struct unit_traits<gallon_length>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Gallonlength";
     static constexpr std::string_view abbr   = "Gallonlength";
     static constexpr std::string_view dimName= "volume";
     static constexpr int id = 0;
  };

template<> struct Conversion<gallon_length, meters>
{
  using scale = factor_t<
    std::ratio<1, 1>,
    std::tuple<ratio_exponent<std::ratio<127, 1>, 1, 1>, ratio_exponent<std::ratio<2, 1>, -3, 1>, ratio_exponent<std::ratio<5, 1>, -4, 1>, ratio_exponent<std::ratio<3, 1>, 1, 3>, ratio_exponent<std::ratio<7, 1>, 1, 3>, ratio_exponent<std::ratio<11, 1>, 1, 3>>>;
};

  struct teaspoon_length{};

  template<> struct unit_traits<teaspoon_length>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Teaspoonlength";
     static constexpr std::string_view abbr   = "Teaspoonlength";
     static constexpr std::string_view dimName= "volume";
     static constexpr int id = 0;
  };

template<> struct Conversion<teaspoon_length, meters>
{
  using scale = factor_t<
    std::ratio<1, 1>,
    std::tuple<ratio_exponent<std::ratio<127, 1>, 1, 1>, ratio_exponent<std::ratio<2, 1>, -17, 3>, ratio_exponent<std::ratio<5, 1>, -4, 1>, ratio_exponent<std::ratio<7, 1>, 1, 3>, ratio_exponent<std::ratio<11, 1>, 1, 3>>>;
};

  struct tablespoon_length{};

  template<> struct unit_traits<tablespoon_length>
  {
     using dimension = lengthType;
     static constexpr std::string_view name   = "Tablespoonlength";
     static constexpr std::string_view abbr   = "Tablespoonlength";
     static constexpr std::string_view dimName= "volume";
     static constexpr int id = 0;
  };

template<> struct Conversion<tablespoon_length, meters>
{
  using scale = factor_t<
    std::ratio<1, 1>,
    std::tuple<ratio_exponent<std::ratio<127, 1>, 1, 1>, ratio_exponent<std::ratio<2, 1>, -17, 3>, ratio_exponent<std::ratio<5, 1>, -4, 1>, ratio_exponent<std::ratio<3, 1>, 1, 3>, ratio_exponent<std::ratio<7, 1>, 1, 3>, ratio_exponent<std::ratio<11, 1>, 1, 3>>>;
};

  struct liters
  {
     using units = std::tuple<unit_exponent<deci_meters, 3, 1>>;
  };

  template<> struct unit_traits<liters>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<deci_meters, 3, 1>>;
  };
  struct milli_liters
  {
     using units = std::tuple<unit_exponent<centi_meters, 3, 1>>;
  };

  template<> struct unit_traits<milli_liters>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<centi_meters, 3, 1>>;
  };
  struct fluid_ounces
  {
     using units = std::tuple<unit_exponent<fluid_ounce_length, 3, 1>>;
  };

  template<> struct unit_traits<fluid_ounces>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<fluid_ounce_length, 3, 1>>;
  };
  struct cups
  {
     using units = std::tuple<unit_exponent<cup_length, 3, 1>>;
  };

  template<> struct unit_traits<cups>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<cup_length, 3, 1>>;
  };
  struct pints
  {
     using units = std::tuple<unit_exponent<pint_length, 3, 1>>;
  };

  template<> struct unit_traits<pints>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<pint_length, 3, 1>>;
  };
  struct quarts
  {
     using units = std::tuple<unit_exponent<quart_length, 3, 1>>;
  };

  template<> struct unit_traits<quarts>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<quart_length, 3, 1>>;
  };
  struct gallons
  {
     using units = std::tuple<unit_exponent<gallon_length, 3, 1>>;
  };

  template<> struct unit_traits<gallons>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<gallon_length, 3, 1>>;
  };
  struct teaspoons
  {
     using units = std::tuple<unit_exponent<teaspoon_length, 3, 1>>;
  };

  template<> struct unit_traits<teaspoons>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<teaspoon_length, 3, 1>>;
  };
  struct tablespoons
  {
     using units = std::tuple<unit_exponent<tablespoon_length, 3, 1>>;
  };

  template<> struct unit_traits<tablespoons>
  {
     // If/when you add names/abbr to JSON for derived units, emit here.
     using units = std::tuple<unit_exponent<tablespoon_length, 3, 1>>;
  };
}


#endif // STATIC_DIMENSION_VOLUME_H