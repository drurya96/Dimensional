#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"
#include "AngleDimension.h"
#include "TemperatureDimension.h"

using namespace Dimension;

TEST_F(DimensionTest, TestTimeConversions)
{

   //Time myBadTime = Time<Feet>(10.0); // Correctly fails to compile - Cannot create a Time object using Feet.

   // Test from Seconds to other units
   Time<Seconds> timeInSeconds{3600.0};  // 1 hour in seconds

   ASSERT_NEAR(timeInSeconds.GetTime<Seconds>(), 3600.0, TOLERANCE);
   ASSERT_NEAR(timeInSeconds.GetTime<Minutes>(), 60.0, TOLERANCE);
   ASSERT_NEAR(timeInSeconds.GetTime<Hours>(), 1.0, TOLERANCE);

   // Test from Minutes to other units
   Time<Minutes> timeInMinutes{120.0};  // 2 hours in minutes

   ASSERT_NEAR(timeInMinutes.GetTime<Seconds>(), 7200.0, TOLERANCE);
   ASSERT_NEAR(timeInMinutes.GetTime<Minutes>(), 120.0, TOLERANCE);
   ASSERT_NEAR(timeInMinutes.GetTime<Hours>(), 2.0, TOLERANCE);

   // Test from Hours to other units
   Time<Hours> timeInHours{3.0};  // 3 hours

   ASSERT_NEAR(timeInHours.GetTime<Seconds>(), 10800.0, TOLERANCE);
   ASSERT_NEAR(timeInHours.GetTime<Minutes>(), 180.0, TOLERANCE);
   ASSERT_NEAR(timeInHours.GetTime<Hours>(), 3.0, TOLERANCE);

}


TEST_F(DimensionTest, TestLengthConversions)
{

   // Base length in Meters
   Length<Meters> lengthInMeters{10.0};

   ASSERT_NEAR(lengthInMeters.GetLength<Meters>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<Feet>(), 32.8084, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<Inches>(), 393.701, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<AstronomicalUnits>(), 6.68459e-11, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<DataMiles>(), 0.0062137, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<NauticalMiles>(), 0.0053996, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<Miles>(), 0.00621371, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<Leagues>(), 0.00207124, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<Fathoms>(), 5.46807, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<Furlong>(), 0.0497097, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<Yards>(), 10.9361, TOLERANCE);
   ASSERT_NEAR(lengthInMeters.GetLength<US_Survey_Feet>(), 32.8083, TOLERANCE);

   // Test from Feet to other units
   Length<Feet> lengthInFeet{10.0};

   ASSERT_NEAR(lengthInFeet.GetLength<Meters>(), 3.048, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<Feet>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<Inches>(), 120.0, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<AstronomicalUnits>(), 2.03721e-12, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<DataMiles>(), 0.00189394, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<NauticalMiles>(), 0.00164579, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<Miles>(), 0.00189394, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<Leagues>(), 0.000631313, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<Fathoms>(), 1.66667, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<Furlong>(), 0.0151515, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<Yards>(), 3.33333, TOLERANCE);
   ASSERT_NEAR(lengthInFeet.GetLength<US_Survey_Feet>(), 9.99999, TOLERANCE);

   // Test from Inches to other units
   Length<Inches> lengthInInches{10.0};

   ASSERT_NEAR(lengthInInches.GetLength<Meters>(), 0.254, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<Feet>(), 0.833333, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<Inches>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<AstronomicalUnits>(), 5.09302e-14, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<DataMiles>(), 0.000157828, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<NauticalMiles>(), 0.000137657, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<Miles>(), 0.000157828, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<Leagues>(), 5.26094e-05, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<Fathoms>(), 0.138889, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<Furlong>(), 0.00151515, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<Yards>(), 0.277778, TOLERANCE);
   ASSERT_NEAR(lengthInInches.GetLength<US_Survey_Feet>(), 0.833333, TOLERANCE);

   // Test from Miles to other units
   Length<Miles> lengthInMiles{10.0};

   
   ASSERT_NEAR(lengthInMiles.GetLength<Meters>(), 16093.44, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<Feet>(), 52800.0, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<Inches>(), 633600.0, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<AstronomicalUnits>(), 1.07578e-07, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<DataMiles>(), 8.80003, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<NauticalMiles>(), 8.68976, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<Miles>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<Leagues>(), 3.33333, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<Fathoms>(), 8800.0, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<Furlong>(), 80.0, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<Yards>(), 17600.0, TOLERANCE);
   ASSERT_NEAR(lengthInMiles.GetLength<US_Survey_Feet>(), 52799.8944, TOLERANCE);

   // Test from Astronomical Units to other units
   Length<AstronomicalUnits> lengthInAU{1e-10};

   ASSERT_NEAR(lengthInAU.GetLength<Meters>(), 14.959787069, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<Feet>(), 49.080666237, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<Inches>(), 588.96799485, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<AstronomicalUnits>(), 1e-10, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<DataMiles>(), 0.00818013721609745, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<NauticalMiles>(), 0.0080776388, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<Miles>(), 0.0092955807, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<Leagues>(), 0.0030985269, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<Fathoms>(), 8.1801110395, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<Furlong>(), 0.0743646458, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<Yards>(), 16.360222079, TOLERANCE);
   ASSERT_NEAR(lengthInAU.GetLength<US_Survey_Feet>(), 49.080568076, TOLERANCE);

   Length<DataMiles> lengthInDataMiles{10.0};

   ASSERT_NEAR(lengthInDataMiles.GetLength<Meters>(), 18288.0, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<Feet>(), 60000.0, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<Inches>(), 720000, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<AstronomicalUnits>(), 1.07578e-07, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<DataMiles>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<NauticalMiles>(), 9.8747, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<Miles>(), 11.3636, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<Leagues>(), 3.7878787879, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<Fathoms>(), 10000.0, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<Furlong>(), 90.909090909, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<Yards>(), 20000, TOLERANCE);
   ASSERT_NEAR(lengthInDataMiles.GetLength<US_Survey_Feet>(), 59999.88, TOLERANCE);

   // Test from NauticalMiles to other units
   Length<NauticalMiles> lengthInNauticalMiles{10.0};

   ASSERT_NEAR(lengthInNauticalMiles.GetLength<Meters>(), 18520.0, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<Feet>(), 60761.15, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<Inches>(), 729133.858, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<AstronomicalUnits>(), 1.23787e-07, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<DataMiles>(), 10.1269, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<NauticalMiles>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<Miles>(), 11.5078, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<Leagues>(), 3.83595, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<Fathoms>(), 10126.86, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<Furlong>(), 92.064, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<Yards>(), 20253.7183, TOLERANCE);
   ASSERT_NEAR(lengthInNauticalMiles.GetLength<US_Survey_Feet>(), 60761.0333, TOLERANCE);

   // Test from Leagues to other units
   Length<Leagues> lengthInLeagues{10.0};

   ASSERT_NEAR(lengthInLeagues.GetLength<Meters>(), 48280.32, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<Feet>(), 158400.0, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<Inches>(), 1900800.0, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<AstronomicalUnits>(), 3.227340053e-7, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<DataMiles>(), 26.400084481, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<NauticalMiles>(), 26.069287257, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<Miles>(), 30.0, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<Leagues>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<Fathoms>(), 26400.0, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<Furlong>(), 240.0, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<Yards>(), 52800.0, TOLERANCE);
   ASSERT_NEAR(lengthInLeagues.GetLength<US_Survey_Feet>(), 158399.6832, TOLERANCE);

   // Test from Fathoms to other units
   Length<Fathoms> lengthInFathoms{10.0};

   ASSERT_NEAR(lengthInFathoms.GetLength<Meters>(), 18.288, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<Feet>(), 60.0, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<Inches>(), 720.0, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<AstronomicalUnits>(), 1.22291e-11, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<DataMiles>(), 0.0113636, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<NauticalMiles>(), 0.0108021, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<Miles>(), 0.0113636, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<Leagues>(), 0.00378788, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<Fathoms>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<Furlong>(), 0.0909091, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<Yards>(), 20.0, TOLERANCE);
   ASSERT_NEAR(lengthInFathoms.GetLength<US_Survey_Feet>(), 59.9997, TOLERANCE);

   // Test from Furlongs to other units
   Length<Furlong> lengthInFurlong{10.0};

   ASSERT_NEAR(lengthInFurlong.GetLength<Meters>(), 2011.68, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<Feet>(), 6600.0, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<Inches>(), 79200.0, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<AstronomicalUnits>(), 1.3448e-10, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<DataMiles>(), 1.10001, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<NauticalMiles>(), 1.08688, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<Miles>(), 1.25, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<Leagues>(), 0.416667, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<Fathoms>(), 1100.0, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<Furlong>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<Yards>(), 2200.0, TOLERANCE);
   ASSERT_NEAR(lengthInFurlong.GetLength<US_Survey_Feet>(), 6599.9868, TOLERANCE);

   // Test from Yards to other units
   Length<Yards> lengthInYards{10.0};

   ASSERT_NEAR(lengthInYards.GetLength<Meters>(), 9.144, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<Feet>(), 30.0, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<Inches>(), 360.0, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<AstronomicalUnits>(), 6.11456e-12, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<DataMiles>(), 0.00568182, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<NauticalMiles>(), 0.00540104, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<Miles>(), 0.00568182, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<Leagues>(), 0.00189394, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<Fathoms>(), 5.0, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<Furlong>(), 0.0454545455, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<Yards>(), 10.0, TOLERANCE);
   ASSERT_NEAR(lengthInYards.GetLength<US_Survey_Feet>(), 29.9998, TOLERANCE);

   // Test from US_Survey_Feet to other units
   Length<US_Survey_Feet> lengthInUSSurveyFeet{10.0};

   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<Meters>(), 3.048006096, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<Feet>(), 10.0000002, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<Inches>(), 120.0, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<AstronomicalUnits>(), 2.0372e-12, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<DataMiles>(), 0.00189394, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<NauticalMiles>(), 0.00164579, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<Miles>(), 0.00189394, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<Leagues>(), 0.000631313, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<Fathoms>(), 1.66667, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<Furlong>(), 0.0151515, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<Yards>(), 3.33333, TOLERANCE);
   ASSERT_NEAR(lengthInUSSurveyFeet.GetLength<US_Survey_Feet>(), 10.0, TOLERANCE);
   
}

TEST_F(DimensionTest, TestMassConversions)
{

   // Test from Grams to other units
   Mass<Grams> massInGrams{1000.0};  // 1 kilogram in grams

   ASSERT_NEAR(massInGrams.GetMass<Grams>(), 1000.0, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<PoundMass>(), 2.20462, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<Ounces>(), 35.27396, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<Slugs>(), 0.068522, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<Grains>(), 15432.358353, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<Stone>(), 0.157473, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<ShortTon>(), 0.00110231, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<LongTon>(), 0.000984207, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<Tonne>(), 0.001, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<KiloGrams>(), 1.0, TOLERANCE);
   ASSERT_NEAR(massInGrams.GetMass<MilliGrams>(), 1000000.0, TOLERANCE);

   // Test from PoundMass to other units
   Mass<PoundMass> massInPounds{10.0};  // 10 pounds

   ASSERT_NEAR(massInPounds.GetMass<Grams>(), 4535.92, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<PoundMass>(), 10.0, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<Ounces>(), 160.0, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<Slugs>(), 0.3108095017, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<Grains>(), 70000.0, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<Stone>(), 0.714286, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<ShortTon>(), 0.005, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<LongTon>(), 0.00446429, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<Tonne>(), 0.00453592, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<KiloGrams>(), 4.53592, TOLERANCE);
   ASSERT_NEAR(massInPounds.GetMass<MilliGrams>(), 4535923.7, 0.5); // Higher tolerance for large order of magnitude difference

   // Test from Ounces to other units
   Mass<Ounces> massInOunces{16.0};  // 1 pound in ounces

   ASSERT_NEAR(massInOunces.GetMass<Grams>(), 453.592, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<PoundMass>(), 1.0, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<Ounces>(), 16.0, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<Slugs>(), 0.0310809502, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<Grains>(), 7000.0, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<Stone>(), 0.0714286, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<ShortTon>(), 0.0005, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<LongTon>(), 0.000446429, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<Tonne>(), 0.000453592, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<KiloGrams>(), 0.453592, TOLERANCE);
   ASSERT_NEAR(massInOunces.GetMass<MilliGrams>(), 453592.32, TOLERANCE);

   // Test from Slugs to other units
   Mass<Slugs> massInSlugs{5.0};

   ASSERT_NEAR(massInSlugs.GetMass<Grams>(), 72969.5, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<PoundMass>(), 160.87, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<Ounces>(), 2573.92, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<Slugs>(), 5.0, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<Grains>(), 1126091.47, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<Stone>(), 11.4907, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<ShortTon>(), 0.080435, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<LongTon>(), 0.071817, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<Tonne>(), 0.0729695147, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<KiloGrams>(), 72.969514686, TOLERANCE);
   ASSERT_NEAR(massInSlugs.GetMass<MilliGrams>(), 72969514.686, 15.0); // Very high tolerance due to difference in order of magnitude

   // Test from Grains to other units
   Mass<Grains> massInGrains{7000.0};  // 1 pound in grains

   ASSERT_NEAR(massInGrains.GetMass<Grams>(), 453.59237, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<PoundMass>(), 1.0, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<Ounces>(), 16.0, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<Slugs>(), 0.03108095, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<Grains>(), 7000.0, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<Stone>(), 0.0714286, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<ShortTon>(), 0.0005, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<LongTon>(), 0.000446429, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<Tonne>(), 0.000453592, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<KiloGrams>(), 0.453592, TOLERANCE);
   ASSERT_NEAR(massInGrains.GetMass<MilliGrams>(), 453592.37, TOLERANCE);

   // Test from Stone to other units
   Mass<Stone> massInStone{2.0};

   ASSERT_NEAR(massInStone.GetMass<Grams>(), 12700.58636, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<PoundMass>(), 28.0, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<Ounces>(), 448.0, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<Slugs>(), 0.8702666048, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<Grains>(), 196000.0, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<Stone>(), 2.0, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<ShortTon>(), 0.014, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<LongTon>(), 0.0125, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<Tonne>(), 0.0127, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<KiloGrams>(), 12.7, TOLERANCE);
   ASSERT_NEAR(massInStone.GetMass<MilliGrams>(), 12700600.0, 15.0); // Very high tolerance due to difference in order of magnitude

   // Test from ShortTon to other units
   Mass<ShortTon> massInShortTon{1.0};

   ASSERT_NEAR(massInShortTon.GetMass<Grams>(), 907184.74, 0.05); // slightly higher tolerance due to difference in order of magnitude
   ASSERT_NEAR(massInShortTon.GetMass<PoundMass>(), 2000.0, TOLERANCE);
   ASSERT_NEAR(massInShortTon.GetMass<Ounces>(), 32000.0, TOLERANCE);
   ASSERT_NEAR(massInShortTon.GetMass<Slugs>(), 62.1619, TOLERANCE);
   ASSERT_NEAR(massInShortTon.GetMass<Grains>(), 14000000.0, 1.0); // Very high tolerance due to difference in order of magnitude
   ASSERT_NEAR(massInShortTon.GetMass<Stone>(), 142.857, TOLERANCE);
   ASSERT_NEAR(massInShortTon.GetMass<ShortTon>(), 1.0, TOLERANCE);
   ASSERT_NEAR(massInShortTon.GetMass<LongTon>(), 0.892857, TOLERANCE);
   ASSERT_NEAR(massInShortTon.GetMass<Tonne>(), 0.907185, TOLERANCE);
   ASSERT_NEAR(massInShortTon.GetMass<KiloGrams>(), 907.185, TOLERANCE);
   ASSERT_NEAR(massInShortTon.GetMass<MilliGrams>(), 9.071847e8, TOLERANCE);

   // Test from LongTon to other units
   Mass<LongTon> massInLongTon{1.0};

   ASSERT_NEAR(massInLongTon.GetMass<Grams>(), 1016046.91, 0.1); // slightly higher tolerance due to difference in order of magnitude
   ASSERT_NEAR(massInLongTon.GetMass<PoundMass>(), 2240.0, TOLERANCE);
   ASSERT_NEAR(massInLongTon.GetMass<Ounces>(), 35840.0, TOLERANCE);
   ASSERT_NEAR(massInLongTon.GetMass<Slugs>(), 69.6213, TOLERANCE);
   ASSERT_NEAR(massInLongTon.GetMass<Grains>(), 15680000.0, 2.0); // Very high tolerance due to difference in order of magnitude
   ASSERT_NEAR(massInLongTon.GetMass<Stone>(), 160.0, TOLERANCE);
   ASSERT_NEAR(massInLongTon.GetMass<ShortTon>(), 1.12, TOLERANCE);
   ASSERT_NEAR(massInLongTon.GetMass<LongTon>(), 1.0, TOLERANCE);
   ASSERT_NEAR(massInLongTon.GetMass<Tonne>(), 1.01605, TOLERANCE);
   ASSERT_NEAR(massInLongTon.GetMass<KiloGrams>(), 1016.05, TOLERANCE);
   ASSERT_NEAR(massInLongTon.GetMass<MilliGrams>(), 1016046908.0, 100.0); // Very high tolerance due to difference in order of magnitude

   // Test from Tonne to other units
   Mass<Tonne> massInTonne{2.0};

   ASSERT_NEAR(massInTonne.GetMass<Grams>(), 2000000.0, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<PoundMass>(), 4409.25, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<Ounces>(), 70547.923899, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<Slugs>(), 137.0456, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<Grains>(), 30864716.706, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<Stone>(), 314.94608884, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<ShortTon>(), 2.20462, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<LongTon>(), 1.96841, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<Tonne>(), 2.0, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<KiloGrams>(), 2000.0, TOLERANCE);
   ASSERT_NEAR(massInTonne.GetMass<MilliGrams>(), 2.0e9, TOLERANCE);

   // Test from KiloGrams to other units
   Mass<KiloGrams> massInKilograms{5.0};

   ASSERT_NEAR(massInKilograms.GetMass<Grams>(), 5000.0, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<PoundMass>(), 11.0231, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<Ounces>(), 176.37, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<Slugs>(), 0.343, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<Grains>(), 77161.791764, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<Stone>(), 0.7874, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<ShortTon>(), 0.00551156, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<LongTon>(), 0.00492103, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<Tonne>(), 0.005, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<KiloGrams>(), 5.0, TOLERANCE);
   ASSERT_NEAR(massInKilograms.GetMass<MilliGrams>(), 5000000.0, TOLERANCE);

   // Test from MilliGrams to other units
   Mass<MilliGrams> massInMilliGrams{1000000.0};

   ASSERT_NEAR(massInMilliGrams.GetMass<Grams>(), 1000.0, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<PoundMass>(), 2.20462, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<Ounces>(), 35.27396, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<Slugs>(), 0.068522, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<Grains>(), 15432.358353, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<Stone>(), 0.157473, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<ShortTon>(), 0.00110231, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<LongTon>(), 0.000984207, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<Tonne>(), 0.001, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<KiloGrams>(), 1.0, TOLERANCE);
   ASSERT_NEAR(massInMilliGrams.GetMass<MilliGrams>(), 1000000.0, TOLERANCE);
}

TEST_F(DimensionTest, TestAngleConversions)
{

   // Test from Degrees to other units
   Angle<Degrees> angleInDegrees{180.0};  // 180 degrees, equivalent to pi radians

   ASSERT_NEAR(angleInDegrees.GetAngle<Degrees>(), 180.0, TOLERANCE);
   ASSERT_NEAR(angleInDegrees.GetAngle<Radians>(), 3.14159, TOLERANCE);

   // Test from Radians to other units
   Angle<Radians> angleInRadians{3.14159};  // pi radians, equivalent to 180 degrees

   ASSERT_NEAR(angleInRadians.GetAngle<Degrees>(), 180.0, TOLERANCE);
   ASSERT_NEAR(angleInRadians.GetAngle<Radians>(), 3.14159, TOLERANCE);

}

TEST_F(DimensionTest, TestTemperatureConversions)
{

   // Test from Celsius to other units
   Temperature<Celsius> tempInCelsius{100.0};  // 100 degrees Celsius

   ASSERT_NEAR(tempInCelsius.GetTemperature<Celsius>(), 100.0, TOLERANCE);
   ASSERT_NEAR(tempInCelsius.GetTemperature<Fahrenheit>(), 212.0, TOLERANCE);
   ASSERT_NEAR(tempInCelsius.GetTemperature<Kelvin>(), 373.15, TOLERANCE);
   ASSERT_NEAR(tempInCelsius.GetTemperature<Rankine>(), 671.67, TOLERANCE);

   // Test from Fahrenheit to other units
   Temperature<Fahrenheit> tempInFahrenheit{32.0};  // 32 degrees Fahrenheit (freezing point of water)

   ASSERT_NEAR(tempInFahrenheit.GetTemperature<Celsius>(), 0.0, TOLERANCE);
   ASSERT_NEAR(tempInFahrenheit.GetTemperature<Fahrenheit>(), 32.0, TOLERANCE);
   ASSERT_NEAR(tempInFahrenheit.GetTemperature<Kelvin>(), 273.15, TOLERANCE);
   ASSERT_NEAR(tempInFahrenheit.GetTemperature<Rankine>(), 491.67, TOLERANCE);

   // Test from Kelvin to other units
   Temperature<Kelvin> tempInKelvin{0.0};  // Absolute zero in Kelvin

   ASSERT_NEAR(tempInKelvin.GetTemperature<Celsius>(), -273.15, TOLERANCE);
   ASSERT_NEAR(tempInKelvin.GetTemperature<Fahrenheit>(), -459.67, TOLERANCE);
   ASSERT_NEAR(tempInKelvin.GetTemperature<Kelvin>(), 0.0, TOLERANCE);
   ASSERT_NEAR(tempInKelvin.GetTemperature<Rankine>(), 0.0, TOLERANCE);

   // Test from Rankine to other units
   Temperature<Rankine> tempInRankine{491.67};  // Freezing point of water in Rankine

   ASSERT_NEAR(tempInRankine.GetTemperature<Celsius>(), 0.0, TOLERANCE);
   ASSERT_NEAR(tempInRankine.GetTemperature<Fahrenheit>(), 32.0, TOLERANCE);
   ASSERT_NEAR(tempInRankine.GetTemperature<Kelvin>(), 273.15, TOLERANCE);
   ASSERT_NEAR(tempInRankine.GetTemperature<Rankine>(), 491.67, TOLERANCE);

}
