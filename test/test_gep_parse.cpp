#include <iostream>

#include <gep.hpp>

using namespace std;
using namespace gep;

int main(int argc, char* argv[])
{
	if(argc < 1)
	{
		cout << "Pass gep chromosome string to run the program" << endl;
		return -1;
	}
	else
	{
		cout << "Input: " << argv[1] << endl;
	}
	
	vector<union GEP_NODE> tmpChro = gep_parse(argv[1]);
	cout << "Parsed: ";
	gep_print_chro(tmpChro, cout);
	cout << endl;

	return 0;
}
