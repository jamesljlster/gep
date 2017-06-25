#include <cmath>

#include <gep.hpp>

using namespace std;

namespace gep
{
	double gep_add(double a, double b)
	{
		return a + b;
	}

	double gep_sub(double a, double b)
	{
		return a - b;
	}

	double gep_mul(double a, double b)
	{
		return a * b;
	}

	double gep_div(double a, double b)
	{
		if(b != 0)
			return a / b;
		else
			return 0;
	}

	double gep_take_left(double a, double b)
	{
		return a;
	}

	double gep_take_right(double a, double b)
	{
		return b;
	}

	double gep_sqrt(double a, double b)
	{
		if(a < 0)
			return -sqrt(fabs(a));
		else
			return sqrt(a);
	}

	struct GEP_FUNC gep_func_box[] = {
		{"+",	gep_add},
		{"-",	gep_sub},
		{"*",	gep_mul},
		{"/",	gep_div},
		{"tl",	gep_take_left},
		{"tr",	gep_take_right},
		{"sqrt",	gep_sqrt}
	};
}

