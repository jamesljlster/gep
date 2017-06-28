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

	// Create model
	union GEP_NODE tmpNode;
	vector<union GEP_NODE> gepModel;

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_ADD;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_ADD;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_ADD;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_DIV;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_SQRT;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_ADD;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_SIN;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_SUB;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_MUL;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_MUL;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_SUB;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_SIN;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_SIN;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_MUL;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_OPERATOR;
	tmpNode.op.op = GEP_OP_TAKE_LEFT;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 4;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 5;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_TERMINAL;
	tmpNode.t.value = 48.6125;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_TERMINAL;
	tmpNode.t.value = 0.131;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 0;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 8;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_TERMINAL;
	tmpNode.t.value = 1;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 8;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_TERMINAL;
	tmpNode.t.value = 0.5041;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_TERMINAL;
	tmpNode.t.value = 2.56;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 2;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 0;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 8;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_TERMINAL;
	tmpNode.t.value = -0.487;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_VARIABLE;
	tmpNode.var.index = 6;
	gepModel.push_back(tmpNode);

	tmpNode.type = GEP_TYPE_TERMINAL;
	tmpNode.t.value = -0.031;
	gepModel.push_back(tmpNode);

	// Print model
	cout << "Using gep model: ";
	gep_print_chro(gepModel, cout);
	cout << endl;

	// Find training and testing fitness
	int inputSize = tra->cols - 1;
	cout << "Training fitness: " << fitness(gepModel, (void*)tra) << endl;
	cout << "Testing fitness: " << fitness(gepModel, (void*)tst) << endl;

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

