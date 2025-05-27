#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"
#include "AngleDimension.h"
#include "TemperatureDimension.h"
#include "ChargeDimension.h"

using namespace Dimension;

TEST(FundamentalConversions, TestTimeConversions)
{

   //Time myBadTime = Time<Feet>(10.0); // Correctly fails to compile - Cannot create a Time object using Feet.

   // Test from Seconds to other units
   Time<Seconds> timeInSeconds{3600.0};  // 1 hour in seconds

   ASSERT_NEAR(get_time_as<Seconds>(timeInSeconds), 3600.0, TOLERANCE);
   ASSERT_NEAR(get_time_as<Minutes>(timeInSeconds), 60.0, TOLERANCE);
   ASSERT_NEAR(get_time_as<Hours>(timeInSeconds), 1.0, TOLERANCE);

   // Test from Minutes to other units
   Time<Minutes> timeInMinutes{120.0};  // 2 hours in minutes

   ASSERT_NEAR(get_time_as<Seconds>(timeInMinutes), 7200.0, TOLERANCE);
   ASSERT_NEAR(get_time_as<Minutes>(timeInMinutes), 120.0, TOLERANCE);
   ASSERT_NEAR(get_time_as<Hours>(timeInMinutes), 2.0, TOLERANCE);

   // Test from Hours to other units
   Time<Hours> timeInHours{3.0};  // 3 hours

   ASSERT_NEAR(get_time_as<Seconds>(timeInHours), 10800.0, TOLERANCE);
   ASSERT_NEAR(get_time_as<Minutes>(timeInHours), 180.0, TOLERANCE);
   ASSERT_NEAR(get_time_as<Hours>(timeInHours), 3.0, TOLERANCE);

}


TEST(FundamentalConversions, TestLengthConversions)
{

   // Base length in Meters
   Length<Meters> lengthInMeters{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInMeters), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInMeters), 32.8084, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInMeters), 393.701, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInMeters), 6.68459e-11, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInMeters), 0.0062137, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInMeters), 0.0053996, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInMeters), 0.00621371, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInMeters), 0.00207124, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInMeters), 5.46807, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInMeters), 0.0497097, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInMeters), 10.9361, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInMeters), 32.8083, TOLERANCE);

   // Test from Feet to other units
   Length<Feet> lengthInFeet{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInFeet), 3.048, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInFeet), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInFeet), 120.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInFeet), 2.03721e-12, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInFeet), 0.00189394, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInFeet), 0.00164579, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInFeet), 0.00189394, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInFeet), 0.000631313, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInFeet), 1.66667, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInFeet), 0.0151515, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInFeet), 3.33333, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInFeet), 9.99999, TOLERANCE);

   // Test from Inches to other units
   Length<Inches> lengthInInches{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInInches), 0.254, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInInches), 0.833333, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInInches), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInInches), 5.09302e-14, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInInches), 0.000157828, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInInches), 0.000137657, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInInches), 0.000157828, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInInches), 5.26094e-05, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInInches), 0.138889, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInInches), 0.00151515, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInInches), 0.277778, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInInches), 0.833333, TOLERANCE);

   // Test from Miles to other units
   Length<Miles> lengthInMiles{10.0};

   
   ASSERT_NEAR(get_length_as<Meters>(lengthInMiles), 16093.44, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInMiles), 52800.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInMiles), 633600.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInMiles), 1.07578e-07, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInMiles), 8.80003, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInMiles), 8.68976, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInMiles), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInMiles), 3.33333, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInMiles), 8800.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInMiles), 80.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInMiles), 17600.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInMiles), 52799.8944, TOLERANCE);

   // Test from Astronomical Units to other units
   Length<AstronomicalUnits> lengthInAU{1e-10};

   ASSERT_NEAR(get_length_as<Meters>(lengthInAU), 14.959787069, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInAU), 49.080666237, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInAU), 588.96799485, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInAU), 1e-10, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInAU), 0.00818013721609745, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInAU), 0.0080776388, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInAU), 0.0092955807, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInAU), 0.0030985269, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInAU), 8.1801110395, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInAU), 0.0743646458, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInAU), 16.360222079, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInAU), 49.080568076, TOLERANCE);

   Length<DataMiles> lengthInDataMiles{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInDataMiles), 18288.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInDataMiles), 60000.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInDataMiles), 720000, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInDataMiles), 1.07578e-07, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInDataMiles), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInDataMiles), 9.8747, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInDataMiles), 11.3636, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInDataMiles), 3.7878787879, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInDataMiles), 10000.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInDataMiles), 90.909090909, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInDataMiles), 20000, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInDataMiles), 59999.88, TOLERANCE);

   // Test from NauticalMiles to other units
   Length<NauticalMiles> lengthInNauticalMiles{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInNauticalMiles), 18520.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInNauticalMiles), 60761.15, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInNauticalMiles), 729133.858, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInNauticalMiles), 1.23787e-07, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInNauticalMiles), 10.1269, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInNauticalMiles), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInNauticalMiles), 11.5078, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInNauticalMiles), 3.83595, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInNauticalMiles), 10126.86, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInNauticalMiles), 92.064, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInNauticalMiles), 20253.7183, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInNauticalMiles), 60761.0333, TOLERANCE);

   // Test from Leagues to other units
   Length<Leagues> lengthInLeagues{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInLeagues), 48280.32, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInLeagues), 158400.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInLeagues), 1900800.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInLeagues), 3.227340053e-7, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInLeagues), 26.400084481, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInLeagues), 26.069287257, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInLeagues), 30.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInLeagues), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInLeagues), 26400.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInLeagues), 240.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInLeagues), 52800.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInLeagues), 158399.6832, TOLERANCE);

   // Test from Fathoms to other units
   Length<Fathoms> lengthInFathoms{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInFathoms), 18.288, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInFathoms), 60.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInFathoms), 720.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInFathoms), 1.22291e-11, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInFathoms), 0.0113636, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInFathoms), 0.0108021, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInFathoms), 0.0113636, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInFathoms), 0.00378788, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInFathoms), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInFathoms), 0.0909091, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInFathoms), 20.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInFathoms), 59.9997, TOLERANCE);

   // Test from Furlongs to other units
   Length<Furlong> lengthInFurlong{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInFurlong), 2011.68, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInFurlong), 6600.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInFurlong), 79200.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInFurlong), 1.3448e-10, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInFurlong), 1.10001, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInFurlong), 1.08688, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInFurlong), 1.25, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInFurlong), 0.416667, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInFurlong), 1100.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInFurlong), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInFurlong), 2200.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInFurlong), 6599.9868, TOLERANCE);

   // Test from Yards to other units
   Length<Yards> lengthInYards{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInYards), 9.144, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInYards), 30.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInYards), 360.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInYards), 6.11456e-12, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInYards), 0.00568182, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInYards), 0.00540104, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInYards), 0.00568182, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInYards), 0.00189394, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInYards), 5.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInYards), 0.0454545455, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInYards), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInYards), 29.9998, TOLERANCE);

   // Test from US_Survey_Feet to other units
   Length<US_Survey_Feet> lengthInUSSurveyFeet{10.0};

   ASSERT_NEAR(get_length_as<Meters>(lengthInUSSurveyFeet), 3.048006096, TOLERANCE);
   ASSERT_NEAR(get_length_as<Feet>(lengthInUSSurveyFeet), 10.0000002, TOLERANCE);
   ASSERT_NEAR(get_length_as<Inches>(lengthInUSSurveyFeet), 120.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<AstronomicalUnits>(lengthInUSSurveyFeet), 2.0372e-12, TOLERANCE);
   ASSERT_NEAR(get_length_as<DataMiles>(lengthInUSSurveyFeet), 0.00189394, TOLERANCE);
   ASSERT_NEAR(get_length_as<NauticalMiles>(lengthInUSSurveyFeet), 0.00164579, TOLERANCE);
   ASSERT_NEAR(get_length_as<Miles>(lengthInUSSurveyFeet), 0.00189394, TOLERANCE);
   ASSERT_NEAR(get_length_as<Leagues>(lengthInUSSurveyFeet), 0.000631313, TOLERANCE);
   ASSERT_NEAR(get_length_as<Fathoms>(lengthInUSSurveyFeet), 1.66667, TOLERANCE);
   ASSERT_NEAR(get_length_as<Furlong>(lengthInUSSurveyFeet), 0.0151515, TOLERANCE);
   ASSERT_NEAR(get_length_as<Yards>(lengthInUSSurveyFeet), 3.33333, TOLERANCE);
   ASSERT_NEAR(get_length_as<US_Survey_Feet>(lengthInUSSurveyFeet), 10.0, TOLERANCE);
   
}

TEST(FundamentalConversions, TestMassConversions)
{

   // Test from Grams to other units
   Mass<Grams> massInGrams{1000.0};  // 1 kilogram in grams

   ASSERT_NEAR(get_mass_as<Grams>(massInGrams), 1000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInGrams), 2.20462, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInGrams), 35.27396, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInGrams), 0.068522, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInGrams), 15432.358353, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInGrams), 0.157473, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInGrams), 0.00110231, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInGrams), 0.000984207, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInGrams), 0.001, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInGrams), 1.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInGrams), 1000000.0, TOLERANCE);

   // Test from PoundMass to other units
   Mass<PoundMass> massInPounds{10.0};  // 10 pounds

   ASSERT_NEAR(get_mass_as<Grams>(massInPounds), 4535.92, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInPounds), 10.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInPounds), 160.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInPounds), 0.3108095017, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInPounds), 70000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInPounds), 0.714286, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInPounds), 0.005, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInPounds), 0.00446429, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInPounds), 0.00453592, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInPounds), 4.53592, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInPounds), 4535923.7, 0.5); // Higher tolerance for large order of magnitude difference

   // Test from Ounces to other units
   Mass<Ounces> massInOunces{16.0};  // 1 pound in ounces

   ASSERT_NEAR(get_mass_as<Grams>(massInOunces), 453.592, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInOunces), 1.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInOunces), 16.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInOunces), 0.0310809502, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInOunces), 7000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInOunces), 0.0714286, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInOunces), 0.0005, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInOunces), 0.000446429, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInOunces), 0.000453592, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInOunces), 0.453592, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInOunces), 453592.32, TOLERANCE);

   // Test from Slugs to other units
   Mass<Slugs> massInSlugs{5.0};

   ASSERT_NEAR(get_mass_as<Grams>(massInSlugs), 72969.5, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInSlugs), 160.87, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInSlugs), 2573.92, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInSlugs), 5.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInSlugs), 1126091.47, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInSlugs), 11.4907, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInSlugs), 0.080435, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInSlugs), 0.071817, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInSlugs), 0.0729695147, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInSlugs), 72.969514686, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInSlugs), 72969514.686, 15.0); // Very high tolerance due to difference in order of magnitude

   // Test from Grains to other units
   Mass<Grains> massInGrains{7000.0};  // 1 pound in grains

   ASSERT_NEAR(get_mass_as<Grams>(massInGrains), 453.59237, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInGrains), 1.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInGrains), 16.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInGrains), 0.03108095, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInGrains), 7000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInGrains), 0.0714286, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInGrains), 0.0005, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInGrains), 0.000446429, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInGrains), 0.000453592, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInGrains), 0.453592, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInGrains), 453592.37, TOLERANCE);

   // Test from Stone to other units
   Mass<Stone> massInStone{2.0};

   ASSERT_NEAR(get_mass_as<Grams>(massInStone), 12700.58636, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInStone), 28.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInStone), 448.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInStone), 0.8702666048, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInStone), 196000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInStone), 2.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInStone), 0.014, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInStone), 0.0125, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInStone), 0.0127, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInStone), 12.7, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInStone), 12700600.0, 15.0); // Very high tolerance due to difference in order of magnitude

   // Test from ShortTon to other units
   Mass<ShortTon> massInShortTon{1.0};

   ASSERT_NEAR(get_mass_as<Grams>(massInShortTon), 907184.74, 0.05); // slightly higher tolerance due to difference in order of magnitude
   ASSERT_NEAR(get_mass_as<PoundMass>(massInShortTon), 2000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInShortTon), 32000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInShortTon), 62.1619, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInShortTon), 14000000.0, 1.0); // Very high tolerance due to difference in order of magnitude
   ASSERT_NEAR(get_mass_as<Stone>(massInShortTon), 142.857, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInShortTon), 1.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInShortTon), 0.892857, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInShortTon), 0.907185, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInShortTon), 907.185, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInShortTon), 9.071847e8, TOLERANCE);

   // Test from LongTon to other units
   Mass<LongTon> massInLongTon{1.0};

   ASSERT_NEAR(get_mass_as<Grams>(massInLongTon), 1016046.91, 0.1); // slightly higher tolerance due to difference in order of magnitude
   ASSERT_NEAR(get_mass_as<PoundMass>(massInLongTon), 2240.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInLongTon), 35840.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInLongTon), 69.6213, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInLongTon), 15680000.0, 2.0); // Very high tolerance due to difference in order of magnitude
   ASSERT_NEAR(get_mass_as<Stone>(massInLongTon), 160.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInLongTon), 1.12, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInLongTon), 1.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInLongTon), 1.01605, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInLongTon), 1016.05, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInLongTon), 1016046908.0, 100.0); // Very high tolerance due to difference in order of magnitude

   // Test from Tonne to other units
   Mass<Tonne> massInTonne{2.0};

   ASSERT_NEAR(get_mass_as<Grams>(massInTonne), 2000000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInTonne), 4409.25, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInTonne), 70547.923899, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInTonne), 137.0456, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInTonne), 30864716.706, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInTonne), 314.94608884, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInTonne), 2.20462, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInTonne), 1.96841, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInTonne), 2.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInTonne), 2000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInTonne), 2.0e9, TOLERANCE);

   // Test from KiloGrams to other units
   Mass<KiloGrams> massInKilograms{5.0};

   ASSERT_NEAR(get_mass_as<Grams>(massInKilograms), 5000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInKilograms), 11.0231, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInKilograms), 176.37, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInKilograms), 0.343, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInKilograms), 77161.791764, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInKilograms), 0.7874, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInKilograms), 0.00551156, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInKilograms), 0.00492103, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInKilograms), 0.005, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInKilograms), 5.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInKilograms), 5000000.0, TOLERANCE);

   // Test from MilliGrams to other units
   Mass<MilliGrams> massInMilliGrams{1000000.0};

   ASSERT_NEAR(get_mass_as<Grams>(massInMilliGrams), 1000.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<PoundMass>(massInMilliGrams), 2.20462, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Ounces>(massInMilliGrams), 35.27396, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Slugs>(massInMilliGrams), 0.068522, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Grains>(massInMilliGrams), 15432.358353, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Stone>(massInMilliGrams), 0.157473, TOLERANCE);
   ASSERT_NEAR(get_mass_as<ShortTon>(massInMilliGrams), 0.00110231, TOLERANCE);
   ASSERT_NEAR(get_mass_as<LongTon>(massInMilliGrams), 0.000984207, TOLERANCE);
   ASSERT_NEAR(get_mass_as<Tonne>(massInMilliGrams), 0.001, TOLERANCE);
   ASSERT_NEAR(get_mass_as<KiloGrams>(massInMilliGrams), 1.0, TOLERANCE);
   ASSERT_NEAR(get_mass_as<MilliGrams>(massInMilliGrams), 1000000.0, TOLERANCE);
}

TEST(FundamentalConversions, TestAngleConversions)
{

   // Test from Degrees to other units
   Angle<Degrees> angleInDegrees{180.0};  // 180 degrees, equivalent to pi radians

   ASSERT_NEAR(get_angle_as<Degrees>(angleInDegrees), 180.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<Radians>(angleInDegrees), 3.14159, TOLERANCE);

   // Test from Radians to other units
   Angle<Radians> angleInRadians{3.14159};  // pi radians, equivalent to 180 degrees

   ASSERT_NEAR(get_angle_as<Degrees>(angleInRadians), 180.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<Radians>(angleInRadians), 3.14159, TOLERANCE);

}

TEST(FundamentalConversions, TestTemperatureConversions)
{

   // IMPORTANT!
   // All of these conversions treat Temperature AS A DELTA RATHER THAN QUANTITY!

   // Test from Kelvin to other units
   Temperature<Kelvin> deltaInKelvin{100.0};  // 100-unit difference in Kelvin

   ASSERT_NEAR(get_temperature_as<Kelvin>(deltaInKelvin), 100.0, TOLERANCE);
   ASSERT_NEAR(get_temperature_as<Rankine>(deltaInKelvin), 180.0, TOLERANCE);      // Delta_R = Delta_K * 9/5

   // Test from Rankine to other units
   Temperature<Rankine> deltaInRankine{180.0};  // 180-unit difference in Rankine

   ASSERT_NEAR(get_temperature_as<Kelvin>(deltaInRankine), 100.0, TOLERANCE);      // Delta_K = Delta_R * 5/9
   ASSERT_NEAR(get_temperature_as<Rankine>(deltaInRankine), 180.0, TOLERANCE);

}

TEST(FundamentalConversions, TestChargeConversions)
{

   // Test from Coulombs to other units
   Charge<Coulombs> chargeInCoulombs{100.0};

   ASSERT_NEAR(get_charge_as<Coulombs>(chargeInCoulombs), 100.0, TOLERANCE);
   ASSERT_NEAR(get_charge_as<StatCoulombs>(chargeInCoulombs), 299792457999.96, TOLERANCE);
   ASSERT_NEAR(get_charge_as<ElementaryCharges>(chargeInCoulombs), 6.241506363094e+20, TOLERANCE);

   // Test from StatCoulombs to other units
   Charge<StatCoulombs> chargeInStatCoulombs{100.0};

   ASSERT_NEAR(get_charge_as<Coulombs>(chargeInStatCoulombs), 3.335640951E-8, TOLERANCE);
   ASSERT_NEAR(get_charge_as<StatCoulombs>(chargeInStatCoulombs), 100.0, TOLERANCE);
   ASSERT_NEAR(get_charge_as<ElementaryCharges>(chargeInStatCoulombs), 208194242268, 0.1); // Large numbers, so higher tolerance

   // Test from ElementaryCharges to other units
   Charge<ElementaryCharges> chargeInElementaryCharges{100.0};

   ASSERT_NEAR(get_charge_as<Coulombs>(chargeInElementaryCharges), 1.60217733E-17, TOLERANCE);
   ASSERT_NEAR(get_charge_as<StatCoulombs>(chargeInElementaryCharges), 4.803206799E-8, TOLERANCE);
   ASSERT_NEAR(get_charge_as<ElementaryCharges>(chargeInElementaryCharges), 100.0, TOLERANCE);

}
