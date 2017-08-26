#pragma once

#include <string>
#include <sstream>
#include <iomanip>

template <typename T>
std::string ToString(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}