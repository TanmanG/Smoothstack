#include <iostream>
#include <bitset>

#define BYTE_FIVE 5
class Grade {
	public:
		char grade(const int&& score) {
			// Bit value of the @ symbol: 1000000
			// Bit-wise value of 5 is  : 0101
			return ('@' | (BYTE_FIVE - (score / 10)));
		}
};

int main() {
	Grade grade;
	int scores[4] = { 10, 20, 30, 40 };
	for (int i = 0; i < sizeof(scores) / sizeof(int); i++) {
		std::cout << "Score = " << scores[i] << ": " << "Grade " << grade.grade(std::move(scores[i])) << '\n';
	}
}

/* An Explanation:
Given the terms of the assignment (i.e. return a grade A-D given 10-40), we can first reduce the score to a single
digit number (dividing by 10). After, we will have a binary representation of either: 0001, 0010, 0011, or 0100.
We can take these bits and subtract them from five, such that:

5 - 4 = 1
5 - 3 = 2
5 - 2 = 3
5 - 1 = 4

In binary:

0101 - 0100 = 0001
0101 - 0011 = 0010
0101 - 0010 = 0011
0101 - 0001 = 0100


We can then take the character @, who's value is equal to 01000000, and add the remaining bits we have.
This, for example in the first example (5-4=1), then gives us:

 1000000
+0000001
--------
=1000001

Which is the character value of A, meaning we have converted 40 into A.
*/
