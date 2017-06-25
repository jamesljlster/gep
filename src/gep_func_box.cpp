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

	double gep_min(double a, double b)
	{
		if(a < b)
		{
			return a;
		}
		else
		{
			return b;
		}
	}

	double gep_max(double a, double b)
	{
		if(a > b)
		{
			return a;
		}
		else
		{
			return b;
		}
	}

	double gep_exp(double a, double b)
	{
		return exp(a);
	}

	double gep_avg(double a, double b)
	{
		return (a + b) / 2.0;
	}

	double gep_abs(double a, double b)
	{
		return fabs(a);
	}

	double gep_log(double a, double b)
	{
		return log(a);
	}

	double gep_inv(double a, double b)
	{
		if(a != 0)
		{
			return 1.0 / a;
		}
		else
		{
			return 0;
		}
	}

	struct GEP_FUNC gep_func_box[] = {
		{"+",	gep_add},
		{"-",	gep_sub},
		{"*",	gep_mul},
		{"/",	gep_div},
		{"tl",	gep_take_left},
		{"tr",	gep_take_right},
		{"sqrt",	gep_sqrt},
		{"min",	gep_min},
		{"max",	gep_max},
		{"exp", gep_exp},
		{"avg", gep_avg},
		{"abs", gep_abs},
		{"log", gep_log},
		{"inv", gep_inv}
	};
}

