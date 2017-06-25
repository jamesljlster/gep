#ifndef __GEP_H__
#define __GEP_H__

#include <iostream>
#include <vector>

namespace gep
{
	enum GEP_OP2T
	{
		GEP_OP2T_ADD,
		GEP_OP2T_SUB,
		GEP_OP2T_MUL,
		GEP_OP2T_DIV,
		GEP_OP2T_TAKE_LEFT,
		GEP_OP2T_TAKE_RIGHT,
		GEP_OP2T_MIN,
		GEP_OP2T_MAX,
		GEP_OP2T_POW,

		GEP_OP2T_AMOUNT
	};

	enum GEP_OP1T
	{
		GEP_OP1T_LINEAR,
		GEP_OP1T_SQRT,
		GEP_OP1T_POW2,
		GEP_OP1T_POW3,
		GEP_OP1T_EXP,
		GEP_OP1T_ABS,

		GEP_OP1T_AMOUNT
	}; 

	enum GEP_TYPE
	{
		GEP_TYPE_TERMINAL,
		GEP_TYPE_OPERATOR,
		GEP_TYPE_VARIABLE,

		GEP_TYPE_AMOUNT
	};

	struct GEP_FUNC_OP1T
	{
		const char* name;
		double (*func)(double);
	};
	
	struct GEP_FUNC_OP2T
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
		int prefixOp;
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

	extern struct GEP_FUNC_OP2T gep_op2t_func_box[];
	extern struct GEP_FUNC_OP1T gep_op1t_func_box[];

	double gep_rand_real(int numMax, int numMin, int expMax, int expMin, int precision);
	std::vector<union GEP_NODE> gep_rand_chro(struct GEP_RAND randSet, int treeLevel, int inputs);
	union GEP_NODE gep_rand_node_fixed(union GEP_NODE node, struct GEP_RAND randSet, int inputs);
	union GEP_NODE gep_rand_node(struct GEP_RAND randSet, int inputs);

	union GEP_NODE gep_rand_op();
	union GEP_NODE gep_rand_terminal(struct GEP_RAND randSet);
	union GEP_NODE gep_rand_variable(int inputs);

	void gep_print_chro(std::vector<union GEP_NODE> chro, std::ostream& os);

	double gep_tree_calc(std::vector<union GEP_NODE> chro, std::vector<double> inputs);
}

#endif
