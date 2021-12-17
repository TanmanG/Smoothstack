#include <iostream>
#include <math.h>
#include <limits>
#include <iomanip>

static int fibo[50];

int Fibonacci(int term);
int FibonacciLimit(int bitLimit);

int main() {
	std::cout << "-Prompt 1-" << '\n';
	std::cout << "(The computer's best attempt of) Fibonacci of 50: " << Fibonacci(50) << '\n';

	// Visible is the integer rapidly approaching its limit of 2 million before an overflow occurs.
	// This can be remedied slightly by switching to unsigned (raising the limit to 4 million) or a larger type.
	// Detection can only realistically be done through checks when adding Fibonacci terms in the final base case.
	// This is in part due to the lack of access or presence of overflow flags on processors.
	for (int i = 0; i < 50; i++) {
		std::cout << "Fibonacci " << i << ": " << fibo[i] << '\n';
	}

	std::cout << "-Prompt 2-" << '\n';

	int bitSize;
	std::cout << "Enter the size in bits to calculate: ";
	std::cin >> bitSize;
	std::cout << "Largest Fibonacci Sequence Index: " << FibonacciLimit(bitSize) << '\n';
	std::cout << "Hard Upper-Limit: " << std::fixed << std::pow(2, bitSize) - 1 << '\n';
	std::cout << "Fibonacci Number: " << Fibonacci(FibonacciLimit(bitSize)) << '\n';
}

int Fibonacci(int term) {
	if (term == 0) {
		return 0;
	}
	// Check if we are at indecies 0 or 1, where 1 is the return type and we cannot go back further.
	else if (term == 1 | term == 2) {
		return 1;
	} // Check if the value has been evaluated already.
	else if (fibo[term] != 0) {
		return fibo[term];
	} // Calculate, store, and return the missing fibonacci number.
	else {
		fibo[term] = Fibonacci(term - 1) + Fibonacci(term - 2);
		return fibo[term];
	}
}

int FibonacciLimit(int bitLimit) {
	// Calculate the golden ratio.
	float goldenRatio = (1 + std::sqrt(5)) / 2;
	float largestNum = std::pow(2, bitLimit) - 1;

	// Add two to adjust for how long it takes for the ratio to break out of inaccurate bases (0, 1, and 1 are terribly inprecise)
	return (std::log(largestNum/5)/std::log(goldenRatio)) + 5;
	// Since the golden ratio of the fibonacci sequence can be expressed as "(sqrt(5) + 1) / 2", and the ratio
	// raised to some N power times the starting fibonacci number (with precision tied to the size of the chosen
	// fibonacci number), the equation can be built around calculating the N power required, as we know the golden
	// ratio and the limit of a number that can fit into the given size of bits (2^bits - 1), allowing us to solve for
	// N giving us the index of the largest storable value of the fibonacci sequence.

	// HOWEVER, this solution is imperfect. The aforementioned precision issue with starting numbers is extremely crippling with
	// imprecision growing to almost 40% larger than the actual number when N=6. Thankfully, we can adjust the starting number by
	// dividing the limit by 5 (the 5th index in the fibonnaci sequence) then adding 5 (to adjust the result to account for the new
	// starting point).
}
