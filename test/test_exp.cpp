#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

#include <galib.hpp>
#include <gep.hpp>

#define RAND_PRECISION	1000
#define RAND_NUM_MAX	1
#define RAND_NUM_MIN	-1
#define RAND_EXP_MAX	3
#define RAND_EXP_MIN	-3

#define LEVEL	4
#define INPUTS	1

#define MUT_RATE		0.01
#define ITER_LIMIT		10000
#define RESTART			100
#define RESTART_ADD		10
#define POOL_WEIGHT		10

#define RANGE_MIN	0
#define RANGE_MAX	10
#define RANGE_STEP	0.1

#define DEBUG

struct FIT_DATA
{
	double* dataset;
	int dataInst;
};

using namespace std;
using namespace gep;

double fitness(vector<union GEP_NODE> chro, void* arg);
vector<int> gen_rand_index_vector(int size, int swap);
double* dataprep(int rows);

int main()
{
	srand(time(NULL));

	// Prepare dataset
	int dataInst = (RANGE_MAX - RANGE_MIN) / RANGE_STEP + 1;
	double* dataset = dataprep(dataInst);
#ifdef DEBUG
	for(int i = 0; i < dataInst; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			cout << dataset[i * 2 + j];
			if(j == 1)
			{
				cout << endl;
			}
			else
			{
				cout << ", ";
			}
		}
	}
#endif

	struct FIT_DATA fitData;
	fitData.dataset = dataset;
	fitData.dataInst = dataInst;

	// Create ga pool
	int chroLen = (1 << LEVEL) - 1;
	int headSize = (1 << (LEVEL - 1)) - 1;
	gaPool<union GEP_NODE> ga;
	ga.set_chro_len(chroLen);

	// Insert random chromosomes to ga pool
	struct GEP_RAND randSet = {RAND_NUM_MAX, RAND_NUM_MIN, RAND_EXP_MAX, RAND_EXP_MIN, RAND_PRECISION};
	for(int i = 0; i < chroLen * POOL_WEIGHT; i++)
	{
		ga.insert(gep_rand_chro(randSet, LEVEL, INPUTS));
	}
	
	int limitPoolSize = ga.pool_size();

	// Run task
	int counter = 0;
	while(counter++ < ITER_LIMIT)
	{
		// Restart
		if(counter % RESTART == 0)
		{
			ga.remove_same_chro();
			for(int i = ga.pool_size(); i < limitPoolSize + RESTART_ADD; i++)
			{
				ga.insert(gep_rand_chro(randSet, LEVEL, INPUTS));
			}
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
				if(rand() % 100 < MUT_RATE * 100)
				{
					union GEP_NODE tmpNode = gep_rand_node(randSet, INPUTS);

					if(j >= headSize && tmpNode.type == GEP_TYPE_OPERATOR)
					{
						if(rand() % 2 == 0)
						{
							tmpNode = gep_rand_terminal(randSet);
						}
						else
						{
							tmpNode = gep_rand_variable(INPUTS);
						}
					}

					ga.edit_chro(poolSize + i, j, tmpNode);
				}
			}
		}

		// Order
		ga.order(fitness, 1, (void*)&fitData);
		double tmpFitness = fitness(ga.get_chro(0), (void*)&fitData);
		cout << "Iter " << counter <<  ", mse: " <<  tmpFitness << endl;
		gep_print_chro(ga.get_chro(0), cout);
		cout << endl;

		// Kill after
		ga.kill_after(limitPoolSize - 1);
	}

	delete[] dataset;

	return 0;
}

double fitness(vector<union GEP_NODE> chro, void* arg)
{
	struct FIT_DATA* fitData = (struct FIT_DATA*)arg;
	
	double mse;
	for(int i = 0; i < fitData->dataInst; i++)
	{
		// Set inputs
		vector<double> inputs;
		inputs.push_back(fitData->dataset[i * 2]);

		// Find error
		double out = gep_tree_calc(chro, inputs);
		mse += pow(out - fitData->dataset[i * 2 + 1], 2);
	}
	
	return mse / (double)fitData->dataInst;
}

double* dataprep(int rows)
{
	double* data = new double[rows * 2];

	for(int i = RANGE_MIN; i < rows; i++)
	{
		data[i * 2] = i * RANGE_STEP;
		data[i * 2 + 1] = exp(0.5 * data[i * 2]);
	}

	return data;
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

