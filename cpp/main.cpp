#include "SudokuSolver.h"

#include <ctime>

int main(int argc, const char* argv[]) {

	static const int arr[] = {0,9,0,3,8,4,0,0,0,0,0,2,0,7,0,0,0,0,0,0,0,0,0,0,0,7,1,5,0,0,0,0,3,2,4,0,0,3,0,0,0,0,0,0,0,0,0,1,0,0,5,0,9,0,0,0,0,8,0,0,0,0,0,7,0,6,5,2,0,0,0,0,0,0,0,0,0,6,4,0,0};
	std::vector<int> values (arr, arr + sizeof(arr) / sizeof(arr[0]) );

	printf("a crear sudoku solver\n");

	SudokuSolver s(9, values);

	printf("a resolver sudoku\n");

	clock_t begin = clock();

	int res = s.solve();

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	if (res) {
		printf("Se resolvio el sudoku en %.2f!!!!!\n", elapsed_secs);
		s.printGrid(s.getBestSolution());
	}

	return 0;

}