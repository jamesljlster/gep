#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

#include <gep.hpp>

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
	tmpChro = gep_rand_chro(randSet, 4, 4);

	// Print chromosome
	gep_print_chro(tmpChro, cout);

	return 0;
}
