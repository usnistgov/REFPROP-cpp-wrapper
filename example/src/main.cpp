#include <cassert>
#include <chrono>
#include "test_utils.h"
#include "refprop_v10.h"
#include "fluid_codes.h"

void TestGetEnumLib() {
    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Call function and test outputs
    EXPECT_EQ(0, refpropv10.GetEnumLib("DEFAULT"));
    //EXPECT_EQ(-9999970, refpropv10.GetEnumLib("MOLE SI"));      // MOLE SI is currently not recognized
    EXPECT_EQ(2, refpropv10.GetEnumLib("MASS SI"));
    EXPECT_EQ(3, refpropv10.GetEnumLib("SI WITH C"));
    EXPECT_EQ(20, refpropv10.GetEnumLib("MOLAR BASE SI"));
    EXPECT_EQ(21, refpropv10.GetEnumLib("MASS BASE SI"));
    EXPECT_EQ(5, refpropv10.GetEnumLib("ENGLISH"));
    EXPECT_EQ(6, refpropv10.GetEnumLib("MOLAR ENGLISH"));
    EXPECT_EQ(7, refpropv10.GetEnumLib("MKS"));
    EXPECT_EQ(8, refpropv10.GetEnumLib("CGS"));
    EXPECT_EQ(9, refpropv10.GetEnumLib("MIXED"));
    EXPECT_EQ(10, refpropv10.GetEnumLib("MEUNITS"));
    EXPECT_EQ(11, refpropv10.GetEnumLib("USER"));
}

void TestSetFluidsLib() {
    const int SUCCESS = 0;
    
    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Call function and test for success
    EXPECT_EQ(SUCCESS, refpropv10.SetFluidsLib("ARGON"));
    EXPECT_EQ(SUCCESS, refpropv10.SetFluidsLib("ARGON; NITROGEN"));
    EXPECT_NE(SUCCESS, refpropv10.SetFluidsLib("NOT_A_FLUID_NAME"));
}

void TestAbflshLib() {
    const double kRelTol = 0.001;    // 0.001 == 0.1 %

    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Set fluid
    refpropv10.SetFluidsLib("NITROGEN;OXYGEN");

    // Define inputs
    RefpropV10::LibInputs inputs;
    inputs.hIn = "TP";
    inputs.a = 293;
    inputs.b = 101.325;
    inputs.z = { 0.5, 0.5 };
    inputs.iFlag = 0;

    // Call AbflshLib using inputs
    RefpropV10::LibOutputs outputs = refpropv10.AbflshLib(inputs);

    // Map outputs
    std::map<std::string, double> output_map = IndexVectorByLabels(outputs.Output, outputs.output_props);

    // Test outputs
    EXPECT_NEAR_REL(8516.9320996925780, output_map["h"], kRelTol);
}

void TestAllPropsLib() {
    const double kRelTol = 0.001;    // 0.001 == 0.1 %

    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Set fluid
    int result = refpropv10.SetFluidsLib("NITROGEN;OXYGEN");

    // Define inputs
    RefpropV10::LibInputs inputs;
    inputs.hOut = "H;S";
    inputs.iUnits = Units::MOLAR_BASE_SI;
    inputs.iMass = Basis::MOLAR;
    inputs.iFlag = FlagAllPropsLib::WRITE_NO_UNITS;
    inputs.a = 293.;              // T [K]
    inputs.b = 41.613;            // D [mol/m3]
    inputs.z = { 0.5, 0.5 };

    // Call AllPropsLib using inputs
    RefpropV10::LibOutputs outputs = refpropv10.AllPropsLib(inputs);

    // Map and test outputs
    std::map<std::string, double> output_map = IndexVectorByLabels(outputs.Output, outputs.output_props);
    EXPECT_NEAR_REL(8516.9320480273145, output_map["H"], kRelTol);
}

void TestRefpropLib() {
    const double kRelTol = 0.001;    // 0.001 == 0.1 %

    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Define inputs
    RefpropV10::LibInputs inputs;
    inputs.hFld = "WATER";
    inputs.hIn = "PQ";
    inputs.hOut = "T";
    inputs.iUnits = Units::DEFAULT;
    inputs.iMass = Basis::MOLAR;
    inputs.iFlag = FlagRefpropLib::NONE;
    inputs.a = 101.325;
    inputs.b = 0.;
    inputs.z = { 20., 0. };

    // Call Refprop using inputs
    RefpropV10::LibOutputs outputs = refpropv10.RefpropLib(inputs);

    // Map and test outputs
    std::map<std::string, double> output_map = IndexVectorByLabels(outputs.Output, outputs.output_props);
    EXPECT_NEAR_REL(373.1242958477, output_map["T"], kRelTol);
}

void TestRefpropParallel() {
    const double kRelTol = 0.001;    // 0.001 == 0.1 %
    std::vector<double> input_pressures = { 80., 85., 90., 95., 100., 105., 110., 115., 120., 125. };
    std::vector<double> expected_temps = { 366.636, 368.275, 369.837, 371.328, 372.756, 374.126, 375.442, 376.710, 377.934, 379.115 };

    // Define constant inputs
    auto GetDefaultInputs = []() {
        RefpropV10::LibInputs inputs;
        inputs.hFld = "WATER";
        inputs.hIn = "PQ";
        inputs.hOut = "T";
        inputs.iUnits = Units::DEFAULT;
        inputs.iMass = Basis::MOLAR;
        inputs.iFlag = FlagRefpropLib::NONE;
        inputs.a = 100;             // kPa
        inputs.b = 0.;              // kg/kg quality
        inputs.z = { 1. };          // composition
        return inputs;
        };

    auto RefpropTempK = [GetDefaultInputs](double P) {
        RefpropV10 refpropv10;
        auto inputs = GetDefaultInputs();
        inputs.a = P;
        RefpropV10::LibOutputs outputs = refpropv10.RefpropLib(inputs);
        return outputs.Output.at(0);
        };

    // Run in series and print elapsed run time
    RefpropV10 refpropv10;          // don't instantiate a new instance for each, as done in parallel runs
    RefpropV10::LibInputs inputs = GetDefaultInputs();
    auto start = std::chrono::high_resolution_clock::now();
    assert(input_pressures.size() == expected_temps.size());
    for (size_t i = 0; i < input_pressures.size(); ++i) {
        inputs.a = input_pressures[i];
        RefpropV10::LibOutputs outputs = refpropv10.RefpropLib(inputs);
        EXPECT_NEAR_REL(expected_temps[i], outputs.Output.at(0), kRelTol);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Series runs completed in " << duration << " ms." << std::endl;

    // Run in parallel and print elapsed run time
    start = std::chrono::high_resolution_clock::now();
    ParallelTester(RefpropTempK, input_pressures, expected_temps, kRelTol);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Parallel runs completed in " << duration << " ms." << std::endl;
}

int main() {
    TestGetEnumLib();
    TestSetFluidsLib();
    TestAbflshLib();
    TestAllPropsLib();
    TestRefpropLib();
    TestRefpropParallel();

    std::cout << "Tests finished." << std::endl;
    return 0;
}
