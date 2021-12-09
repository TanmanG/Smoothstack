#include <iostream>
#include <cstdint>

int main() {
	// Declare and initialize test variables.
	int x1 = 1;
	float x2 = 3.2525f;

	// Create a void pointer array to store the references to the values.
	void *test[2] = { &x1, &x2 };

	// There is no way of knowing what the original data-type was for re-casting, making this impractical.
	// Only if you know the data-type before-hand can you cast the pointer and dereference to the correct type.
	// Just use a vector of type <std::variant<types>> to hold multiple types.
	std::cout << *(float*)test[1] << '\n';
	std::cout << *(int*)test[0] << '\n';

	// This will display the float equivalent to the data used to store the int '1'.
	// This reminds me of an issue I've encountered in a personal project attempting to read an HTTP request
	// on a Python socket server. I couldn't figure out the encoding of the incoming request so it'd only come
	// out as a garbled mess of various ascii/utf-8/etc characters.
	std::cout << *(float*)test[0] << '\n';
}

// Prompt 3A. Arrays are great choices for working with fixed data sizes (as they maintain high efficiency),
// while vectors are a better choice for more dynamic sets of information (as they can grow/shrink).
// Due to the limited and structured nature of Arrays, they are easy to sort and always have perfect random access
// efficiency. Their downside is that they are immutable in size, can only store one type, and always uses a fixed
// number of elements (i.e. the declared size of the array).

// A good time to use an array would be storing names associated with a range of phone numbers.
// A good time to use a vector would be tracking employee IDs as they enter/exit a building, adding and removing
// them from the vector dynamically.

// Prompt 4. push_back() is extremely efficient as it makes use of vector::back, which is a pointer to the back-most
// element. Furthermore, there is no re-shuffling of other elements required freeing the efficiency to O(1).

