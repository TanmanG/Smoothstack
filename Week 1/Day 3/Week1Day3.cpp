#include <vector>
#include <iostream>
#include <string>

int main() {
	// Declare an array and vector, initializing the array.
	float myArr [3] = { 1.0f, 2.0f, 3.0f};
	std::vector<float> myVec;

	size_t size = 0;

	// Iterate through the array and push each element into the vector, incrementing the tracked size of the array
	// for later.
	for (float val : myArr) {
		std::cout << val << ' ';
		myVec.push_back(val);
		size++;
	}
	std::cout << '\n';

	// Using the tracked array size from earlier, iterate through each element of the vector and print each one.
	// Alternate solution would be to just use another iterator for-loop but that'd take some creative expression
	// away.
	for (size_t i = 0; i < size; i++) {
		std::cout << myVec.at(i) << ' ';
	}
	std::cout << '\n';
}
