#include <cmath>

#include <gep.hpp>

using namespace std;

namespace gep
{
	double gep_op2t_add(double a, double b)
	{
		return a + b;
	}

	double gep_op2t_sub(double a, double b)
	{
		return a - b;
	}

	double gep_op2t_mul(double a, double b)
	{
		return a * b;
	}

	double gep_op2t_div(double a, double b)
	{
		if(b != 0)
			return a / b;
		else
			return 0;
	}

	double gep_op2t_take_left(double a, double b)
	{
		return a;
	}

	double gep_op2t_take_right(double a, double b)
	{
		return b;
	}

	double gep_op1t_linear(double a)
	{
		return a;
	}

	double gep_op1t_sqrt(double a)
	{
		if(a < 0)
			return -sqrt(fabs(a));
		else
			return sqrt(a);
	}

	struct GEP_FUNC_OP2T gep_op2t_func_box[] = {
		{"+",	gep_op2t_add},
		{"-",	gep_op2t_sub},
		{"*",	gep_op2t_mul},
		{"/",	gep_op2t_div},
		{"tl",	gep_op2t_take_left},
		{"tr",	gep_op2t_take_right}
	};

	struct GEP_FUNC_OP1T gep_op1t_func_box[] = {
		{"",		gep_op1t_linear},
		{"sqrt",	gep_op1t_sqrt}
	};

}

