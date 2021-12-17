#include <iostream>
#include <cstdlib>


int main() {
	// Generate two matrixes that contain random numbers.
	int matrixA[6][3];
	int matrixB[3][6];

	// Create variables os matrix sizes to clean up code performance and readability.
	int matrixAX = sizeof(matrixA) / sizeof(matrixA[0]);
	int matrixAY = sizeof(matrixA[0]) / sizeof(int);
	int matrixBX = sizeof(matrixB) / sizeof(matrixB[0]);
	int matrixBY = sizeof(matrixB[0]) / sizeof(int);

	if (matrixAX != matrixBY || matrixAY != matrixBX) {
		std::cout << "Matrix dimensions are invalid (Matrix A's height and width must match Matrix B's width and height respectively)!" << '\n';
		return 0;
	}

	// Populate Matrix A and B with random integers.
	// Iterate through each row (Matrix A) and column (Matrix B).
	for (int aRow = 0; aRow < matrixAX; aRow++) {
		// Iterate through each column (Matrix A) and row (Matrix B)
		for (int aCol = 0; aCol < matrixAY; aCol++) {
			matrixA[aCol][aRow] = rand() % 10;
			matrixB[aRow][aCol] = rand() % 10;
		}
	}

	int matrixC[matrixAY][matrixBX];

	// Holder for the current dot product.
	int currTotal = 0;

	// Iterate through each row of Matrix A.
	for (int n = 0; n < matrixAY; n++) {
		// Iterate through each column of Matrix B.
		for (int m = 0; m < matrixBX; m++) {
			// Iterate through each element.
			for (int i = 0; i < matrixAX; i++) {
				currTotal += (matrixA[n][i] * matrixB[i][m]);
			}
		matrixC[n][m] = currTotal;
		currTotal = 0;
		}
	}

	std::cout << "Matrix A" << '\n';
	// Print Matrix A.
	// Iterate through each row.
	for (int aRow = 0; aRow < matrixAY; aRow++) {
		// Iterate through each column.
		for (int aCol = 0; aCol < matrixAX; aCol++) {
			std::cout << matrixA[aRow][aCol] << " ";
		}
		std:: cout << '\n';
	}
	std::cout << '\n';

	std::cout << "Matrix B" << '\n';
	// Print Matrix B.
	// Iterate through each row.
	for (int bRow = 0; bRow < matrixBY; bRow++) {
		// Iterate through each column.
		for (int bCol = 0; bCol < matrixBX; bCol++) {
			std::cout << matrixB[bRow][bCol] << " ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';

	std::cout << "Matrix C" << '\n';
	// Print Matrix C.
	// Iterate through each row.
	for (int cRow = 0; cRow < matrixAY; cRow++) {
		// Iterate through each column.
		for (int cCol = 0; cCol < matrixBX; cCol++) {
			std::cout << matrixC[cRow][cCol] << " ";
		}
		std::cout <<'\n';
	}
	std::cout << '\n';
	return 0;
}

// Homework -> Week2 Day 1, 2, 3, 4 with the next Monday open for evaluation
// Learn to handle under/overflows
