#include "SudokuSolver.h"

#include <ctime>
#include <map>
#include <cstdlib>

std::vector<int> getSudokuExample(int difficulty) {

/*
	Sudokus utilizados para testear segun nivel de dificultad
	1 1 Estrella A
	2 2 Estrellas A
	3 3 Estrellas A
	4 4 Estrellas A
	5 5 Estrellas A
	6 Easy A
	7 Challenging A
	8 Difficult A
	9 Super Difficult A
	10 Easy A
	11 Medium A 
	12 Hard A
	13 Easy B
	14 Medium B
	15 Hard B
	Descargados de: http://lipas.uwasa.fi/~timan/sudoku/
*/
	std::map<int, std::vector<int>> const sudokus = {
		{1, {0,4,0,0,0,0,1,7,9,0,0,2,0,0,8,0,5,4,0,0,6,0,0,5,0,0,8,0,8,0,0,7,0,9,1,0,0,5,0,0,9,0,0,3,0,0,1,9,0,6,0,0,4,0,3,0,0,4,0,0,7,0,0,5,7,0,1,0,0,2,0,0,9,2,8,0,0,0,0,6,0} },
		{2, {2,0,6,0,0,0,0,4,9,0,3,7,0,0,9,0,0,0,1,0,0,7,0,0,0,0,6,0,0,0,5,8,0,9,0,0,7,0,5,0,0,0,8,0,4,0,0,9,0,6,2,0,0,0,9,0,0,0,0,4,0,0,1,0,0,0,3,0,0,4,9,0,4,1,0,0,0,0,2,0,8} },
		{3, {0,5,0,2,0,0,0,0,0,3,0,0,0,0,5,0,8,0,9,6,0,0,7,8,2,0,0,0,0,0,0,3,0,0,2,0,7,0,8,0,0,0,1,0,3,0,4,0,0,8,0,0,0,0,0,0,1,6,4,0,0,3,2,0,7,0,5,0,0,0,0,1,0,0,0,0,0,9,0,5,0} },
		{4, {0,5,0,0,9,0,0,0,0,0,0,4,8,0,0,0,0,9,0,0,0,1,0,7,2,8,0,5,6,0,0,0,0,1,3,7,0,0,0,0,0,0,0,0,0,1,7,3,0,0,0,0,4,2,0,2,1,5,0,8,0,0,0,6,0,0,0,0,3,8,0,0,0,0,0,0,1,0,0,6,0} },
		{5, {1,0,5,0,0,0,3,7,0,0,0,0,0,0,0,2,0,0,0,9,7,3,0,0,0,1,0,0,0,0,0,5,3,1,0,2,3,0,0,8,0,1,0,0,4,2,0,1,4,7,0,0,0,0,0,7,0,0,0,8,6,4,0,0,0,8,0,0,0,0,0,0,0,1,2,0,0,0,8,0,7} },
		{6, {0,0,8,0,6,0,9,0,0,0,0,0,2,0,3,6,7,8,7,0,6,0,5,1,0,0,4,9,7,3,0,4,8,1,0,0,6,2,0,0,3,9,0,5,0,0,0,1,7,0,0,0,0,0,5,8,0,9,0,0,3,0,6,0,0,0,0,0,0,0,0,0,0,4,0,0,0,5,7,2,1} },
		{7, {0,0,2,0,0,4,0,0,0,0,0,8,6,0,0,0,0,0,0,9,0,0,0,0,0,3,0,8,0,0,0,6,2,0,0,0,1,4,0,0,3,0,5,0,9,0,7,0,0,0,0,0,0,0,0,3,5,0,0,8,6,0,7,0,0,0,0,0,0,0,4,2,0,0,0,9,0,1,0,0,3} },
		{8, {6,7,0,0,0,8,0,1,0,0,2,0,0,6,0,0,0,0,0,0,0,0,3,0,0,0,0,2,0,1,0,0,0,0,0,6,4,8,0,0,0,1,7,0,0,0,0,0,0,0,0,0,0,9,0,0,4,5,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,3,4,0,0,8,0,2} },
		{9, {0,9,0,3,8,4,0,0,0,0,0,2,0,7,0,0,0,0,0,0,0,0,0,0,0,7,1,5,0,0,0,0,3,2,4,0,0,3,0,0,0,0,0,0,0,0,0,1,0,0,5,0,9,0,0,0,0,8,0,0,0,0,0,7,0,6,5,2,0,0,0,0,0,0,0,0,0,6,4,0,0} },
		{10, {0,0,2,0,0,0,5,0,0,0,1,0,7,0,5,0,2,0,4,0,0,0,9,0,0,0,7,0,4,9,0,0,0,7,3,0,8,0,1,0,3,0,4,0,9,0,3,6,0,0,0,2,1,0,2,0,0,0,8,0,0,0,4,0,8,0,9,0,2,0,6,0,0,0,7,0,0,0,8,0,0} },
		{11, {0,0,0,0,0,0,0,0,0,0,7,9,0,5,0,1,8,0,8,0,0,0,0,0,0,0,7,0,0,7,3,0,6,8,0,0,4,5,0,7,0,8,0,9,6,0,0,3,5,0,2,7,0,0,7,0,0,0,0,0,0,0,5,0,1,6,0,3,0,4,2,0,0,0,0,0,0,0,0,0,0} },
		{12, {0,0,0,0,0,3,0,1,7,0,1,5,0,0,9,0,0,8,0,6,0,0,0,0,0,0,0,1,0,0,0,0,7,0,0,0,0,0,9,0,0,0,2,0,0,0,0,0,5,0,0,0,0,4,0,0,0,0,0,0,0,2,0,5,0,0,6,0,0,3,4,0,3,4,0,2,0,0,0,0,0} },
		{13, {0,5,0,0,1,0,0,4,0,1,0,7,0,0,0,6,0,2,0,0,0,9,0,5,0,0,0,2,0,8,0,3,0,5,0,1,0,4,0,0,7,0,0,2,0,9,0,1,0,8,0,4,0,6,0,0,0,4,0,1,0,0,0,3,0,4,0,0,0,7,0,9,0,2,0,0,6,0,0,1,0} },
		{14, {0,0,0,0,0,0,0,8,5,0,0,0,2,1,0,0,0,9,9,6,0,0,8,0,1,0,0,5,0,0,8,0,0,0,1,6,0,0,0,0,0,0,0,0,0,8,9,0,0,0,6,0,0,7,0,0,9,0,7,0,0,5,2,3,0,0,0,5,4,0,0,0,4,8,0,0,0,0,0,0,0} },
		{15, {3,8,0,0,0,0,0,0,0,0,0,0,4,0,0,7,8,5,0,0,9,0,2,0,3,0,0,0,6,0,0,9,0,0,0,0,8,0,0,3,0,2,0,0,9,0,0,0,0,4,0,0,7,0,0,0,1,0,7,0,5,0,0,4,9,5,0,0,6,0,0,0,0,0,0,0,0,0,0,9,2} },
	};

	return sudokus.at(difficulty);
}

void test_solver(std::vector<int> values, int pop_size, int elite_size, double mutation_proba, int max_iters) {

	// std::cout << "Corriendo SudokuSolver con";
	// std::cout << " population_size: " << pop_size;
	// std::cout << " elite_size: " << elite_size;
	// std::cout << " mutation_proba: " << mutation_proba;
	// std::cout << " max_iters: " << max_iters;
	// std::cout << std::endl;

	std::vector<double> times;
	std::vector<int> iters;

	for (int i = 0; i < 100; ++i)
	{

		SudokuSolver s(9, values);

		// std::cout << "Comenzando sudoku numero " << i << std::endl;
		clock_t begin = clock();

		int res = s.solve(pop_size, elite_size, mutation_proba, max_iters);

		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		if (res > 0) {
			// printf("Se resolvio el sudoku en %.2f segundos!!!!!\n", elapsed_secs);
			// s.printGrid(s.getBestSolution().getValues());
			times.push_back(elapsed_secs);
			iters.push_back(res);

		} else {
			// printf("No se resolvio el sudoku luego de %.2f segundos :( \n", elapsed_secs);
			s.printGrid(s.getBestSolution().getValues());
		}

	}

	std::cout << "Se resolvieron " << times.size() << " de 100 sudokus." << std::endl;

	std::cout << "Tiempos" << std::endl;
	std::cout << "[";

	double avg_tiempos = 0.0;
	for (int i = 0; i < times.size(); ++i)
	{
		std::cout << times[i] << ",";
		avg_tiempos = avg_tiempos + times[i];
	}
	std::cout << "]" << std::endl;

	double avg_iteraciones = 0.0;
	std::cout << "Iteraciones" << std::endl;
	std::cout << "[";
	for (int i = 0; i < iters.size(); ++i)
	{
		std::cout << iters[i] << ",";
		avg_iteraciones = avg_iteraciones + iters[i];
	}
	std::cout << "]" << std::endl;

	avg_tiempos = avg_tiempos / 30;
	avg_iteraciones = avg_iteraciones / 30;

	std::cout << "Promedio tiempos: " << avg_tiempos << std::endl;
	std::cout << "Promedio iteraciones: " << avg_iteraciones << std::endl;
}

int main(int argc, const char* argv[]) {

	
	int pop_size = 100;
	int elite_size = 5;
	double mutation_proba = 0.6;
	int max_iters = 300;

	for (int difficulty = 1; difficulty <= 9; ++difficulty)
	{
		std::cout << "Corriendo SudokuSolver con dificultad " << difficulty << std::endl;
		std::vector<int> values = getSudokuExample(difficulty);
		test_solver(values, pop_size, elite_size, mutation_proba, max_iters);
	}

	return 0;
}
