#include <iostream>

int add(int arg1, int arg2);
int subtract(int arg1, int arg2);
int multiply(int arg1, int arg2);
int compute(int arg1, int arg2, unsigned char funcmask);

static int (*functs[4])(int, int) = { &add, &subtract, 0, &multiply };

#define ONE_BYTE 1
#define TWO_BYTE 2
#define FOUR_BYTE 4
int main() {
	std::cout << compute(1, 2, FOUR_BYTE) << '\n';
}

#define SEVEN_BYTE 7
int compute(int arg1, int arg2, unsigned char funcmask) {
	std::cout << (funcmask & SEVEN_BYTE) << '\n';
	return functs[(funcmask & SEVEN_BYTE) - 1](arg1, arg2);
}

int add(int arg1, int arg2) {
	std::cout << "Add." << '\n';
	return arg1 + arg2;
}

int subtract(int arg1, int arg2) {
	std::cout << "Sub." << '\n';
	return arg1 - arg2;
}

int multiply(int arg1, int arg2) {
	std::cout << "Mult." << '\n';
	return arg1 * arg2;
}
