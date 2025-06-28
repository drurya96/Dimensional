#include "DimensionTest.h"

using namespace dimension;
//using dimension::time;

TEST(FundamentalConversions, TestTimeConversions)
{

   //using namespace dimension::TEST;

   //Time myBadTime = Time<feet>(10.0); // Correctly fails to compile - Cannot create a Time object using feet.

   // Test from seconds to other units
   timespan<seconds> timeInseconds{3600.0};  // 1 hour in seconds

   EXPECT_NEAR(get_timespan_as<seconds>(timeInseconds), 3600.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<minutes>(timeInseconds), 60.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<hours>(timeInseconds), 1.0, TOLERANCE);

   // Test from minutes to other units
   timespan<minutes> timeInminutes{120.0};  // 2 hours in minutes

   EXPECT_NEAR(get_timespan_as<seconds>(timeInminutes), 7200.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<minutes>(timeInminutes), 120.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<hours>(timeInminutes), 2.0, TOLERANCE);

   // Test from hours to other units
   timespan<hours> timeInHours{3.0};  // 3 hours

   EXPECT_NEAR(get_timespan_as<seconds>(timeInHours), 10800.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<minutes>(timeInHours), 180.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<hours>(timeInHours), 3.0, TOLERANCE);

}


TEST(FundamentalConversions, TestlengthConversions)
{

   // Base length in meters
   length<meters> lengthInmeters{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInmeters), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInmeters), 32.8084, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInmeters), 393.701, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInmeters), 6.68459e-11, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInmeters), 0.0062137, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInmeters), 0.0053996, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInmeters), 0.00621371, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInmeters), 5.46807, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInmeters), 0.0497097, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInmeters), 10.9361, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInmeters), 32.8083, TOLERANCE);

   // Test from feet to other units
   length<feet> lengthInFeet{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInFeet), 3.048, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInFeet), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInFeet), 120.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInFeet), 2.03721e-12, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInFeet), 0.00189394, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInFeet), 0.00164579, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInFeet), 0.00189394, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInFeet), 1.66667, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInFeet), 0.0151515, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInFeet), 3.33333, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInFeet), 9.99999, TOLERANCE);

   // Test from inches to other units
   length<inches> lengthInInches{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInInches), 0.254, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInInches), 0.833333, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInInches), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInInches), 5.09302e-14, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInInches), 0.000157828, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInInches), 0.000137657, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInInches), 0.000157828, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInInches), 0.138889, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInInches), 0.00151515, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInInches), 0.277778, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInInches), 0.833333, TOLERANCE);

   // Test from miles to other units
   length<miles> lengthInMiles{10.0};

   
   EXPECT_NEAR(get_length_as<meters>(lengthInMiles), 16093.44, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInMiles), 52800.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInMiles), 633600.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInMiles), 1.07578e-07, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInMiles), 8.80003, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInMiles), 8.68976, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInMiles), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInMiles), 8800.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInMiles), 80.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInMiles), 17600.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInMiles), 52799.8944, TOLERANCE);

   // Test from Astronomical Units to other units
   length<astronomical_units> lengthInAU{1e-10};

   EXPECT_NEAR(get_length_as<meters>(lengthInAU), 14.959787069, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInAU), 49.080666237, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInAU), 588.96799485, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInAU), 1e-10, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInAU), 0.00818013721609745, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInAU), 0.0080776388, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInAU), 0.0092955807, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInAU), 8.1801110395, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInAU), 0.0743646458, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInAU), 16.360222079, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInAU), 49.080568076, TOLERANCE);

   length<data_miles> lengthInDataMiles{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInDataMiles), 18288.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInDataMiles), 60000.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInDataMiles), 720000, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInDataMiles), 1.07578e-07, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInDataMiles), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInDataMiles), 9.8747, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInDataMiles), 11.3636, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInDataMiles), 10000.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInDataMiles), 90.909090909, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInDataMiles), 20000, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInDataMiles), 59999.88, TOLERANCE);

   // Test from nautical_miles to other units
   length<nautical_miles> lengthInNauticalMiles{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInNauticalMiles), 18520.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInNauticalMiles), 60761.15, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInNauticalMiles), 729133.858, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInNauticalMiles), 1.23787e-07, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInNauticalMiles), 10.1269, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInNauticalMiles), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInNauticalMiles), 11.5078, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInNauticalMiles), 10126.86, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInNauticalMiles), 92.064, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInNauticalMiles), 20253.7183, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInNauticalMiles), 60761.0333, TOLERANCE);

   // Test from fathoms to other units
   length<fathoms> lengthInFathoms{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInFathoms), 18.288, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInFathoms), 60.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInFathoms), 720.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInFathoms), 1.22291e-11, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInFathoms), 0.0113636, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInFathoms), 0.0108021, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInFathoms), 0.0113636, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInFathoms), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInFathoms), 0.0909091, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInFathoms), 20.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInFathoms), 59.9997, TOLERANCE);

   // Test from Furlongs to other units
   length<furlong> lengthInFurlong{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInFurlong), 2011.68, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInFurlong), 6600.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInFurlong), 79200.2, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInFurlong), 1.3448e-10, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInFurlong), 1.10001, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInFurlong), 1.08688, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInFurlong), 1.25, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInFurlong), 1100.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInFurlong), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInFurlong), 2200.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInFurlong), 6599.9868, TOLERANCE);

   // Test from yards to other units
   length<yards> lengthInYards{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInYards), 9.144, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInYards), 30.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInYards), 360.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInYards), 6.11456e-12, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInYards), 0.00568182, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInYards), 0.00540104, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInYards), 0.00568182, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInYards), 5.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInYards), 0.0454545455, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInYards), 10.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInYards), 29.9998, TOLERANCE);

   // Test from us_survey_feet to other units
   length<us_survey_feet> lengthInUSSurveyFeet{10.0};

   EXPECT_NEAR(get_length_as<meters>(lengthInUSSurveyFeet), 3.048006096, TOLERANCE);
   EXPECT_NEAR(get_length_as<feet>(lengthInUSSurveyFeet), 10.0000002, TOLERANCE);
   EXPECT_NEAR(get_length_as<inches>(lengthInUSSurveyFeet), 120.0, TOLERANCE);
   EXPECT_NEAR(get_length_as<astronomical_units>(lengthInUSSurveyFeet), 2.0372e-12, TOLERANCE);
   EXPECT_NEAR(get_length_as<data_miles>(lengthInUSSurveyFeet), 0.00189394, TOLERANCE);
   EXPECT_NEAR(get_length_as<nautical_miles>(lengthInUSSurveyFeet), 0.00164579, TOLERANCE);
   EXPECT_NEAR(get_length_as<miles>(lengthInUSSurveyFeet), 0.00189394, TOLERANCE);
   EXPECT_NEAR(get_length_as<fathoms>(lengthInUSSurveyFeet), 1.66667, TOLERANCE);
   EXPECT_NEAR(get_length_as<furlong>(lengthInUSSurveyFeet), 0.0151515, TOLERANCE);
   EXPECT_NEAR(get_length_as<yards>(lengthInUSSurveyFeet), 3.33333, TOLERANCE);
   EXPECT_NEAR(get_length_as<us_survey_feet>(lengthInUSSurveyFeet), 10.0, TOLERANCE);
   
}

TEST(FundamentalConversions, TestmassConversions)
{

   // Test from grams to other units
   mass<grams> massInGrams{1000.0};  // 1 kilogram in grams

   EXPECT_NEAR(get_mass_as<grams>(massInGrams), 1000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInGrams), 2.20462, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInGrams), 35.27396, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInGrams), 0.068522, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInGrams), 15432.358353, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInGrams), 0.157473, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInGrams), 0.00110231, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInGrams), 0.000984207, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInGrams), 0.001, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInGrams), 1.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInGrams), 1000000.0, TOLERANCE);

   // Test from pound_mass to other units
   mass<pound_mass> massInPounds{10.0};  // 10 pounds

   EXPECT_NEAR(get_mass_as<grams>(massInPounds), 4535.92, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInPounds), 10.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInPounds), 160.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInPounds), 0.3108095017, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInPounds), 70000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInPounds), 0.714286, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInPounds), 0.005, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInPounds), 0.00446429, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInPounds), 0.00453592, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInPounds), 4.53592, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInPounds), 4535923.7, TOLERANCE); // Higher tolerance for large order of magnitude difference

   // Test from ounces to other units
   mass<ounces> massInOunces{16.0};  // 1 pound in ounces

   EXPECT_NEAR(get_mass_as<grams>(massInOunces), 453.592, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInOunces), 1.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInOunces), 16.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInOunces), 0.0310809502, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInOunces), 7000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInOunces), 0.0714286, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInOunces), 0.0005, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInOunces), 0.000446429, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInOunces), 0.000453592, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInOunces), 0.453592, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInOunces), 453592.32, TOLERANCE);

   // Test from slugs to other units
   mass<slugs> massInSlugs{5.0};

   EXPECT_NEAR(get_mass_as<grams>(massInSlugs), 72969.5, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInSlugs), 160.87, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInSlugs), 2573.92, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInSlugs), 5.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInSlugs), 1126091.6995, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInSlugs), 11.4907, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInSlugs), 0.080435, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInSlugs), 0.071817, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInSlugs), 0.0729695147, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInSlugs), 72.969514686, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInSlugs), 72969514.686, TOLERANCE); // Very high tolerance due to difference in order of magnitude

   // Test from grains to other units
   mass<grains> massInGrains{7000.0};  // 1 pound in grains

   EXPECT_NEAR(get_mass_as<grams>(massInGrains), 453.59237, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInGrains), 1.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInGrains), 16.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInGrains), 0.03108095, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInGrains), 7000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInGrains), 0.0714286, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInGrains), 0.0005, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInGrains), 0.000446429, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInGrains), 0.000453592, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInGrains), 0.453592, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInGrains), 453592.37, TOLERANCE);

   // Test from stone to other units
   mass<stone> massInStone{2.0};

   EXPECT_NEAR(get_mass_as<grams>(massInStone), 12700.58636, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInStone), 28.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInStone), 448.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInStone), 0.8702666048, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInStone), 196000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInStone), 2.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInStone), 0.014, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInStone), 0.0125, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInStone), 0.0127, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInStone), 12.7, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInStone), 12700600.0, 100.0); // Very high tolerance due to difference in order of magnitude

   // Test from short_ton to other units
   mass<short_ton> massInShortTon{1.0};

   EXPECT_NEAR(get_mass_as<grams>(massInShortTon), 907184.74, TOLERANCE); // slightly higher tolerance due to difference in order of magnitude
   EXPECT_NEAR(get_mass_as<pound_mass>(massInShortTon), 2000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInShortTon), 32000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInShortTon), 62.1619, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInShortTon), 14000000.0, TOLERANCE); // Very high tolerance due to difference in order of magnitude
   EXPECT_NEAR(get_mass_as<stone>(massInShortTon), 142.857, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInShortTon), 1.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInShortTon), 0.892857, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInShortTon), 0.907185, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInShortTon), 907.185, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInShortTon), 9.071847e8, 10000.0);

   // Test from long_ton to other units
   mass<long_ton> massInLongTon{1.0};

   EXPECT_NEAR(get_mass_as<grams>(massInLongTon), 1016046.91, TOLERANCE); // slightly higher tolerance due to difference in order of magnitude
   EXPECT_NEAR(get_mass_as<pound_mass>(massInLongTon), 2240.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInLongTon), 35840.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInLongTon), 69.6213, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInLongTon), 15680000.0, TOLERANCE); // Very high tolerance due to difference in order of magnitude
   EXPECT_NEAR(get_mass_as<stone>(massInLongTon), 160.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInLongTon), 1.12, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInLongTon), 1.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInLongTon), 1.01605, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInLongTon), 1016.05, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInLongTon), 1016046908.0, 10000.0); // Very high tolerance due to difference in order of magnitude

   // Test from tonne to other units
   mass<tonne> massInTonne{2.0};

   EXPECT_NEAR(get_mass_as<grams>(massInTonne), 2000000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInTonne), 4409.25, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInTonne), 70547.923899, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInTonne), 137.0456, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInTonne), 30864716.706, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInTonne), 314.94608884, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInTonne), 2.20462, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInTonne), 1.96841, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInTonne), 2.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInTonne), 2000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInTonne), 2.0e9, TOLERANCE);

   // Test from kilo_grams to other units
   mass<kilo_grams> massInKilograms{5.0};

   EXPECT_NEAR(get_mass_as<grams>(massInKilograms), 5000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInKilograms), 11.0231, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInKilograms), 176.37, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInKilograms), 0.343, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInKilograms), 77161.791764, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInKilograms), 0.7874, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInKilograms), 0.00551156, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInKilograms), 0.00492103, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInKilograms), 0.005, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInKilograms), 5.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInKilograms), 5000000.0, TOLERANCE);

   // Test from milli_grams to other units
   mass<milli_grams> massInMilliGrams{1000000.0};

   EXPECT_NEAR(get_mass_as<grams>(massInMilliGrams), 1000.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<pound_mass>(massInMilliGrams), 2.20462, TOLERANCE);
   EXPECT_NEAR(get_mass_as<ounces>(massInMilliGrams), 35.27396, TOLERANCE);
   EXPECT_NEAR(get_mass_as<slugs>(massInMilliGrams), 0.068522, TOLERANCE);
   EXPECT_NEAR(get_mass_as<grains>(massInMilliGrams), 15432.358353, TOLERANCE);
   EXPECT_NEAR(get_mass_as<stone>(massInMilliGrams), 0.157473, TOLERANCE);
   EXPECT_NEAR(get_mass_as<short_ton>(massInMilliGrams), 0.00110231, TOLERANCE);
   EXPECT_NEAR(get_mass_as<long_ton>(massInMilliGrams), 0.000984207, TOLERANCE);
   EXPECT_NEAR(get_mass_as<tonne>(massInMilliGrams), 0.001, TOLERANCE);
   EXPECT_NEAR(get_mass_as<kilo_grams>(massInMilliGrams), 1.0, TOLERANCE);
   EXPECT_NEAR(get_mass_as<milli_grams>(massInMilliGrams), 1000000.0, TOLERANCE);
}

TEST(FundamentalConversions, TestangleConversions)
{

   // Test from degrees to other units
   angle<degrees> angleInDegrees{180.0};  // 180 degrees, equivalent to pi radians

   EXPECT_NEAR(get_angle_as<degrees>(angleInDegrees), 180.0, TOLERANCE);
   EXPECT_NEAR(get_angle_as<radians>(angleInDegrees), 3.14159, TOLERANCE);

   // Test from radians to other units
   angle<radians> angleInradians{3.14159};  // pi radians, equivalent to 180 degrees

   EXPECT_NEAR(get_angle_as<degrees>(angleInradians), 180.0, TOLERANCE);
   EXPECT_NEAR(get_angle_as<radians>(angleInradians), 3.14159, TOLERANCE);

}

TEST(FundamentalConversions, TesttemperatureConversions)
{

   // IMPORTANT!
   // All of these conversions treat temperature AS A DELTA RATHER THAN QUANTITY!

   // Test from kelvin to other units
   temperature<kelvin> deltaInKelvin{100.0};  // 100-unit difference in kelvin

   EXPECT_NEAR(get_temperature_as<kelvin>(deltaInKelvin), 100.0, TOLERANCE);
   EXPECT_NEAR(get_temperature_as<rankine>(deltaInKelvin), 180.0, TOLERANCE);      // Delta_R = Delta_K * 9/5

   // Test from rankine to other units
   temperature<rankine> deltaInRankine{180.0};  // 180-unit difference in rankine

   EXPECT_NEAR(get_temperature_as<kelvin>(deltaInRankine), 100.0, TOLERANCE);      // Delta_K = Delta_R * 5/9
   EXPECT_NEAR(get_temperature_as<rankine>(deltaInRankine), 180.0, TOLERANCE);

}

TEST(FundamentalConversions, TestchargeConversions)
{

   // Test from coulombs to other units
   charge<coulombs> chargeInCoulombs{100.0};

   EXPECT_NEAR(get_charge_as<coulombs>(chargeInCoulombs), 100.0, TOLERANCE);
   EXPECT_NEAR(get_charge_as<elementary_charges>(chargeInCoulombs), 6.2415090744607624e+20, TOLERANCE);

   // Test from elementary_charges to other units
   charge<elementary_charges> chargeInElementarycharges{100.0};

   EXPECT_NEAR(get_charge_as<coulombs>(chargeInElementarycharges), 1.60217733E-17, TOLERANCE);
   EXPECT_NEAR(get_charge_as<elementary_charges>(chargeInElementarycharges), 100.0, TOLERANCE);

}
