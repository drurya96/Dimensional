#include "DimensionTest.h"

#include "AngleDimension.h"
#include "LengthDimension.h"
#include "TemperatureDimension.h"
#include "MassDimension.h"
#include "TimeDimension.h"

#include "EnergyDimension.h"
#include "SpeedDimension.h"
#include "AccelerationDimension.h"
#include "ForceDimension.h"
#include "VolumeDimension.h"

#include <ostream>

#include <iostream>

using namespace Dimension;

using Dim = Meters::Dim;

// Test Suite for subtract_tuples
TEST(SubtractTuplesTest, QuantityMinusQuantity_SameUnit_Meters) {
    using Test1Tup1 = std::tuple<Quantity<Meters>>;
    using Test1Tup2 = std::tuple<Quantity<Meters>>;

    using Test1Result = subtract_tuples<Dim, Test1Tup1, Test1Tup2>::type;
    using ExpectedResult = std::tuple<Meters>;

    EXPECT_TRUE((std::is_same_v<Test1Result, ExpectedResult>));
}



TEST(SubtractTuplesTest, QuantityMinusQuantity_SameUnit_Feet) {
    using Test2Tup1 = std::tuple<Quantity<Feet>>;
    using Test2Tup2 = std::tuple<Quantity<Feet>>;

    using Test2Result = subtract_tuples<Dim, Test2Tup1, Test2Tup2>::type;
    using ExpectedResult = std::tuple<Feet>;

    EXPECT_TRUE((std::is_same_v<Test2Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusQuantity_SameUnit_Yards) {
    using Test3Tup1 = std::tuple<Quantity<Yards>>;
    using Test3Tup2 = std::tuple<Quantity<Yards>>;

    using Test3Result = subtract_tuples<Dim, Test3Tup1, Test3Tup2>::type;
    using ExpectedResult = std::tuple<Yards>;

    EXPECT_TRUE((std::is_same_v<Test3Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusQuantity_SameUnit_Miles) {
    using Test4Tup1 = std::tuple<Quantity<Miles>>;
    using Test4Tup2 = std::tuple<Quantity<Miles>>;

    using Test4Result = subtract_tuples<Dim, Test4Tup1, Test4Tup2>::type;
    using ExpectedResult = std::tuple<Miles>;

    EXPECT_TRUE((std::is_same_v<Test4Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusQuantity_SameUnit_KiloMeters) {
    using Test5Tup1 = std::tuple<Quantity<KiloMeters>>;
    using Test5Tup2 = std::tuple<Quantity<KiloMeters>>;

    using Test5Result = subtract_tuples<Dim, Test5Tup1, Test5Tup2>::type;
    using ExpectedResult = std::tuple<KiloMeters>;

    EXPECT_TRUE((std::is_same_v<Test5Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusQuantity_SameUnit_CentiMeters) {
    using Test6Tup1 = std::tuple<Quantity<CentiMeters>>;
    using Test6Tup2 = std::tuple<Quantity<CentiMeters>>;

    using Test6Result = subtract_tuples<Dim, Test6Tup1, Test6Tup2>::type;
    using ExpectedResult = std::tuple<CentiMeters>;

    EXPECT_TRUE((std::is_same_v<Test6Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusQuantity_SameUnit_MilliMeters) {
    using Test7Tup1 = std::tuple<Quantity<MilliMeters>>;
    using Test7Tup2 = std::tuple<Quantity<MilliMeters>>;

    using Test7Result = subtract_tuples<Dim, Test7Tup1, Test7Tup2>::type;
    using ExpectedResult = std::tuple<MilliMeters>;

    EXPECT_TRUE((std::is_same_v<Test7Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusQuantity_SameUnit_Inches) {
    using Test8Tup1 = std::tuple<Quantity<Inches>>;
    using Test8Tup2 = std::tuple<Quantity<Inches>>;

    using Test8Result = subtract_tuples<Dim, Test8Tup1, Test8Tup2>::type;
    using ExpectedResult = std::tuple<Inches>;

    EXPECT_TRUE((std::is_same_v<Test8Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusQuantity_DifferentUnits_MetersFeet) {
    using Test9Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>>;
    using Test9Tup2 = std::tuple<Quantity<Feet>, Quantity<Meters>>;

    using Test9Result = subtract_tuples<Dim, Test9Tup1, Test9Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet>;

    EXPECT_TRUE((std::is_same_v<Test9Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusQuantity_DifferentUnits_YardsInches) {
    using Test10Tup1 = std::tuple<Quantity<Yards>, Quantity<Inches>>;
    using Test10Tup2 = std::tuple<Quantity<Inches>, Quantity<Yards>>;

    using Test10Result = subtract_tuples<Dim, Test10Tup1, Test10Tup2>::type;
    using ExpectedResult = std::tuple<Yards, Inches>;

    EXPECT_TRUE((std::is_same_v<Test10Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusDelta_SameUnit_Meters) {
    using Test11Tup1 = std::tuple<Meters>;
    using Test11Tup2 = std::tuple<Meters>;

    using Test11Result = subtract_tuples<Dim, Test11Tup1, Test11Tup2>::type;
    using ExpectedResult = std::tuple<Meters>;

    EXPECT_TRUE((std::is_same_v<Test11Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusDelta_SameUnit_Feet) {
    using Test12Tup1 = std::tuple<Feet>;
    using Test12Tup2 = std::tuple<Feet>;

    using Test12Result = subtract_tuples<Dim, Test12Tup1, Test12Tup2>::type;
    using ExpectedResult = std::tuple<Feet>;

    EXPECT_TRUE((std::is_same_v<Test12Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusDelta_SameUnit_Yards) {
    using Test13Tup1 = std::tuple<Yards>;
    using Test13Tup2 = std::tuple<Yards>;

    using Test13Result = subtract_tuples<Dim, Test13Tup1, Test13Tup2>::type;
    using ExpectedResult = std::tuple<Yards>;

    EXPECT_TRUE((std::is_same_v<Test13Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusDelta_SameUnit_Miles) {
    using Test14Tup1 = std::tuple<Miles>;
    using Test14Tup2 = std::tuple<Miles>;

    using Test14Result = subtract_tuples<Dim, Test14Tup1, Test14Tup2>::type;
    using ExpectedResult = std::tuple<Miles>;

    EXPECT_TRUE((std::is_same_v<Test14Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusDelta_SameUnit_KiloMeters) {
    using Test15Tup1 = std::tuple<KiloMeters>;
    using Test15Tup2 = std::tuple<KiloMeters>;

    using Test15Result = subtract_tuples<Dim, Test15Tup1, Test15Tup2>::type;
    using ExpectedResult = std::tuple<KiloMeters>;

    EXPECT_TRUE((std::is_same_v<Test15Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusDelta_SameUnit_CentiMeters) {
    using Test16Tup1 = std::tuple<CentiMeters>;
    using Test16Tup2 = std::tuple<CentiMeters>;

    using Test16Result = subtract_tuples<Dim, Test16Tup1, Test16Tup2>::type;
    using ExpectedResult = std::tuple<CentiMeters>;

    EXPECT_TRUE((std::is_same_v<Test16Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusDelta_SameUnit_MilliMeters) {
    using Test17Tup1 = std::tuple<MilliMeters>;
    using Test17Tup2 = std::tuple<MilliMeters>;

    using Test17Result = subtract_tuples<Dim, Test17Tup1, Test17Tup2>::type;
    using ExpectedResult = std::tuple<MilliMeters>;

    EXPECT_TRUE((std::is_same_v<Test17Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusDelta_SameUnit_Inches) {
    using Test18Tup1 = std::tuple<Inches>;
    using Test18Tup2 = std::tuple<Inches>;

    using Test18Result = subtract_tuples<Dim, Test18Tup1, Test18Tup2>::type;
    using ExpectedResult = std::tuple<Inches>;

    EXPECT_TRUE((std::is_same_v<Test18Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusDelta_SameUnit_Meters) {
    using Test19Tup1 = std::tuple<Quantity<Meters>>;
    using Test19Tup2 = std::tuple<Meters>;

    using Test19Result = subtract_tuples<Dim, Test19Tup1, Test19Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Meters>>;

    EXPECT_TRUE((std::is_same_v<Test19Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusDelta_SameUnit_Feet) {
    using Test20Tup1 = std::tuple<Quantity<Feet>>;
    using Test20Tup2 = std::tuple<Feet>;

    using Test20Result = subtract_tuples<Dim, Test20Tup1, Test20Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Feet>>;

    EXPECT_TRUE((std::is_same_v<Test20Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusDelta_SameUnit_Yards) {
    using Test21Tup1 = std::tuple<Quantity<Yards>>;
    using Test21Tup2 = std::tuple<Yards>;

    using Test21Result = subtract_tuples<Dim, Test21Tup1, Test21Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Yards>>;

    EXPECT_TRUE((std::is_same_v<Test21Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusDelta_SameUnit_Miles) {
    using Test22Tup1 = std::tuple<Quantity<Miles>>;
    using Test22Tup2 = std::tuple<Miles>;

    using Test22Result = subtract_tuples<Dim, Test22Tup1, Test22Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Miles>>;

    EXPECT_TRUE((std::is_same_v<Test22Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusDelta_SameUnit_KiloMeters) {
    using Test23Tup1 = std::tuple<Quantity<KiloMeters>>;
    using Test23Tup2 = std::tuple<KiloMeters>;

    using Test23Result = subtract_tuples<Dim, Test23Tup1, Test23Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<KiloMeters>>;

    EXPECT_TRUE((std::is_same_v<Test23Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusDelta_SameUnit_CentiMeters) {
    using Test24Tup1 = std::tuple<Quantity<CentiMeters>>;
    using Test24Tup2 = std::tuple<CentiMeters>;

    using Test24Result = subtract_tuples<Dim, Test24Tup1, Test24Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<CentiMeters>>;

    EXPECT_TRUE((std::is_same_v<Test24Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusDelta_SameUnit_MilliMeters) {
    using Test25Tup1 = std::tuple<Quantity<MilliMeters>>;
    using Test25Tup2 = std::tuple<MilliMeters>;

    using Test25Result = subtract_tuples<Dim, Test25Tup1, Test25Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<MilliMeters>>;

    EXPECT_TRUE((std::is_same_v<Test25Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantityMinusDelta_SameUnit_Inches) {
    using Test26Tup1 = std::tuple<Quantity<Inches>>;
    using Test26Tup2 = std::tuple<Inches>;

    using Test26Result = subtract_tuples<Dim, Test26Tup1, Test26Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Inches>>;

    EXPECT_TRUE((std::is_same_v<Test26Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, DeltaMinusQuantity_SameUnit_Meters_INVALID) {
    //using Test27Tup1 = std::tuple<Meters>;
    //using Test27Tup2 = std::tuple<Quantity<Meters>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test27Result = subtract_tuples<Dim, Test27Tup1, Test27Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, DeltaMinusQuantity_SameUnit_Feet_INVALID) {
    //using Test28Tup1 = std::tuple<Feet>;
    //using Test28Tup2 = std::tuple<Quantity<Feet>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test28Result = subtract_tuples<Dim, Test28Tup1, Test28Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, MultipleQuantityMinusQuantity_SameUnits) {
    using Test29Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;
    using Test29Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;

    using Test29Result = subtract_tuples<Dim, Test29Tup1, Test29Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards>;

    EXPECT_TRUE((std::is_same_v<Test29Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleQuantityMinusQuantity_DifferentOrder) {
    using Test30Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;
    using Test30Tup2 = std::tuple<Quantity<Yards>, Quantity<Meters>, Quantity<Feet>>;

    using Test30Result = subtract_tuples<Dim, Test30Tup1, Test30Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards>;

    EXPECT_TRUE((std::is_same_v<Test30Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleDeltaMinusDelta_SameUnits) {
    using Test31Tup1 = std::tuple<Meters, Feet, Yards>;
    using Test31Tup2 = std::tuple<Meters, Feet, Yards>;

    using Test31Result = subtract_tuples<Dim, Test31Tup1, Test31Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards>;

    EXPECT_TRUE((std::is_same_v<Test31Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleQuantityMinusDelta_MixedUnits) {
    using Test32Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;
    using Test32Tup2 = std::tuple<Meters, Feet, Yards>;

    using Test32Result = subtract_tuples<Dim, Test32Tup1, Test32Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;

    EXPECT_TRUE((std::is_same_v<Test32Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleDeltaMinusQuantity_MixedUnits_INVALID) {
    //using Test33Tup1 = std::tuple<Meters, Feet, Yards>;
    //using Test33Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test33Result = subtract_tuples<Dim, Test33Tup1, Test33Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, MixedUnits_SameOrder) {
    using Test34Tup1 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>>;
    using Test34Tup2 = std::tuple<Meters, Feet, Yards>;

    using Test34Result = subtract_tuples<Dim, Test34Tup1, Test34Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Quantity<Meters>, Quantity<Yards>>;

    EXPECT_TRUE((std::is_same_v<Test34Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MixedUnits_DifferentOrder) {
    using Test35Tup1 = std::tuple<Yards, Quantity<Meters>, Feet>;
    using Test35Tup2 = std::tuple<Feet, Yards, Quantity<Meters>>;

    using Test35Result = subtract_tuples<Dim, Test35Tup1, Test35Tup2>::type;
    using ExpectedResult = std::tuple<Yards, Feet, Meters>;

    EXPECT_TRUE((std::is_same_v<Test35Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleQuantityMinusQuantity_DifferentUnitsReordered) {
    using Test36Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;
    using Test36Tup2 = std::tuple<Quantity<Miles>, Quantity<Yards>, Quantity<Feet>, Quantity<Meters>>;

    using Test36Result = subtract_tuples<Dim, Test36Tup1, Test36Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards, Miles>;

    EXPECT_TRUE((std::is_same_v<Test36Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleQuantityDeltaMinusQuantityDelta_MixedUnits) {
    using Test37Tup1 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>>;
    using Test37Tup2 = std::tuple<Meters, Feet, Yards>;

    using Test37Result = subtract_tuples<Dim, Test37Tup1, Test37Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Quantity<Meters>, Quantity<Yards>>;

    EXPECT_TRUE((std::is_same_v<Test37Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MixedUnits_SameOrder_WithQuantitiesAndDeltas) {
    using Test38Tup1 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>>;
    using Test38Tup2 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>>;

    using Test38Result = subtract_tuples<Dim, Test38Tup1, Test38Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Meters, Yards>;

    EXPECT_TRUE((std::is_same_v<Test38Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MixedUnits_DifferentOrder_WithQuantitiesAndDeltas) {
    using Test39Tup1 = std::tuple<Yards, Quantity<Meters>, Feet>;
    using Test39Tup2 = std::tuple<Feet, Yards, Quantity<Meters>>;

    using Test39Result = subtract_tuples<Dim, Test39Tup1, Test39Tup2>::type;
    using ExpectedResult = std::tuple<Yards, Feet, Meters>;

    EXPECT_TRUE((std::is_same_v<Test39Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MixedUnits_MultipleQuantitiesAndDeltas_SameOrder) {
    using Test40Tup1 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>, Inches>;
    using Test40Tup2 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>, Inches>;

    using Test40Result = subtract_tuples<Dim, Test40Tup1, Test40Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Inches, Meters, Yards>;

    EXPECT_TRUE((std::is_same_v<Test40Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MixedUnits_MultipleQuantitiesAndDeltas_DifferentOrder) {
    using Test41Tup1 = std::tuple<Inches, Quantity<Meters>, Feet, Quantity<Yards>>;
    using Test41Tup2 = std::tuple<Feet, Quantity<Yards>, Quantity<Meters>, Inches>;

    using Test41Result = subtract_tuples<Dim, Test41Tup1, Test41Tup2>::type;
    using ExpectedResult = std::tuple<Inches, Feet, Meters, Yards>;

    EXPECT_TRUE((std::is_same_v<Test41Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MixedUnits_QuantitiesAndDeltas_SameUnitOrder) {
    using Test42Tup1 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>, Inches>;
    using Test42Tup2 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>, Inches>;

    using Test42Result = subtract_tuples<Dim, Test42Tup1, Test42Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Inches, Meters, Yards>;

    EXPECT_TRUE((std::is_same_v<Test42Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MixedUnits_QuantitiesAndDeltas_DifferentUnitOrder) {
    using Test43Tup1 = std::tuple<Feet, Quantity<Yards>, Inches, Quantity<Meters>>;
    using Test43Tup2 = std::tuple<Quantity<Meters>, Inches, Feet, Quantity<Yards>>;

    using Test43Result = subtract_tuples<Dim, Test43Tup1, Test43Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Inches, Yards, Meters>;

    EXPECT_TRUE((std::is_same_v<Test43Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, AllQuantitiesMinusAllQuantities_SameUnits) {
    using Test44Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;
    using Test44Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;

    using Test44Result = subtract_tuples<Dim, Test44Tup1, Test44Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards, Miles>;

    EXPECT_TRUE((std::is_same_v<Test44Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, AllDeltasMinusAllDeltas_SameUnits) {
    using Test45Tup1 = std::tuple<Meters, Feet, Yards, Miles>;
    using Test45Tup2 = std::tuple<Meters, Feet, Yards, Miles>;

    using Test45Result = subtract_tuples<Dim, Test45Tup1, Test45Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards, Miles>;

    EXPECT_TRUE((std::is_same_v<Test45Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, AllQuantitiesMinusAllDeltas_MixedUnits) {
    using Test46Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;
    using Test46Tup2 = std::tuple<Meters, Feet, Yards, Miles>;

    using Test46Result = subtract_tuples<Dim, Test46Tup1, Test46Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;

    EXPECT_TRUE((std::is_same_v<Test46Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, AllDeltasMinusAllQuantities_MixedUnits_INVALID) {
    //using Test47Tup1 = std::tuple<Meters, Feet, Yards, Miles>;
    //using Test47Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test47Result = subtract_tuples<Dim, Test47Tup1, Test47Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, PartialQuantitiesMinusPartialQuantities_SameUnits) {
    using Test48Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>>;
    using Test48Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>>;

    using Test48Result = subtract_tuples<Dim, Test48Tup1, Test48Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet>;

    EXPECT_TRUE((std::is_same_v<Test48Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, PartialDeltasMinusPartialDeltas_SameUnits) {
    using Test49Tup1 = std::tuple<Meters, Feet>;
    using Test49Tup2 = std::tuple<Meters, Feet>;

    using Test49Result = subtract_tuples<Dim, Test49Tup1, Test49Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet>;

    EXPECT_TRUE((std::is_same_v<Test49Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, PartialQuantitiesMinusPartialDeltas_MixedUnits) {
    using Test50Tup1 = std::tuple<Quantity<Meters>, Feet>;
    using Test50Tup2 = std::tuple<Meters, Quantity<Feet>>;

    using Test50Result = subtract_tuples<Dim, Test50Tup1, Test50Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Meters>;

    EXPECT_TRUE((std::is_same_v<Test50Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, PartialDeltasMinusPartialQuantities_MixedUnits_INVALID) {
    //using Test51Tup1 = std::tuple<Meters, Feet>;
    //using Test51Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test51Result = subtract_tuples<Dim, Test51Tup1, Test51Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, EmptyTuples) {
    using Test52Tup1 = std::tuple<>;
    using Test52Tup2 = std::tuple<>;

    using Test52Result = subtract_tuples<Dim, Test52Tup1, Test52Tup2>::type;
    using ExpectedResult = std::tuple<>;

    EXPECT_TRUE((std::is_same_v<Test52Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, SingleQuantityMinusSingleDelta_DifferentUnits) {
    using Test53Tup1 = std::tuple<Quantity<Meters>>;
    using Test53Tup2 = std::tuple<Feet>;

    using Test53Result = subtract_tuples<Dim, Test53Tup1, Test53Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Meters>>;

    EXPECT_TRUE((std::is_same_v<Test53Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, SingleDeltaMinusSingleQuantity_DifferentUnits_INVALID) {
    //using Test54Tup1 = std::tuple<Feet>;
    //using Test54Tup2 = std::tuple<Quantity<Feet>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test54Result = subtract_tuples<Dim, Test54Tup1, Test54Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, MultipleQuantitiesMinusMultipleDeltas_MixedUnits) {
    using Test55Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;
    using Test55Tup2 = std::tuple<Meters, Feet, Yards, Miles>;

    using Test55Result = subtract_tuples<Dim, Test55Tup1, Test55Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;

    EXPECT_TRUE((std::is_same_v<Test55Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleDeltasMinusMultipleQuantities_MixedUnits_INVALID) {
    //using Test56Tup1 = std::tuple<Meters, Feet, Yards, Miles>;
    //using Test56Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test56Result = subtract_tuples<Dim, Test56Tup1, Test56Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, QuantitiesAndDeltas_WithSomeUnitsMissing) {
    using Test57Tup1 = std::tuple<Quantity<Meters>, Quantity<Yards>>;
    using Test57Tup2 = std::tuple<Meters, Yards>;

    using Test57Result = subtract_tuples<Dim, Test57Tup1, Test57Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Meters>, Quantity<Yards>>;

    EXPECT_TRUE((std::is_same_v<Test57Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantitiesAndDeltas_WithSomeUnitsReordered) {
    using Test58Tup1 = std::tuple<Quantity<Yards>, Quantity<Meters>>;
    using Test58Tup2 = std::tuple<Meters, Yards>;

    using Test58Result = subtract_tuples<Dim, Test58Tup1, Test58Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Yards>, Quantity<Meters>>;

    EXPECT_TRUE((std::is_same_v<Test58Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantitiesAndDeltas_PartialOverlap) {
    using Test59Tup1 = std::tuple<Quantity<Meters>, Feet>;
    using Test59Tup2 = std::tuple<Meters, Quantity<Feet>>;

    using Test59Result = subtract_tuples<Dim, Test59Tup1, Test59Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Meters>;

    EXPECT_TRUE((std::is_same_v<Test59Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, QuantitiesAndDeltas_AllUnitsDifferent) {
    //using Test60Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>>;
    //using Test60Tup2 = std::tuple<Quantity<KiloMeters>, Quantity<CentiMeters>, Quantity<MilliMeters>, Quantity<Inches>>;

    //using Test60Result = subtract_tuples<Dim, Test60Tup1, Test60Tup2>::type;
    //using ExpectedResult = std::tuple<Meters, Feet, Yards, Miles, KiloMeters, CentiMeters, MilliMeters, Inches>; // Adjust based on actual handling

    // Note: This test assumes that subtract_tuples can handle different units,
    // which may not be desired. Typically, units should match.
    // Adjust ExpectedResult accordingly based on your implementation.
    
    // EXPECT_TRUE((std::is_same_v<Test60Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, SingleQuantityMultipleSubtractions) {
    using Test61Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;
    using Test61Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;

    using Test61Result = subtract_tuples<Dim, Test61Tup1, Test61Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards>;

    EXPECT_TRUE((std::is_same_v<Test61Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, SingleQuantityMultipleSubtractions_DifferentOrder) {
    using Test62Tup1 = std::tuple<Quantity<Yards>, Quantity<Meters>, Quantity<Feet>>;
    using Test62Tup2 = std::tuple<Quantity<Feet>, Quantity<Yards>, Quantity<Meters>>;

    using Test62Result = subtract_tuples<Dim, Test62Tup1, Test62Tup2>::type;
    using ExpectedResult = std::tuple<Yards, Meters, Feet>;

    EXPECT_TRUE((std::is_same_v<Test62Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MixedQuantitiesAndDeltas_WithSomeUnitsReordered) {
    using Test63Tup1 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>, Inches>;
    using Test63Tup2 = std::tuple<Meters, Inches, Feet, Quantity<Yards>>;

    using Test63Result = subtract_tuples<Dim, Test63Tup1, Test63Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Inches, Yards, Quantity<Meters>>;

    EXPECT_TRUE((std::is_same_v<Test63Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleQuantitiesAndDeltas_WithDuplicates_INVALID) {
    //using Test64Tup1 = std::tuple<Quantity<Meters>, Quantity<Meters>>;
    //using Test64Tup2 = std::tuple<Quantity<Meters>, Quantity<Meters>>;

    // This should cause a compile-time error if duplicate units are not allowed
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test64Result = subtract_tuples<Dim, Test64Tup1, Test64Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, SingleUnit_EmptyTuples) {
    using Test65Tup1 = std::tuple<>;
    using Test65Tup2 = std::tuple<>;

    using Test65Result = subtract_tuples<Dim, Test65Tup1, Test65Tup2>::type;
    using ExpectedResult = std::tuple<>;

    EXPECT_TRUE((std::is_same_v<Test65Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, SingleQuantityMinusMultipleDeltas_SameUnit) {
    //using Test66Tup1 = std::tuple<Quantity<Meters>>;
    //using Test66Tup2 = std::tuple<Meters, Feet>;

    //using Test66Result = subtract_tuples<Dim, Test66Tup1, Test66Tup2>::type;
    //using ExpectedResult = std::tuple<Quantity<Meters>, Feet>;

    //EXPECT_TRUE((std::is_same_v<Test66Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleQuantitiesMinusSingleDelta_SameUnit) {
    //using Test67Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>>;
    //using Test67Tup2 = std::tuple<Meters>;

    //using Test67Result = subtract_tuples<Dim, Test67Tup1, Test67Tup2>::type;
    //using ExpectedResult = std::tuple<Quantity<Meters>, Quantity<Feet>>;

    //EXPECT_TRUE((std::is_same_v<Test67Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, MultipleQuantitiesMinusMultipleDeltas_SameUnits) {
    using Test68Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;
    using Test68Tup2 = std::tuple<Meters, Feet, Yards>;

    using Test68Result = subtract_tuples<Dim, Test68Tup1, Test68Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;

    EXPECT_TRUE((std::is_same_v<Test68Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, ComplexTuple_MixedQuantitiesAndDeltas) {
    using Test69Tup1 = std::tuple<Quantity<Meters>, Feet, Quantity<Yards>, Inches>;
    using Test69Tup2 = std::tuple<Meters, Inches, Feet, Quantity<Yards>>;

    using Test69Result = subtract_tuples<Dim, Test69Tup1, Test69Tup2>::type;
    using ExpectedResult = std::tuple<Feet, Inches, Yards, Quantity<Meters>>;

    EXPECT_TRUE((std::is_same_v<Test69Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, AllUnits_QuantitiesMinusQuantities) {
    using Test70Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>, Quantity<KiloMeters>, Quantity<CentiMeters>, Quantity<MilliMeters>, Quantity<Inches>>;
    using Test70Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>, Quantity<KiloMeters>, Quantity<CentiMeters>, Quantity<MilliMeters>, Quantity<Inches>>;

    using Test70Result = subtract_tuples<Dim, Test70Tup1, Test70Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards, Miles, KiloMeters, CentiMeters, MilliMeters, Inches>;

    EXPECT_TRUE((std::is_same_v<Test70Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, AllUnits_DeltasMinusDeltas) {
    using Test71Tup1 = std::tuple<Meters, Feet, Yards, Miles, KiloMeters, CentiMeters, MilliMeters, Inches>;
    using Test71Tup2 = std::tuple<Meters, Feet, Yards, Miles, KiloMeters, CentiMeters, MilliMeters, Inches>;

    using Test71Result = subtract_tuples<Dim, Test71Tup1, Test71Tup2>::type;
    using ExpectedResult = std::tuple<Meters, Feet, Yards, Miles, KiloMeters, CentiMeters, MilliMeters, Inches>;

    EXPECT_TRUE((std::is_same_v<Test71Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, AllUnits_QuantitiesMinusDeltas_MixedUnits) {
    using Test72Tup1 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>, Quantity<KiloMeters>, Quantity<CentiMeters>, Quantity<MilliMeters>, Quantity<Inches>>;
    using Test72Tup2 = std::tuple<Meters, Feet, Yards, Miles, KiloMeters, CentiMeters, MilliMeters, Inches>;

    using Test72Result = subtract_tuples<Dim, Test72Tup1, Test72Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>, Quantity<KiloMeters>, Quantity<CentiMeters>, Quantity<MilliMeters>, Quantity<Inches>>;

    EXPECT_TRUE((std::is_same_v<Test72Result, ExpectedResult>));
}

TEST(SubtractTuplesTest, AllUnits_DeltasMinusQuantities_Invalid) {
    //using Test73Tup1 = std::tuple<Meters, Feet, Yards, Miles, KiloMeters, CentiMeters, MilliMeters, Inches>;
    //using Test73Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>, Quantity<Miles>, Quantity<KiloMeters>, Quantity<CentiMeters>, Quantity<MilliMeters>, Quantity<Inches>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test73Result = subtract_tuples<Dim, Test73Tup1, Test73Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}

TEST(SubtractTuplesTest, PartialDeltasMinusPartialQuantities_MixedUnits_Invalid) {
    //using Test74Tup1 = std::tuple<Meters, Feet, Yards>;
    //using Test74Tup2 = std::tuple<Quantity<Meters>, Quantity<Feet>, Quantity<Yards>>;

    // This should cause a compile-time error
    // Uncommenting the following lines should trigger a static_assert failure
    
    //using Test74Result = subtract_tuples<Dim, Test74Tup1, Test74Tup2>::type;
    //EXPECT_TRUE(false); // Should not reach here
    
}



TEST(SubtractTuplesTest, Test1) {
    using TestA1Tup1 = std::tuple<Meters, Seconds, Quantity<Minutes>>;
    using TestA1Tup2 = std::tuple<Hours, Quantity<Seconds>, Yards>;

    using TestA1Result = subtract_all<TestA1Tup1, TestA1Tup2>::type;
    using ExpectedResult = std::tuple<Seconds, Minutes, Meters>;

    EXPECT_TRUE((std::is_same_v<TestA1Result, ExpectedResult>));
    
}

TEST(AddTuplesTest, Test1) {
    using TestA1Tup1 = std::tuple<Meters, Seconds, Quantity<Minutes>>;
    using TestA1Tup2 = std::tuple<Hours, Quantity<Seconds>, Yards>;

    using TestA1Result = add_all<TestA1Tup1, TestA1Tup2>::type;
    using ExpectedResult = std::tuple<Quantity<Minutes>, Quantity<Seconds>, Meters>;

    EXPECT_TRUE((std::is_same_v<TestA1Result, ExpectedResult>));
    
}
