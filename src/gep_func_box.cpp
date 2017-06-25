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

	double gep_op2t_min(double a, double b)
	{
		if(a > b)
		{
			return b;
		}
		else
		{
			return a;
		}
	}

	double gep_op2t_max(double a, double b)
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

	double gep_op2t_pow(double a, double b)
	{
		return pow(a, b);
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

	double gep_op1t_pow2(double a)
	{
		return pow(a, 2);
	}

	double gep_op1t_pow3(double a)
	{
		return pow(a, 3);
	}

	double gep_op1t_exp(double a)
	{
		return exp(a);
	}

	double gep_op1t_abs(double a)
	{
		return fabs(a);
	}

	double gep_op1t_log(double a)
	{
		return log(a);
	}

	double gep_op1t_inv(double a)
	{
		if(a != 0)
			return 1.0 / a;
		else
			return 0;
	}

	struct GEP_FUNC_OP2T gep_op2t_func_box[] = {
		{"+",	gep_op2t_add},
		{"-",	gep_op2t_sub},
		{"*",	gep_op2t_mul},
		{"/",	gep_op2t_div},
		{"tl",	gep_op2t_take_left},
		{"tr",	gep_op2t_take_right},
		{"min",	gep_op2t_min},
		{"max", gep_op2t_max},
		{"pow",	gep_op2t_pow}
	};

	struct GEP_FUNC_OP1T gep_op1t_func_box[] = {
		{"",		gep_op1t_linear},
		{"sqrt",	gep_op1t_sqrt},
		{"pow2",	gep_op1t_pow2},
		{"pow3",	gep_op1t_pow3},
		{"exp",		gep_op1t_exp},
		{"abs",		gep_op1t_abs},
		{"log",		gep_op1t_log},
		{"inv",		gep_op1t_inv}
	};

}

