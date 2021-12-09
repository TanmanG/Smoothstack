#include <vector>
#include <iostream>
#include <string>

int main() {
	float myArr [3] = { 1.0f, 2.0f, 3.0f};
	size_t size = 0;
	std::vector<float> myVec;

	for (float val : myArr) {
		std::cout << val << ' ';
		myVec.push_back(val);
		size++;
	}
	std::cout << '\n';

	for (size_t i = 0; i < size; i++) {
		std::cout << myVec.at(i) << ' ';
	}
	std::cout << '\n';
}
