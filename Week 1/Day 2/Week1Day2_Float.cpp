#include <iostream>
#include <iomanip>
#include <math.h>

int main() {
	// Precision is lost with floats as the size grows.
	float x1 = 2.0f / 3.0f;
	std::cout << std::setprecision(32) << std::fixed << x1 << '\n';
	float x2 = 1000000.0f + 2.0f / 3.0f;
	std::cout << std::setprecision(32) << std::fixed << x2 << '\n';
}
