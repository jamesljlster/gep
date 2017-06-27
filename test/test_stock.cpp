#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <omp.h>

#include <csv_dataproc.h>
#include <galib.hpp>
#include <evo.hpp>
#include <gep.hpp>

#define RAND_PRECISION	1000
#define RAND_NUM_MAX	1
#define RAND_NUM_MIN	-1
#define RAND_EXP_MAX	3
#define RAND_EXP_MIN	-3

#define LEVEL	5

#define MUT_RATE		0.01
#define ITER_LIMIT		30000
#define ANALY_ITER		100
#define RESTART			1000
#define RESTART_ADD		10
#define POOL_WEIGHT		10

#define RANGE_MIN	0
#define RANGE_MAX	10
#define RANGE_STEP	0.1

#define TRA_CSV_PATH	"stock-5-1tra.csv"
#define TST_CSV_PATH	"stock-5-1tst.csv"

#define DEBUG

using namespace std;
using namespace gep;
using namespace evo;

double fitness_single(vector<union GEP_NODE> chro, void* arg);
double fitness(vector<union GEP_NODE> chro, void* arg);

vector<int> gen_rand_index_vector(int size, int swap);
vector<char> gen_crossover_mask(int size, int swap);

int main()
{
	srand(time(NULL));

	// Prepare dataset
	int iResult;
	csv_t tra = NULL;
	csv_t tst = NULL;

	iResult = csv_read(&tra, TRA_CSV_PATH);
	if(iResult < 0)
	{
		cout << "Failed to read " << TRA_CSV_PATH << endl;
		return -1;
	}

	iResult = csv_read(&tst, TST_CSV_PATH);
	if(iResult < 0)
	{
		cout << "Failed to read " << TST_CSV_PATH << endl;
		return -1;
	}

	int inputSize = tra->cols - 1;

	// Create ga pool
	int chroLen = (1 << LEVEL) - 1;
	int headSize = (1 << (LEVEL - 1)) - 1;
	gaPool<union GEP_NODE> ga;
	ga.set_chro_len(chroLen);

	// Insert random chromosomes to ga pool
	struct GEP_RAND randSet = {RAND_NUM_MAX, RAND_NUM_MIN, RAND_EXP_MAX, RAND_EXP_MIN, RAND_PRECISION};
	for(int i = 0; i < chroLen * POOL_WEIGHT; i++)
	{
		ga.insert(gep_rand_chro(randSet, LEVEL, inputSize));
	}
	
	int limitPoolSize = ga.pool_size();

	// Run task
	double mutRate = MUT_RATE;
	int counter = 0;
	double tmpFitness = 0;
	while(counter++ < ITER_LIMIT)
	{
		//if(counter % ANALY_ITER == 0)
		{
			ga.order(fitness, 1, (void*)tra);
			tmpFitness = fitness(ga.get_chro(0), (void*)tra);
			cout << "Iter " << counter <<  ", mse: " <<  tmpFitness << endl;
			gep_print_chro(ga.get_chro(0), cout);
			mutRate = MUT_RATE * (tmpFitness / (double)(counter * counter));
		}

		for(int turn = 0; turn < tra->rows; turn++)
		{
			// Restart
			if(counter % RESTART == 0)
			{
				ga.remove_same_chro();
				for(int i = ga.pool_size(); i < limitPoolSize * 2; i++)
				{
					ga.insert(gep_rand_chro(randSet, LEVEL, inputSize));
				}
			}

			// Generate random index array
			int poolSize = ga.pool_size();
			vector<int> randIndex = gen_rand_index_vector(poolSize, poolSize * poolSize / 2);

			// Crossover
			/*
			for(int i = 0; i < poolSize; i++)
			{
				ga.crossover(randIndex[i], randIndex[(i + 1) % poolSize], rand() % (chroLen - 1));
			}
			*/

			for(int i = 0; i < poolSize; i++)
			{
				vector<char> mask = gen_crossover_mask(chroLen, chroLen * chroLen / 2);
				ga.insert(crossover_mask(
							mask,
							ga.get_chro(randIndex[i]),
							ga.get_chro(randIndex[(i + 1) % poolSize])));
			}

			// Mutation
			for(int i = 0; i < poolSize; i++)
			{
				for(int j = 0; j < chroLen; j++)
				{
					if(rand() % 100 < mutRate * 100)
					{
						union GEP_NODE tmpNode;

						/*
						if(j < 1)
						{
							tmpNode = gep_rand_op();
						}
						else if(j < headSize)
						{
							int randTmp = rand() % 3;
							switch(randTmp)
							{
								case 0:
									tmpNode = gep_rand_op();
									break;

								case 1:
									tmpNode = gep_rand_terminal(randSet);
									break;

								case 2:
									tmpNode = gep_rand_variable(inputSize);
									break;
							}
						}
						*/
						if(j < headSize)
						{
							tmpNode = gep_rand_op();
						}
						else
						{
							if(rand() % 2 == 0)
							{
								tmpNode = gep_rand_terminal(randSet);
							}
							else
							{
								tmpNode = gep_rand_variable(inputSize);
							}
						}

						ga.edit_chro(poolSize + i, j, tmpNode);
					}
				}
			}

			// Order
			ga.order(fitness_single, 1, (void*)tra);
			/*
			tmpFitness = fitness(ga.get_chro(0), (void*)tra);
			cout << "Turn " << turn <<  ", mse: " <<  tmpFitness << endl;
			gep_print_chro(ga.get_chro(0), cout);
			cout << endl;
			*/

			// Kill after
			ga.kill_after(limitPoolSize - 1);
		}

	}

	return 0;
}

double fitness_single(vector<union GEP_NODE> chro, void* arg)
{
	csv_t csv = (csv_t)arg;

	int rows = csv->rows;
	int cols = csv->cols;
	double* data = csv->data;

	static int counter = 0;
	
	int i = counter++;
	if(counter == rows)
		counter = 0;

	// Set inputs
	vector<double> inputs;
	for(int j = 0; j < csv->cols - 1; j++)
	{
		inputs.push_back(data[i * cols + j]);
	}

	// Find error
	double out = gep_tree_calc(chro, inputs);
	double mse = pow(out - data[(i + 1) * cols - 1], 2);
	
	return mse;
}

double fitness(vector<union GEP_NODE> chro, void* arg)
{
	csv_t csv = (csv_t)arg;

	int rows = csv->rows;
	int cols = csv->cols;
	double* data = csv->data;

	double mse = 0;
	for(int i = 0; i < csv->rows; i++)
	{
		// Set inputs
		vector<double> inputs;
		for(int j = 0; j < csv->cols - 1; j++)
		{
			inputs.push_back(data[i * cols + j]);
		}

		// Find error
		double out = gep_tree_calc(chro, inputs);
		mse += pow(out - data[(i + 1) * cols - 1], 2);
	}
	
	return mse / (double)csv->rows;
}

vector<int> gen_rand_index_vector(int size, int swap)
{
	vector<int> indexVector;
	indexVector.reserve(size);
	for(int i = 0; i < size; i++)
	{
		indexVector.push_back(i);
	}

	for(int i = 0; i < swap; i++)
	{
		int index1 = rand() % size;
		int index2 = rand() % size;

		int tmpValue = indexVector[index1];
		indexVector[index1] = indexVector[index2];
		indexVector[index2] = tmpValue;
	}

	return indexVector;
}

vector<char> gen_crossover_mask(int size, int swap)
{
	vector<char> mask;
	mask.reserve(size);
	for(int i = 0; i < size; i++)
	{
		mask.push_back(rand() % 2);
	}

	return mask;
}

