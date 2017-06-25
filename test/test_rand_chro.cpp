#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

#include <gep.hpp>

#define LEVEL	4
#define INPUTS	4

using namespace std;
using namespace gep;

int main()
{
	struct GEP_RAND randSet;
	vector<union GEP_NODE> tmpChro;

	srand(time(NULL));

	// Set random setting
	randSet.numMax = 1;
	randSet.numMin = -1;
	randSet.expMax = 5;
	randSet.expMin = -5;
	randSet.precision = 1000;

	// Generate random chromosome
	tmpChro = gep_rand_chro(randSet, LEVEL, INPUTS);

	// Print chromosome
	gep_print_chro(tmpChro, cout);

	// Calculate test
	vector<double> inputs;
	for(int i = 0; i < INPUTS; i++)
	{
		inputs.push_back(i + 1);
		cout << "Input " << i << ": " << i + 1 << endl;
	}
	cout << "Result: " << gep_tree_calc(tmpChro, inputs) << endl;

	return 0;
}
