#pragma once
#include <string>
#include <limits>

template<typename T>
constexpr T NaN() {
    return std::numeric_limits<T>::quiet_NaN();      // simplify quiet_NaN()
}

template<typename T>
constexpr T inf() {
    return std::numeric_limits<T>::infinity();       // simplify infinity()
}

bool file_exists(const std::string& filename);

std::string parent_path(const std::string& filename);

std::string StripTrailingWhiteSpace(std::string str);

std::string StripTrailingWhiteSpaceAndPipes(std::string str);
