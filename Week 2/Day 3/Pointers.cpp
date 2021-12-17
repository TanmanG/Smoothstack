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
	std::cout << "Given 1 and 2 and a funcmask of 0000 0100: " << compute(1, 2, FOUR_BYTE) << '\n';
	std::cout << "Given 5 and 3 and a funcmask of 0000 0010: " << compute(5, 3, TWO_BYTE) << '\n';
	std::cout << "Given 4 and 6 and a funcmask of 0000 0001: " << compute(4, 6, ONE_BYTE) << '\n';
	return 0;
}

#define SEVEN_BYTE 7
int compute(int arg1, int arg2, unsigned char funcmask) {
	return functs[(funcmask & SEVEN_BYTE) - 1](arg1, arg2);
}

int add(int arg1, int arg2) {
	std::cout << "Adding " << arg1 << " + " << arg2 << " = ";
	return arg1 + arg2;
}

int subtract(int arg1, int arg2) {
	std::cout << "Subtracting " << arg1 << " - " << arg2 << " = ";
	return arg1 - arg2;
}

int multiply(int arg1, int arg2) {
	std::cout << "Multiplying " << arg1 << " * " << arg2 << " = ";
	return arg1 * arg2;
}
