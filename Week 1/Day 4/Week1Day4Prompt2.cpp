#include <iostream>
#include <typeinfo>
#include <string>

int main() {
	// Prompt 2
	float xFloat = 1.0;
	int xInt = 1;
	char xChar = 'a'; // Value is 97
	bool xBool = true;

	// Any of the operations involving float are casted into floats with the rest casting into integers.
	std::cout << "----- " << "F " << "I " <<  "C " << "B" << '\n';
	std::cout << "Float "  << typeid(xFloat + xFloat).name() << " " << typeid(xFloat + xInt).name() << " "
				<< typeid(xFloat + xChar).name() << " " << typeid(xFloat + xBool).name() << '\n';
	std::cout << "Int   " << typeid(xInt + xFloat).name() << " " << typeid(xInt + xInt).name() << " "
				<< typeid(xInt + xChar).name() << " " << typeid(xInt +xBool).name() << '\n';
	std::cout << "Char  " << typeid(xChar + xFloat).name() << " " << typeid(xChar + xInt).name() << " "
				<< typeid(xChar + xChar).name() << " " << typeid(xChar + xBool).name() << '\n';
	std::cout << "Bool  " << typeid(xBool + xFloat).name() << " " << typeid(xBool + xInt).name() << " "
				<< typeid(xBool + xChar).name() << " " << typeid(xBool + xBool).name() << '\n';

	// Prompt 5
	bool tfAND = (true && false);
	bool tfOR = (true || false);
	bool ttAND = (true && true);
	bool ttOR = (true && true);
	bool ffAND = (false && false);
	bool ffOR = (false || false);
	bool tNOT = (!true);
	bool fNOT = (!false);

	std::cout << "----- " << "T " << "F" << '\n';
	std::cout << "T-And " << ttAND << " " << tfAND << '\n';
	std::cout << "F-And " << tfAND << " " << ffAND << '\n';
	std::cout << "T-Or  " << ttOR << " " << tfOR << '\n';
	std::cout << "F-Or  " << tfOR << " " << ffOR << '\n';
	std::cout << "Not   " << tNOT << " " << fNOT << '\n';
}

/*
Prompt 3.
Unary (One operand): ++, --, - (negative), sizeof(), ! (not), Address (&)
Binary (Two operands): +, -, *, /, % (mod.)
Ternary (Three operands): ? and : (conditional operator)

Prompt 4. See Prompt 4 file.
*/
