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

   ASSERT_NEAR(getTime<Seconds>(timeInSeconds), 3600.0, TOLERANCE);
   ASSERT_NEAR(getTime<Minutes>(timeInSeconds), 60.0, TOLERANCE);
   ASSERT_NEAR(getTime<Hours>(timeInSeconds), 1.0, TOLERANCE);

   // Test from Minutes to other units
   Time<Minutes> timeInMinutes{120.0};  // 2 hours in minutes

   ASSERT_NEAR(getTime<Seconds>(timeInMinutes), 7200.0, TOLERANCE);
   ASSERT_NEAR(getTime<Minutes>(timeInMinutes), 120.0, TOLERANCE);
   ASSERT_NEAR(getTime<Hours>(timeInMinutes), 2.0, TOLERANCE);

   // Test from Hours to other units
   Time<Hours> timeInHours{3.0};  // 3 hours

   ASSERT_NEAR(getTime<Seconds>(timeInHours), 10800.0, TOLERANCE);
   ASSERT_NEAR(getTime<Minutes>(timeInHours), 180.0, TOLERANCE);
   ASSERT_NEAR(getTime<Hours>(timeInHours), 3.0, TOLERANCE);

}


TEST(FundamentalConversions, TestLengthConversions)
{

   // Base length in Meters
   Length<Meters> lengthInMeters{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInMeters), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInMeters), 32.8084, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInMeters), 393.701, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInMeters), 6.68459e-11, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInMeters), 0.0062137, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInMeters), 0.0053996, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInMeters), 0.00621371, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInMeters), 0.00207124, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInMeters), 5.46807, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInMeters), 0.0497097, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInMeters), 10.9361, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInMeters), 32.8083, TOLERANCE);

   // Test from Feet to other units
   Length<Feet> lengthInFeet{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInFeet), 3.048, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInFeet), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInFeet), 120.0, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInFeet), 2.03721e-12, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInFeet), 0.00189394, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInFeet), 0.00164579, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInFeet), 0.00189394, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInFeet), 0.000631313, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInFeet), 1.66667, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInFeet), 0.0151515, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInFeet), 3.33333, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInFeet), 9.99999, TOLERANCE);

   // Test from Inches to other units
   Length<Inches> lengthInInches{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInInches), 0.254, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInInches), 0.833333, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInInches), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInInches), 5.09302e-14, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInInches), 0.000157828, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInInches), 0.000137657, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInInches), 0.000157828, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInInches), 5.26094e-05, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInInches), 0.138889, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInInches), 0.00151515, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInInches), 0.277778, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInInches), 0.833333, TOLERANCE);

   // Test from Miles to other units
   Length<Miles> lengthInMiles{10.0};

   
   ASSERT_NEAR(getLength<Meters>(lengthInMiles), 16093.44, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInMiles), 52800.0, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInMiles), 633600.0, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInMiles), 1.07578e-07, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInMiles), 8.80003, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInMiles), 8.68976, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInMiles), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInMiles), 3.33333, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInMiles), 8800.0, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInMiles), 80.0, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInMiles), 17600.0, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInMiles), 52799.8944, TOLERANCE);

   // Test from Astronomical Units to other units
   Length<AstronomicalUnits> lengthInAU{1e-10};

   ASSERT_NEAR(getLength<Meters>(lengthInAU), 14.959787069, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInAU), 49.080666237, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInAU), 588.96799485, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInAU), 1e-10, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInAU), 0.00818013721609745, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInAU), 0.0080776388, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInAU), 0.0092955807, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInAU), 0.0030985269, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInAU), 8.1801110395, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInAU), 0.0743646458, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInAU), 16.360222079, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInAU), 49.080568076, TOLERANCE);

   Length<DataMiles> lengthInDataMiles{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInDataMiles), 18288.0, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInDataMiles), 60000.0, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInDataMiles), 720000, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInDataMiles), 1.07578e-07, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInDataMiles), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInDataMiles), 9.8747, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInDataMiles), 11.3636, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInDataMiles), 3.7878787879, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInDataMiles), 10000.0, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInDataMiles), 90.909090909, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInDataMiles), 20000, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInDataMiles), 59999.88, TOLERANCE);

   // Test from NauticalMiles to other units
   Length<NauticalMiles> lengthInNauticalMiles{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInNauticalMiles), 18520.0, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInNauticalMiles), 60761.15, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInNauticalMiles), 729133.858, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInNauticalMiles), 1.23787e-07, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInNauticalMiles), 10.1269, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInNauticalMiles), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInNauticalMiles), 11.5078, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInNauticalMiles), 3.83595, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInNauticalMiles), 10126.86, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInNauticalMiles), 92.064, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInNauticalMiles), 20253.7183, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInNauticalMiles), 60761.0333, TOLERANCE);

   // Test from Leagues to other units
   Length<Leagues> lengthInLeagues{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInLeagues), 48280.32, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInLeagues), 158400.0, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInLeagues), 1900800.0, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInLeagues), 3.227340053e-7, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInLeagues), 26.400084481, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInLeagues), 26.069287257, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInLeagues), 30.0, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInLeagues), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInLeagues), 26400.0, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInLeagues), 240.0, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInLeagues), 52800.0, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInLeagues), 158399.6832, TOLERANCE);

   // Test from Fathoms to other units
   Length<Fathoms> lengthInFathoms{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInFathoms), 18.288, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInFathoms), 60.0, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInFathoms), 720.0, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInFathoms), 1.22291e-11, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInFathoms), 0.0113636, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInFathoms), 0.0108021, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInFathoms), 0.0113636, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInFathoms), 0.00378788, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInFathoms), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInFathoms), 0.0909091, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInFathoms), 20.0, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInFathoms), 59.9997, TOLERANCE);

   // Test from Furlongs to other units
   Length<Furlong> lengthInFurlong{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInFurlong), 2011.68, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInFurlong), 6600.0, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInFurlong), 79200.0, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInFurlong), 1.3448e-10, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInFurlong), 1.10001, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInFurlong), 1.08688, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInFurlong), 1.25, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInFurlong), 0.416667, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInFurlong), 1100.0, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInFurlong), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInFurlong), 2200.0, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInFurlong), 6599.9868, TOLERANCE);

   // Test from Yards to other units
   Length<Yards> lengthInYards{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInYards), 9.144, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInYards), 30.0, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInYards), 360.0, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInYards), 6.11456e-12, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInYards), 0.00568182, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInYards), 0.00540104, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInYards), 0.00568182, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInYards), 0.00189394, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInYards), 5.0, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInYards), 0.0454545455, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInYards), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInYards), 29.9998, TOLERANCE);

   // Test from US_Survey_Feet to other units
   Length<US_Survey_Feet> lengthInUSSurveyFeet{10.0};

   ASSERT_NEAR(getLength<Meters>(lengthInUSSurveyFeet), 3.048006096, TOLERANCE);
   ASSERT_NEAR(getLength<Feet>(lengthInUSSurveyFeet), 10.0000002, TOLERANCE);
   ASSERT_NEAR(getLength<Inches>(lengthInUSSurveyFeet), 120.0, TOLERANCE);
   ASSERT_NEAR(getLength<AstronomicalUnits>(lengthInUSSurveyFeet), 2.0372e-12, TOLERANCE);
   ASSERT_NEAR(getLength<DataMiles>(lengthInUSSurveyFeet), 0.00189394, TOLERANCE);
   ASSERT_NEAR(getLength<NauticalMiles>(lengthInUSSurveyFeet), 0.00164579, TOLERANCE);
   ASSERT_NEAR(getLength<Miles>(lengthInUSSurveyFeet), 0.00189394, TOLERANCE);
   ASSERT_NEAR(getLength<Leagues>(lengthInUSSurveyFeet), 0.000631313, TOLERANCE);
   ASSERT_NEAR(getLength<Fathoms>(lengthInUSSurveyFeet), 1.66667, TOLERANCE);
   ASSERT_NEAR(getLength<Furlong>(lengthInUSSurveyFeet), 0.0151515, TOLERANCE);
   ASSERT_NEAR(getLength<Yards>(lengthInUSSurveyFeet), 3.33333, TOLERANCE);
   ASSERT_NEAR(getLength<US_Survey_Feet>(lengthInUSSurveyFeet), 10.0, TOLERANCE);
   
}

TEST(FundamentalConversions, TestMassConversions)
{

   // Test from Grams to other units
   Mass<Grams> massInGrams{1000.0};  // 1 kilogram in grams

   ASSERT_NEAR(getMass<Grams>(massInGrams), 1000.0, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInGrams), 2.20462, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInGrams), 35.27396, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInGrams), 0.068522, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInGrams), 15432.358353, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInGrams), 0.157473, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInGrams), 0.00110231, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInGrams), 0.000984207, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInGrams), 0.001, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInGrams), 1.0, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInGrams), 1000000.0, TOLERANCE);

   // Test from PoundMass to other units
   Mass<PoundMass> massInPounds{10.0};  // 10 pounds

   ASSERT_NEAR(getMass<Grams>(massInPounds), 4535.92, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInPounds), 10.0, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInPounds), 160.0, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInPounds), 0.3108095017, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInPounds), 70000.0, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInPounds), 0.714286, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInPounds), 0.005, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInPounds), 0.00446429, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInPounds), 0.00453592, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInPounds), 4.53592, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInPounds), 4535923.7, 0.5); // Higher tolerance for large order of magnitude difference

   // Test from Ounces to other units
   Mass<Ounces> massInOunces{16.0};  // 1 pound in ounces

   ASSERT_NEAR(getMass<Grams>(massInOunces), 453.592, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInOunces), 1.0, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInOunces), 16.0, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInOunces), 0.0310809502, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInOunces), 7000.0, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInOunces), 0.0714286, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInOunces), 0.0005, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInOunces), 0.000446429, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInOunces), 0.000453592, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInOunces), 0.453592, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInOunces), 453592.32, TOLERANCE);

   // Test from Slugs to other units
   Mass<Slugs> massInSlugs{5.0};

   ASSERT_NEAR(getMass<Grams>(massInSlugs), 72969.5, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInSlugs), 160.87, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInSlugs), 2573.92, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInSlugs), 5.0, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInSlugs), 1126091.47, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInSlugs), 11.4907, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInSlugs), 0.080435, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInSlugs), 0.071817, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInSlugs), 0.0729695147, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInSlugs), 72.969514686, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInSlugs), 72969514.686, 15.0); // Very high tolerance due to difference in order of magnitude

   // Test from Grains to other units
   Mass<Grains> massInGrains{7000.0};  // 1 pound in grains

   ASSERT_NEAR(getMass<Grams>(massInGrains), 453.59237, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInGrains), 1.0, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInGrains), 16.0, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInGrains), 0.03108095, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInGrains), 7000.0, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInGrains), 0.0714286, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInGrains), 0.0005, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInGrains), 0.000446429, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInGrains), 0.000453592, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInGrains), 0.453592, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInGrains), 453592.37, TOLERANCE);

   // Test from Stone to other units
   Mass<Stone> massInStone{2.0};

   ASSERT_NEAR(getMass<Grams>(massInStone), 12700.58636, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInStone), 28.0, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInStone), 448.0, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInStone), 0.8702666048, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInStone), 196000.0, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInStone), 2.0, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInStone), 0.014, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInStone), 0.0125, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInStone), 0.0127, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInStone), 12.7, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInStone), 12700600.0, 15.0); // Very high tolerance due to difference in order of magnitude

   // Test from ShortTon to other units
   Mass<ShortTon> massInShortTon{1.0};

   ASSERT_NEAR(getMass<Grams>(massInShortTon), 907184.74, 0.05); // slightly higher tolerance due to difference in order of magnitude
   ASSERT_NEAR(getMass<PoundMass>(massInShortTon), 2000.0, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInShortTon), 32000.0, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInShortTon), 62.1619, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInShortTon), 14000000.0, 1.0); // Very high tolerance due to difference in order of magnitude
   ASSERT_NEAR(getMass<Stone>(massInShortTon), 142.857, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInShortTon), 1.0, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInShortTon), 0.892857, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInShortTon), 0.907185, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInShortTon), 907.185, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInShortTon), 9.071847e8, TOLERANCE);

   // Test from LongTon to other units
   Mass<LongTon> massInLongTon{1.0};

   ASSERT_NEAR(getMass<Grams>(massInLongTon), 1016046.91, 0.1); // slightly higher tolerance due to difference in order of magnitude
   ASSERT_NEAR(getMass<PoundMass>(massInLongTon), 2240.0, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInLongTon), 35840.0, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInLongTon), 69.6213, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInLongTon), 15680000.0, 2.0); // Very high tolerance due to difference in order of magnitude
   ASSERT_NEAR(getMass<Stone>(massInLongTon), 160.0, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInLongTon), 1.12, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInLongTon), 1.0, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInLongTon), 1.01605, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInLongTon), 1016.05, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInLongTon), 1016046908.0, 100.0); // Very high tolerance due to difference in order of magnitude

   // Test from Tonne to other units
   Mass<Tonne> massInTonne{2.0};

   ASSERT_NEAR(getMass<Grams>(massInTonne), 2000000.0, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInTonne), 4409.25, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInTonne), 70547.923899, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInTonne), 137.0456, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInTonne), 30864716.706, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInTonne), 314.94608884, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInTonne), 2.20462, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInTonne), 1.96841, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInTonne), 2.0, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInTonne), 2000.0, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInTonne), 2.0e9, TOLERANCE);

   // Test from KiloGrams to other units
   Mass<KiloGrams> massInKilograms{5.0};

   ASSERT_NEAR(getMass<Grams>(massInKilograms), 5000.0, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInKilograms), 11.0231, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInKilograms), 176.37, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInKilograms), 0.343, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInKilograms), 77161.791764, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInKilograms), 0.7874, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInKilograms), 0.00551156, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInKilograms), 0.00492103, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInKilograms), 0.005, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInKilograms), 5.0, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInKilograms), 5000000.0, TOLERANCE);

   // Test from MilliGrams to other units
   Mass<MilliGrams> massInMilliGrams{1000000.0};

   ASSERT_NEAR(getMass<Grams>(massInMilliGrams), 1000.0, TOLERANCE);
   ASSERT_NEAR(getMass<PoundMass>(massInMilliGrams), 2.20462, TOLERANCE);
   ASSERT_NEAR(getMass<Ounces>(massInMilliGrams), 35.27396, TOLERANCE);
   ASSERT_NEAR(getMass<Slugs>(massInMilliGrams), 0.068522, TOLERANCE);
   ASSERT_NEAR(getMass<Grains>(massInMilliGrams), 15432.358353, TOLERANCE);
   ASSERT_NEAR(getMass<Stone>(massInMilliGrams), 0.157473, TOLERANCE);
   ASSERT_NEAR(getMass<ShortTon>(massInMilliGrams), 0.00110231, TOLERANCE);
   ASSERT_NEAR(getMass<LongTon>(massInMilliGrams), 0.000984207, TOLERANCE);
   ASSERT_NEAR(getMass<Tonne>(massInMilliGrams), 0.001, TOLERANCE);
   ASSERT_NEAR(getMass<KiloGrams>(massInMilliGrams), 1.0, TOLERANCE);
   ASSERT_NEAR(getMass<MilliGrams>(massInMilliGrams), 1000000.0, TOLERANCE);
}

TEST(FundamentalConversions, TestAngleConversions)
{

   // Test from Degrees to other units
   Angle<Degrees> angleInDegrees{180.0};  // 180 degrees, equivalent to pi radians

   ASSERT_NEAR(getAngle<Degrees>(angleInDegrees), 180.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(angleInDegrees), 3.14159, TOLERANCE);

   // Test from Radians to other units
   Angle<Radians> angleInRadians{3.14159};  // pi radians, equivalent to 180 degrees

   ASSERT_NEAR(getAngle<Degrees>(angleInRadians), 180.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(angleInRadians), 3.14159, TOLERANCE);

}

TEST(FundamentalConversions, TestTemperatureConversions)
{

   // IMPORTANT!
   // All of these conversions treat Temperature AS A DELTA RATHER THAN QUANTITY!

   // Test from Celsius to other units
   Temperature<Celsius> deltaInCelsius{100.0};  // 100-degree difference in Celsius

   ASSERT_NEAR(getTemperature<Celsius>(deltaInCelsius), 100.0, TOLERANCE);
   ASSERT_NEAR(getTemperature<Fahrenheit>(deltaInCelsius), 180.0, TOLERANCE);  // Delta_F = Delta_C * 9/5
   ASSERT_NEAR(getTemperature<Kelvin>(deltaInCelsius), 100.0, TOLERANCE);      // Kelvin scales like Celsius for deltas
   ASSERT_NEAR(getTemperature<Rankine>(deltaInCelsius), 180.0, TOLERANCE);     // Delta_R = Delta_C * 9/5

   // Test from Fahrenheit to other units
   Temperature<Fahrenheit> deltaInFahrenheit{180.0};  // 180-degree difference in Fahrenheit

   ASSERT_NEAR(getTemperature<Celsius>(deltaInFahrenheit), 100.0, TOLERANCE);  // Delta_C = Delta_F * 5/9
   ASSERT_NEAR(getTemperature<Fahrenheit>(deltaInFahrenheit), 180.0, TOLERANCE);
   ASSERT_NEAR(getTemperature<Kelvin>(deltaInFahrenheit), 100.0, TOLERANCE);
   ASSERT_NEAR(getTemperature<Rankine>(deltaInFahrenheit), 180.0, TOLERANCE);

   // Test from Kelvin to other units
   Temperature<Kelvin> deltaInKelvin{100.0};  // 100-unit difference in Kelvin

   ASSERT_NEAR(getTemperature<Celsius>(deltaInKelvin), 100.0, TOLERANCE);      // Kelvin deltas are same as Celsius
   ASSERT_NEAR(getTemperature<Fahrenheit>(deltaInKelvin), 180.0, TOLERANCE);   // Delta_F = Delta_K * 9/5
   ASSERT_NEAR(getTemperature<Kelvin>(deltaInKelvin), 100.0, TOLERANCE);
   ASSERT_NEAR(getTemperature<Rankine>(deltaInKelvin), 180.0, TOLERANCE);      // Delta_R = Delta_K * 9/5

   // Test from Rankine to other units
   Temperature<Rankine> deltaInRankine{180.0};  // 180-unit difference in Rankine

   ASSERT_NEAR(getTemperature<Celsius>(deltaInRankine), 100.0, TOLERANCE);     // Delta_C = Delta_R * 5/9
   ASSERT_NEAR(getTemperature<Fahrenheit>(deltaInRankine), 180.0, TOLERANCE);
   ASSERT_NEAR(getTemperature<Kelvin>(deltaInRankine), 100.0, TOLERANCE);      // Delta_K = Delta_R * 5/9
   ASSERT_NEAR(getTemperature<Rankine>(deltaInRankine), 180.0, TOLERANCE);

}

TEST(FundamentalConversions, TestChargeConversions)
{

   // Test from Coulombs to other units
   Charge<Coulombs> chargeInCoulombs{100.0};

   ASSERT_NEAR(getCharge<Coulombs>(chargeInCoulombs), 100.0, TOLERANCE);
   ASSERT_NEAR(getCharge<StatCoulombs>(chargeInCoulombs), 299792457999.96, TOLERANCE);
   ASSERT_NEAR(getCharge<ElementaryCharges>(chargeInCoulombs), 6.241506363094e+20, TOLERANCE);

   // Test from StatCoulombs to other units
   Charge<StatCoulombs> chargeInStatCoulombs{100.0};

   ASSERT_NEAR(getCharge<Coulombs>(chargeInStatCoulombs), 3.335640951E-8, TOLERANCE);
   ASSERT_NEAR(getCharge<StatCoulombs>(chargeInStatCoulombs), 100.0, TOLERANCE);
   ASSERT_NEAR(getCharge<ElementaryCharges>(chargeInStatCoulombs), 208194242268, 0.1); // Large numbers, so higher tolerance

   // Test from ElementaryCharges to other units
   Charge<ElementaryCharges> chargeInElementaryCharges{100.0};

   ASSERT_NEAR(getCharge<Coulombs>(chargeInElementaryCharges), 1.60217733E-17, TOLERANCE);
   ASSERT_NEAR(getCharge<StatCoulombs>(chargeInElementaryCharges), 4.803206799E-8, TOLERANCE);
   ASSERT_NEAR(getCharge<ElementaryCharges>(chargeInElementaryCharges), 100.0, TOLERANCE);

}
