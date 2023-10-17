#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <future>

/// These are macros created with the same signature as those in GoogleTest
#define EXPECT_EQ(expected, actual) \
  do { \
    if ((expected) != (actual)) { \
      std::cout << "Error: Expected " << (expected) << " to be equal to " << (actual) << " in " << __func__ << std::endl; \
    } \
  } while (0)

#define EXPECT_NE(expected, actual) \
  do { \
    if ((expected) == (actual)) { \
      std::cout << "Error: Expected " << (expected) << " to not be equal to " << (actual) << " in " << __func__ << std::endl; \
    } \
  } while (0)

#define EXPECT_NEAR(expected, actual, abs_tol) \
  do { \
    if (std::abs((expected) - (actual)) > (abs_tol)) { \
      std::cout << "Error: Expected " << (expected) << " to be within " << (abs_tol) << " of " << (actual) << " in " << __func__ << std::endl; \
    } \
  } while (0)

#define EXPECT_NEAR_REL(expected, actual, rel_tol) \
  EXPECT_NEAR(expected, actual, std::abs((expected) * (rel_tol)))


/// Parallel function tester
template<typename Func, typename InputType, typename OutputType>
void ParallelTester(Func func, const std::vector<InputType>&inputs, const std::vector<OutputType>&expected_outputs, double rel_tol) {
    assert(inputs.size() == expected_outputs.size());
    size_t num_tests = inputs.size();
    std::vector<std::future<void>> futures(num_tests);

    for (size_t i = 0; i < num_tests; ++i) {
        futures[i] = std::async(std::launch::async, [&, i]() {
            EXPECT_NEAR_REL(expected_outputs[i], func(inputs[i]), rel_tol);
        });
    }

    for (auto& f : futures) {
        f.get();
    }
}
