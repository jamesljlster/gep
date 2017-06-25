#ifndef __GEP_H__
#define __GEP_H__

#include <iostream>
#include <vector>

namespace gep
{
	enum GEP_OP
	{
		GEP_OP_ADD,
		GEP_OP_SUB,
		GEP_OP_MUL,
		GEP_OP_DIV,
		GEP_OP_TAKE_LEFT,
		GEP_OP_TAKE_RIGHT,
		GEP_OP_SQRT,
		GEP_OP_MIN,
		GEP_OP_MAX,
		GEP_OP_EXP,
		GEP_OP_AVG,
		GEP_OP_ABS,
		GEP_OP_LOG,
		GEP_OP_INV,

		GEP_OP_AMOUNT
	};

	enum GEP_TYPE
	{
		GEP_TYPE_TERMINAL,
		GEP_TYPE_OPERATOR,
		GEP_TYPE_VARIABLE,

		GEP_TYPE_AMOUNT
	};
	
	struct GEP_FUNC
	{
		const char* name;
		double (*func)(double, double);
	};

	struct GEP_RAND
	{
		int numMax;
		int numMin;
		int expMax;
		int expMin;
		int precision;
	};

	struct GEP_TERMINAL
	{
		int type;
		double value;
	};

	struct GEP_OPERATOR
	{
		int type;
		int op;
	};

	struct GEP_VARIABLE
	{
		int type;
		int index;
	};

	union GEP_NODE
	{
		int type;
		struct GEP_TERMINAL t;
		struct GEP_OPERATOR op;
		struct GEP_VARIABLE var;
	};

	extern struct GEP_FUNC gep_func_box[];

	double gep_rand_real(int numMax, int numMin, int expMax, int expMin, int precision);
	std::vector<union GEP_NODE> gep_rand_chro(struct GEP_RAND randSet, int treeLevel, int inputs);
	union GEP_NODE gep_rand_node(struct GEP_RAND randSet);

	union GEP_NODE gep_rand_op();
	union GEP_NODE gep_rand_terminal(struct GEP_RAND randSet);
	union GEP_NODE gep_rand_variable(int inputs);

	void gep_print_chro(std::vector<union GEP_NODE> chro, std::ostream& os);

	double gep_tree_calc(std::vector<union GEP_NODE> chro, std::vector<double> inputs);
}

#endif
