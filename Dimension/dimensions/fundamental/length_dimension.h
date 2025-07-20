#ifndef STATIC_DIMENSION_LENGTH_H
#define STATIC_DIMENSION_LENGTH_H

#include "Dimension_Impl/FundamentalDimensions/length_dimension_Impl.h"

namespace dimension
{
   struct meters : public lengthUnit<meters, "meters", "m"> {};
   struct feet : public lengthUnit<feet, "Feet", "ft"> {};
   struct inches : public lengthUnit<inches, "Inches", "in"> {};
   struct astronomical_units : public lengthUnit<astronomical_units, "Astronomical Units", "au"> {};
   struct data_miles : public lengthUnit<data_miles, "Data Miles", "DataMiles"> {};
   struct nautical_miles : public lengthUnit<nautical_miles, "Nautical Miles", "nmi"> {};
   struct miles : public lengthUnit<miles, "Miles", "mi"> {};
   struct fathoms : public lengthUnit<fathoms, "Fathoms", "ftm"> {};
   struct furlong : public lengthUnit<furlong, "Furlong", "fur"> {};
   struct yards : public lengthUnit<yards, "Yards", "yd"> {};
   struct us_survey_feet : public lengthUnit<us_survey_feet, "US Survey Feet", "ftUS"> {};

   template<> struct Conversion<meters, feet> { static constexpr PrecisionType slope = (1250.0 / 381.0); };
   template<> struct Conversion<meters, inches> { static constexpr PrecisionType slope = (15000.0 / 381.0); };
   template<> struct Conversion<meters, astronomical_units> { static constexpr PrecisionType slope = (1.0 / 149597870700.0); };
   template<> struct Conversion<meters, data_miles> { static constexpr PrecisionType slope = (5.0 / 9144.0); };
   template<> struct Conversion<meters, nautical_miles> { static constexpr PrecisionType slope = (1.0 / 1852.0); };
   template<> struct Conversion<meters, miles> { static constexpr PrecisionType slope = (125.0 / 201168.0); };
   template<> struct Conversion<meters, fathoms> { static constexpr PrecisionType slope = (3937.0 / 7200.0); };
   template<> struct Conversion<meters, furlong> { static constexpr PrecisionType slope = (3937.0 / 792000.0); };
   template<> struct Conversion<meters, yards> { static constexpr PrecisionType slope = (1250.0 / 1143.0); };
   template<> struct Conversion<meters, us_survey_feet> { static constexpr PrecisionType slope = (3937.0 / 1200.0); };

   ALL_SI_PREFIXES(meters, "m", lengthUnit);

}




#endif //STATIC_DIMENSION_LENGTH_H