#include <iostream>
#include <cstdlib>


int main() {
	// Generate two matrixes that contain random numbers.
	int matrixA[3][6];
	int matrixB[6][3];

	for (int i = 0; i < sizeof(matrixA) / sizeof(matrixA[0]); i++) {
		for (int h = 0; h < sizeof(matrixA) / sizeof(int); h++) {
			matrixA[i][h] = rand() % 10;
		}
	}

	// Create a third matrix of the final size.
	int matrixC[sizeof(matrixA) / sizeof(matrixA[0])][sizeof(matrixB[0]) / sizeof(int)];

	// calculate dot product.
	int currTotal = 0;
	// Iterate through each row in matrix A.
	for (size_t matrixARow = 0; matrixARow < sizeof(matrixA) / sizeof(matrixA[0]); matrixARow++) {
		// Iterate through each column in matrix B.
		for (size_t matrixBColumn = 0; matrixBColumn < sizeof(matrixB[0]) / sizeof(int); matrixBColumn++) {
			// Iterate through each element within matrix A and B.
			for (size_t element = 0; element < sizeof(matrixA) / sizeof(matrixA[0]); element++) {
				currTotal += matrixA[element][matrixARow] * matrixB[matrixBColumn][element];
			}
			matrixC[matrixARow][matrixBColumn] = currTotal;
			currTotal = 0;
		}
	}
	std::cout << matrixA[0][0] << '\n';
	return 0;
}

// Homework -> Week2 Day 1, 2, 3, 4 with the next Monday open for evaluation
// Learn to handle under/overflows
