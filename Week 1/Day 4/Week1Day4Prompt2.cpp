// Prompt 2.
#include <iostream>
#include <typeinfo>

int main() {
	float xFloat = 1.0;
	int xInt = 1;
	char xChar = 'a'; // Value is 97
	bool xBool = true;

	// Any of the operations involving float are casted into floats with the rest casting into integers.
	std::cout << "Type of Float + Int: "  << typeid(xFloat + xInt).name() << '\n';
	std::cout << "Type of Float + Char: " << typeid(xFloat + xChar).name() << '\n';
	std::cout << "Type of Float + Bool: " << typeid(xFloat + xBool).name() << '\n';
	std::cout << "Type of Int + Char: " << typeid(xInt + xChar).name() << '\n';
	std::cout << "Type of Int + Bool: " << typeid(xInt + xBool).name() << '\n';
	std::cout << "Type of Char + Bool: " << typeid(xChar + xBool).name() << '\n';
}

/*
Prompt 3.
Unary (One operand): ++, --, - (negative), sizeof(), ! (not), Address (&)
Binary (Two operands): +, -, *, /, % (mod.)
Ternary (Three operands): ? and : (conditional operator)
*/
