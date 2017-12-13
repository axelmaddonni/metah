#include "SudokuSolver.h"

#include <ctime>
#include <map>
#include <cstdlib>

std::vector<int> getSudokuExample(int difficulty) {

/*
	Sudokus utilizados para testear segun nivel de dificultad
	1 1 Estrella
	2 2 Estrellas
	3 3 Estrellas
	4 4 Estrellas
	5 5 Estrellas
	6 Easy
	7 Challenging
	8 Difficult
	9 Super Difficult
	10 Easy
	11 Medium
	12 Hard
	Descargados de: http://lipas.uwasa.fi/~timan/sudoku/
*/
	std::map<int, std::vector<int>> const sudokus = {
		{2, {2,0,6,0,0,0,0,4,9,0,3,7,0,0,9,0,0,0,1,0,0,7,0,0,0,0,6,0,0,0,5,8,0,9,0,0,7,0,5,0,0,0,8,0,4,0,0,9,0,6,2,0,0,0,9,0,0,0,0,4,0,0,1,0,0,0,3,0,0,4,9,0,4,1,0,0,0,0,2,0,8} },
		{5, {1,0,5,0,0,0,3,7,0,0,0,0,0,0,0,2,0,0,0,9,7,3,0,0,0,1,0,0,0,0,0,5,3,1,0,2,3,0,0,8,0,1,0,0,4,2,0,1,4,7,0,0,0,0,0,7,0,0,0,8,6,4,0,0,0,8,0,0,0,0,0,0,0,1,2,0,0,0,8,0,7} },
		{7, {0,0,2,0,0,4,0,0,0,0,0,8,6,0,0,0,0,0,0,9,0,0,0,0,0,3,0,8,0,0,0,6,2,0,0,0,1,4,0,0,3,0,5,0,9,0,7,0,0,0,0,0,0,0,0,3,5,0,0,8,6,0,7,0,0,0,0,0,0,0,4,2,0,0,0,9,0,1,0,0,3} },
		{8, {6,7,0,0,0,8,0,1,0,0,2,0,0,6,0,0,0,0,0,0,0,0,3,0,0,0,0,2,0,1,0,0,0,0,0,6,4,8,0,0,0,1,7,0,0,0,0,0,0,0,0,0,0,9,0,0,4,5,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,3,4,0,0,8,0,2} },
		{9, {0,9,0,3,8,4,0,0,0,0,0,2,0,7,0,0,0,0,0,0,0,0,0,0,0,7,1,5,0,0,0,0,3,2,4,0,0,3,0,0,0,0,0,0,0,0,0,1,0,0,5,0,9,0,0,0,0,8,0,0,0,0,0,7,0,6,5,2,0,0,0,0,0,0,0,0,0,6,4,0,0} },
		{11, {0,0,0,0,0,0,0,0,0,0,7,9,0,5,0,1,8,0,8,0,0,0,0,0,0,0,7,0,0,7,3,0,6,8,0,0,4,5,0,7,0,8,0,9,6,0,0,3,5,0,2,7,0,0,7,0,0,0,0,0,0,0,5,0,1,6,0,3,0,4,2,0,0,0,0,0,0,0,0,0,0} }
	};

	return sudokus.at(difficulty);
}


int main(int argc, const char* argv[]) {

	int difficulty = 9;
	if (argc >= 2) {
		difficulty = atoi(argv[1]);
	}

	std::vector<int> values = getSudokuExample(difficulty);

	std::vector<double> times;
	std::vector<int> iters;

	for (int i = 0; i < 100; ++i)
	{

		SudokuSolver s(9, values);

		clock_t begin = clock();

		int res = s.solve();

		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		if (res > 0) {
			std::cout << "Comenzando sudoku numero " << i << std::endl;
			printf("Se resolvio el sudoku en %.2f segundos!!!!!\n", elapsed_secs);
			// s.printGrid(s.getBestSolution().getValues());
			times.push_back(elapsed_secs);
			iters.push_back(res);

		} else {
			printf("No se resolvio el sudoku luego de %.2f segundos :( \n", elapsed_secs);
			s.printGrid(s.getBestSolution().getValues());
		}

	}

	std::cout << "Se resolvieron " << times.size() << " de 100 sudokus." << std::endl;

	std::cout << "Tiempos" << std::endl;
	std::cout << "[";
	for (int i = 0; i < times.size(); ++i)
	{
		std::cout << times[i] << ",";
	}
	std::cout << "]" << std::endl;

	std::cout << "Iteraciones" << std::endl;
	std::cout << "[";
	for (int i = 0; i < iters.size(); ++i)
	{
		std::cout << iters[i] << ",";
	}
	std::cout << "]" << std::endl;

	return 0;

}


