#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

#include <csv_dataproc.h>
#include <galib.hpp>
#include <gep.hpp>

#define RAND_PRECISION	1000
#define RAND_NUM_MAX	1
#define RAND_NUM_MIN	-1
#define RAND_EXP_MAX	3
#define RAND_EXP_MIN	-3

#define LEVEL	4

#define MUT_RATE		0.9
#define MUT_DECAY		0.996
#define ITER_LIMIT		30000
#define RESTART			10
#define RESTART_ADD		10
#define POOL_WEIGHT		10

#define RANGE_MIN	0
#define RANGE_MAX	10
#define RANGE_STEP	0.1

#define TRA_CSV_PATH	"booth_func_tra.csv"
#define TST_CSV_PATH	"booth_func_tst.csv"

#define DEBUG

using namespace std;
using namespace gep;

double fitness(vector<union GEP_NODE> chro, void* arg);
vector<int> gen_rand_index_vector(int size, int swap);
double* dataprep(int rows);

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
		// Restart
		if(counter % RESTART == 0)
		{
			ga.remove_same_chro();
			for(int i = ga.pool_size(); i < limitPoolSize * 2; i++)
			{
				ga.insert(gep_rand_chro(randSet, LEVEL, inputSize));
			}

			mutRate = MUT_RATE * (tmpFitness / (double)counter);
		}

		// Generate random index array
		int poolSize = ga.pool_size();
		vector<int> randIndex = gen_rand_index_vector(poolSize, poolSize * poolSize / 2);

		// Crossover
		for(int i = 0; i < poolSize; i++)
		{
			ga.crossover(randIndex[i], randIndex[(i + 1) % poolSize], rand() % (chroLen - 1));
		}

		// Mutation
		for(int i = 0; i < 2 * poolSize; i++)
		{
			for(int j = 0; j < chroLen; j++)
			{
				if(rand() % 100 < mutRate * 100)
				{
					union GEP_NODE tmpNode = gep_rand_node(randSet, inputSize);

					if(j >= headSize && tmpNode.type == GEP_TYPE_OPERATOR)
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
		ga.order(fitness, 1, (void*)tra);
		tmpFitness = fitness(ga.get_chro(0), (void*)tra);
		cout << "Iter " << counter <<  ", mse: " <<  tmpFitness << endl;
		gep_print_chro(ga.get_chro(0), cout);
		cout << endl;

		// Kill after
		ga.kill_after(limitPoolSize - 1);

	}

	return 0;
}

double fitness(vector<union GEP_NODE> chro, void* arg)
{
	csv_t csv = (csv_t)arg;
	
	double mse;
	for(int i = 0; i < csv->rows; i++)
	{
		// Set inputs
		vector<double> inputs;
		for(int j = 0; j < csv->cols - 1; j++)
		{
			inputs.push_back(csv_get_value(csv, i, j));
		}

		// Find error
		double out = gep_tree_calc(chro, inputs);
		mse += pow(out - csv_get_value(csv, i, csv->cols - 1), 2);
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

