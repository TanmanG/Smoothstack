#include <iostream>
#include <iomanip>
#include <math.h>

int main() {
	// Precision is lost with floats as the size of the number grows, as the bits for exponent shrink.
	float x1 = 2.0f / 3.0f;
	std::cout << std::setprecision(32) << std::fixed << x1 << '\n';
	float x2 = 1000000.0f + x1;
	std::cout << std::setprecision(32) << std::fixed << x2 << '\n';
}
