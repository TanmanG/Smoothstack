// Prompt 1.

// #include #include <- This will not work as no directive exists named '#include'
// using namespace std; <- This will work, but will warn the user as they are overloading the std namespace.
#include <iostream>
#include <assert.h>
#include <typeinfo>


int main(void) {
	// This will work because 3.0 can be represented with perfect accuracy in floating point numbers.
	assert(3 == 3.0);

	// This will not work because precision is lost on assignment of the float, as it loses some precision
	// with every operation.
	float x = 3.1;
	// assert(x == 3.1);

	// These will work as they are declared in the same method, leaving no opportunity for floating point
	// precision to lower.
	assert (3.1 == 3.1);

	// This will work as 1 can be cleanly represented in a float and 'true' can be evaluated to a 1 or 0 (true or
	// false).
	assert(true == 1.0);

	// This works as ASCII characters/chars have numerical values in the nature of how they are being stored
	// in memory. A char is 1 byte, so the value of 'a' is something between 0 and 255.
	assert('a' == 97);

	// NULL is evaluated to an integral 0, allowing for checks to be made (albeit with warnings).
	int* p = 0;
	assert(NULL == p);
	assert(NULL == 0);
}

// Prompt 2. See separate file.
