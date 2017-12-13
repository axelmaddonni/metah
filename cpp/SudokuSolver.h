#ifndef SOLVER_H
#define SOLVER_H

#include <set>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <array>
#include <iterator>

class SudokuChromosome;

class SudokuSolver {

public:
	SudokuSolver(int sudoku_size, std::vector<int> init_values);

	int solve();

	void initPopulation();

	void reorderPopulation();

	int getBestFitness();

	SudokuChromosome& getBestSolution();

	static bool chromosome_compare (SudokuChromosome& c1, SudokuChromosome& c2);

	int getSudokuSize();

	std::set<int> getAvailableNumbersByZone(int zone);

	std::set<int> getAvailableNumbersByPos(int pos);

	int getRowNumberByPos(int pos);

	int getColNumberByPos(int pos);

	void printGrid(std::vector<int> values);

	int getZoneNumberByPos(int pos);

	std::vector<int> getZoneIndexesByNumber(int zone_number);

	std::set<int> getFixedPositions();

	std::vector<int> getFreePositions();

	std::vector<int> getInitValues();

	int getSize();

private:
	int sudoku_size;
	std::vector<int> init_values;
	std::set<int> fixed_positions;
	std::vector<int> free_positions;

	std::vector<std::set<int>> available_numbers_by_zone;
	std::vector<std::set<int>> available_numbers_by_row;
	std::vector<std::set<int>> available_numbers_by_col;
	std::vector<std::set<int>> available_numbers_by_pos;

	std::vector<SudokuChromosome> population;
	int population_size = 100;
	int elite_size = 1;

	SudokuChromosome* last_sol;
	int last_best_fitness;
};

class SudokuChromosome {
public:

	SudokuChromosome(SudokuSolver* s); 

	SudokuChromosome(SudokuSolver* s, SudokuChromosome p1, SudokuChromosome p2);

	SudokuChromosome(const SudokuChromosome& c);

	void mutate();

	int updateFitness();

	int getFitness();

	std::vector<int> getValues();

	void age();

	bool operator==(SudokuChromosome& c) const;

private:

	int getSampleFromSet(const std::set<int>& s);

	int getSampleFromVector(const std::vector<int>& s);

	SudokuSolver* solver;
	std::vector<int> values;
	int fitness;

	std::vector<std::vector<int>> count_by_row;
	std::vector<std::vector<int>> count_by_col;
};


double getRandomNumber() {
	return ((double) rand() / (RAND_MAX));
}

SudokuSolver::SudokuSolver(int size, std::vector<int> init_values) {
	this->sudoku_size = size;
	this->init_values = init_values;

	fixed_positions = std::set<int>();
	free_positions = std::vector<int>();

	for (int zone = 0; zone < sudoku_size; ++zone) {
		int numbers[] = {1,2,3,4,5,6,7,8,9};
		std::set<int> numbers_set(numbers, numbers + 9);
		available_numbers_by_zone.push_back(numbers_set);
		available_numbers_by_row.push_back(numbers_set);
		available_numbers_by_col.push_back(numbers_set);
	}

	for (int i = 0; i < init_values.size(); ++i) {
		if (init_values[i] == 0) {
			free_positions.push_back(i);
		} else {
			fixed_positions.insert(i);
			available_numbers_by_zone[this->getZoneNumberByPos(i)].erase(init_values[i]);
			available_numbers_by_row[this->getRowNumberByPos(i)].erase(init_values[i]);
			available_numbers_by_col[this->getColNumberByPos(i)].erase(init_values[i]);
		}
	}

	for (int i = 0; i < init_values.size(); ++i) {
		if (init_values[i] != 0) {
			available_numbers_by_pos.push_back(std::set<int>());
		} else {
			std::set<int> intersection;
			std::set<int> intersection_aux;

			std::set_intersection(
				available_numbers_by_row[this->getRowNumberByPos(i)].begin(),
				available_numbers_by_row[this->getRowNumberByPos(i)].end(),
				available_numbers_by_col[this->getColNumberByPos(i)].begin(),
				available_numbers_by_col[this->getColNumberByPos(i)].end(),
				std::inserter(intersection_aux, intersection_aux.begin()));

			std::set_intersection(
				intersection_aux.begin(), intersection_aux.end(),
				available_numbers_by_zone[this->getZoneNumberByPos(i)].begin(),
				available_numbers_by_zone[this->getZoneNumberByPos(i)].end(),
				std::inserter(intersection, intersection.begin()));

			available_numbers_by_pos.push_back(intersection);
		}
	}
}

int SudokuSolver::solve() {
	srand(time(NULL));

	// std::cout << "Initial Grid \n";
	// this->printGrid(this->init_values);

	this->initPopulation();
	int iter_count = 0;
	int iters_without_improvement = 0;
	int best_fitness = this->getBestFitness();

	while (this->getBestFitness() > 0 and iter_count < 100000) {

		int new_fitness = this->getBestFitness();
		if (new_fitness < best_fitness) {
			best_fitness = new_fitness;
			// std::cout << "iteracion: " << iter_count << " best_fitness:" << best_fitness << std::endl;
			iters_without_improvement = 0;
			// this->printGrid(this->getBestSolution().getValues());

		} else {
			iters_without_improvement++;
		}

		if (iters_without_improvement == 2000) {
			std::cout << "Restarting..." << std::endl;
			this->initPopulation();
			iters_without_improvement = 0;
			best_fitness = this->getBestFitness();

		} else {
			for (int i = this->population_size-1; i >= elite_size; i--) {

				bool repeated = true;
				int intentos = 0;

				while (repeated) {

					int p1_index = (int)(i * getRandomNumber());
					int p2_index = (int)(i * getRandomNumber());

					SudokuChromosome child (this, population[p1_index], this->population[p2_index]);
					child.mutate();

					repeated = false;
					for (auto p : population) {
						if (p == child) {
							repeated = true;
							intentos++;
							break;
						}
					}

					if (intentos == 3) {
						child = SudokuChromosome(this);
						repeated = false;
					}

					if (not repeated) {
						population[i] = child;
					}
				}
			}

			reorderPopulation();
		}
		iter_count++;
	}

	if (iter_count == 100000) {
		return 0;
	}

	return iter_count;
}

void SudokuSolver::initPopulation() {
	population = std::vector<SudokuChromosome>();

	for (int i = 0; i < population_size; ++i) {

		SudokuChromosome new_individual(this);
		bool repeated = true;

		while (repeated) {
			repeated = false;
			for (auto p : population) {
				if (p == new_individual) {
					repeated = true;
					break;
				}
			}
		}
		population.push_back(new_individual);
	}
}

void SudokuSolver::reorderPopulation() {
	if (this->last_sol != NULL and *(this->last_sol) == this->getBestSolution()) {
		this->getBestSolution().age();
	}
	std::sort (population.begin(), population.end(), chromosome_compare);
	this->last_sol = new SudokuChromosome(this->getBestSolution());
	this->last_best_fitness = this->last_sol->getFitness();
}

int SudokuSolver::getBestFitness() {
	return this->getBestSolution().getFitness();
}

SudokuChromosome& SudokuSolver::getBestSolution() {
	return population[0];
}

bool SudokuSolver::chromosome_compare (SudokuChromosome& c1, SudokuChromosome& c2) {
	return c1.getFitness() < c2.getFitness();
}

int SudokuSolver::getSudokuSize() {
	return sudoku_size;
}

std::set<int> SudokuSolver::getAvailableNumbersByZone(int zone) {
	return available_numbers_by_zone[zone];
};

std::set<int> SudokuSolver::getAvailableNumbersByPos(int pos) {
	return available_numbers_by_pos[pos];
};

int SudokuSolver::getRowNumberByPos(int pos) {
	return (int) pos / sudoku_size;
}

int SudokuSolver::getColNumberByPos(int pos) {
	return pos % sudoku_size;
}

void SudokuSolver::printGrid(std::vector<int> values) {
	// For each row
	for (int y=0; y<9; y++) {
		// Draw horizontal lines between blocks
		if (y % 3== 0) {
			std::cout << "-------------------------------" << std::endl;
		}
		
		// For each cell in row
		for (int x=0; x<9; x++) {
			if (x % 3 == 0) {
				std::cout << "|";
			}
		
			if (values[y*9 + x] != 0) {
				std::cout << " " << values[y*9 +x] << " ";
			} else {
				std::cout << " . ";
			}
			
		}
		// Draw columns between blocks
		std::cout << "|" << std::endl;
	}
	std::cout << "-------------------------------" << std::endl;
}

int SudokuSolver::getZoneNumberByPos(int pos) {
	int row_number = this->getRowNumberByPos(pos);
	int col_number = this->getColNumberByPos(pos);
	if (row_number % 9 < 3) {
		if (col_number % 9 < 3) {
			return 0;
		}
		else if (col_number % 9 < 6) {
			return 1;
		}
		else {
			return 2;
		}
	} else if (row_number % 9 < 6) {
		if (col_number % 9 < 3) {
			return 3;
		}
		else if (col_number % 9 < 6) {
			return 4;
		}
		else {
			return 5;
		}
	} else {
		if (col_number % 9 < 3) {
			return 6;
		}
		else if (col_number % 9 < 6) {
			return 7;
		}
		else {
			return 8;
		}
	}
}

std::vector<int> SudokuSolver::getZoneIndexesByNumber(int zone_number) {
	int first = sudoku_size * (int)(zone_number/3) * 3 + zone_number % 3 * 3;
	std::vector<int> indexes;
	for (int i = 0; i < 3; ++i) {
		indexes.push_back(first);
		indexes.push_back(first + 1);
		indexes.push_back(first + 2);
		first += sudoku_size;
	}
	return indexes;
}

int SudokuSolver::getSize() {
	return sudoku_size;
}

std::set<int> SudokuSolver::getFixedPositions() {
	return fixed_positions;
}

std::vector<int> SudokuSolver::getFreePositions() {
	return free_positions;
}

std::vector<int> SudokuSolver::getInitValues() {
	return init_values;
}


SudokuChromosome::SudokuChromosome(SudokuSolver* s) {
	// Crea individuo random
	solver = s;
	this->values = std::vector<int>(81);

	for (int z = 0; z < solver->getSudokuSize(); z++) {

		std::set<int> numbers = solver->getAvailableNumbersByZone(z);

		for (int pos : solver->getZoneIndexesByNumber(z)) {

			if (solver->getFixedPositions().count(pos) != 0) {
				this->values[pos] = solver->getInitValues()[pos];
			} else {
				this->values[pos] = getSampleFromSet(numbers);
				numbers.erase(this->values[pos]);
			}
		}
	}

	this->updateFitness();
}

SudokuChromosome::SudokuChromosome(SudokuSolver* s, SudokuChromosome p1, SudokuChromosome p2) {
	// Crea individuo a partir de 2 padres (crossover)
	solver = s;
	this->values = std::vector<int>(81);

	for (int z = 0; z < solver->getSudokuSize(); z++) {
		SudokuChromosome* chosen;
		if (getRandomNumber() <= 0.5) {
			chosen = &p1;
		} else {
			chosen = &p2;
		}

		for (int pos : solver->getZoneIndexesByNumber(z)) {
			this->values[pos] = chosen->getValues()[pos];
		}
	}

	this->updateFitness();
}

std::vector<int> SudokuChromosome::getValues() {
	return this->values;
}

int SudokuChromosome::getSampleFromSet(const std::set<int>& s) {
	double r = rand() % s.size();
	std::set<int>::const_iterator it(s.begin());
	advance(it,r);
	return *it;
}

int SudokuChromosome::getSampleFromVector(const std::vector<int>& s) {
	double r = rand() % s.size();
	std::vector<int>::const_iterator it(s.begin());
	advance(it,r);
	return *it;
}

 // Version de Mantere y Koljonen 
void SudokuChromosome::mutate() {

	if (getRandomNumber() > 0.6) {
		return;
	}

	int zone_number = rand() % 9; 

	std::vector<int> indexes = solver->getZoneIndexesByNumber(zone_number);

	int index1 = getSampleFromVector(indexes);
	int index2 = getSampleFromVector(indexes);

	while (index1 == index2
		or solver->getFixedPositions().count(index1) != 0
		or solver->getFixedPositions().count(index2) != 0 ) {

		index1 = getSampleFromVector(indexes);
		index2 = getSampleFromVector(indexes);
	}

	/* Esta mejora de Mantere no la replicamos
	count_by_row[solver->getRowNumberByPos(index1)][this->values[index2]-1] +
		count_by_col[solver->getColNumberByPos(index1)][this->values[index2]-1] <= 2 and
		count_by_row[solver->getRowNumberByPos(index2)][this->values[index1]-1] +
		count_by_col[solver->getColNumberByPos(index2)][this->values[index1]-1] <= 2
	*/

	// int tmp = this->values[index1];
	// this->values[index1] = this->values[index2];
	// this->values[index2] = tmp;

	// updateFitness();

	if (solver->getAvailableNumbersByPos(index1).count(this->values[index2]) != 0 and
		solver->getAvailableNumbersByPos(index2).count(this->values[index1]) != 0) {
		int tmp = this->values[index1];
		this->values[index1] = this->values[index2];
		this->values[index2] = tmp;

		updateFitness();
	}
}

/*
void SudokuChromosome::mutate() {

	std::vector<int> indexes = solver->getFreePositions();
	std::random_shuffle(std::begin(indexes), std::end(indexes));

	for (int i : indexes) {
		if (getRandomNumber() <= 0.2) {
			this->values[i] = getSampleFromSet(solver->getAvailableNumbersByPos(i));
		}
	}

	updateFitness();
}
*/

int SudokuChromosome::updateFitness() {
	this->fitness = 0;

	this->count_by_row = std::vector<std::vector<int>>(solver->getSize(), std::vector<int>(solver->getSize(), 0));
	this->count_by_col = std::vector<std::vector<int>>(solver->getSize(), std::vector<int>(solver->getSize(), 0));

	for (int row = 0; row < solver->getSudokuSize() ; row++) {

		int numbers[] = {1,2,3,4,5,6,7,8,9};
		std::set<int> numbers_set(numbers, numbers + 9);

		for (int i = row * 9; i < row*9 + 9; i++) {
			numbers_set.erase(this->values[i]);
			count_by_row[row][this->values[i]-1]++;
		}
		this->fitness = this->fitness + numbers_set.size();
	}

	for (int col = 0; col < solver->getSudokuSize() ; col++) {

		int numbers[] = {1,2,3,4,5,6,7,8,9};
		std::set<int> numbers_set(numbers, numbers + 9);
		for (int i = col; i < 73 + col; i = i+9) {
			numbers_set.erase(this->values[i]);
			count_by_col[col][this->values[i]-1]++;
		}
		this->fitness = this->fitness + numbers_set.size();
	}

	return this->fitness;
}

int SudokuChromosome::getFitness() {
	return this->fitness;
}

SudokuChromosome::SudokuChromosome(const SudokuChromosome& c) {
	solver = c.solver;
	values = c.values;
	fitness = c.fitness;
}

bool SudokuChromosome::operator==(SudokuChromosome& c) const {
	return this->values == c.getValues();
}

void SudokuChromosome::age() {
	fitness++;
}

#endif