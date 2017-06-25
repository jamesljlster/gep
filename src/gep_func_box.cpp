#include <cmath>

#include <gep.hpp>

using namespace std;

namespace gep
{
	double (*gep_op2t_func_box[])(double, double) = {
		gep_op2t_add,
		gep_op2t_sub,
		gep_op2t_mul,
		gep_op2t_div
	};

	double (*gep_op1t_func_box[])(double) = {
		gep_op1t_linear,
		gep_op1t_sqrt
	};

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

	double gep_op1t_linear(double a)
	{
		return a;
	}

	double gep_op1t_sqrt(double a)
	{
		return sqrt(a);
	}
}
