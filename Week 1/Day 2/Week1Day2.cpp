#include <iostream>
#include <string>

int main() {
	// This solution is terrible, hard coding checks like this. Maybe some use of generic types would be possible?

	std::string input;
	std::cin >> input;

	if (input.compare("int") == 0) {
		std::cout << std::to_string(sizeof(int));
	}
	else if (input.compare("double") == 0) {
		std::cout << std::to_string(sizeof(double));
	}
	else if (input.compare("long") == 0) {
		std::cout << std::to_string(sizeof(long));
	}
	return 0;
}
/*
Prompt 2. The hard-coded nature of this solution limits it significantly. A programming language like say, Python,
would be able to freely handle input data without having to declare type, allowing it to assign and measure the
size of the data on runtime rather than requiring a static solution like this.

Prompt 3. The '-g' parameter adds additional debugging information.

Prompt 4. The '-c' parameter only the object code.

Prompt 5. See other file in folder.

*/
