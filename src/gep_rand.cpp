#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

#include <gep.hpp>

using namespace std;

namespace gep
{
	vector<union GEP_NODE> gep_rand_chro(struct GEP_RAND randSet, int treeLevel, int inputs)
	{
		vector<union GEP_NODE> tmpChro;

		// Find chromosome length and head size
		int headSize = (1 << (treeLevel - 1)) - 1;
		int tailLen = 1 << (treeLevel - 1);

		// Rand head
		for(int i = 0; i < headSize; i++)
		{
			tmpChro.push_back(gep_rand_op());
		}

		// Rand terminal
		for(int i = 0; i < tailLen; i++)
		{
			tmpChro.push_back(gep_rand_variable(inputs));
		}

		return tmpChro;
	}

	union GEP_NODE gep_rand_variable(int inputs)
	{
		union GEP_NODE tmpNode;
		
		tmpNode.type = GEP_TYPE_VARIABLE;
		tmpNode.var.index = rand() % inputs;

		return tmpNode;
	}

	union GEP_NODE gep_rand_op()
	{
		union GEP_NODE tmpNode;

		tmpNode.type = GEP_TYPE_OPERATOR;
		tmpNode.op.prefixOp = rand() % GEP_OP1T_AMOUNT;
		tmpNode.op.op = rand() % GEP_OP2T_AMOUNT;

		return tmpNode;
	}

	union GEP_NODE gep_rand_terminal(struct GEP_RAND randSet)
	{
		union GEP_NODE tmpNode;

		tmpNode.type = GEP_TYPE_TERMINAL;
		tmpNode.t.value = gep_rand_real(
				randSet.numMax,
				randSet.numMin,
				randSet.expMax,
				randSet.expMin,
				randSet.precision
				);

		return tmpNode;
	}

	union GEP_NODE gep_rand_node(struct GEP_RAND randSet, int inputs)
	{
		union GEP_NODE tmpNode;
		tmpNode.type = rand() % GEP_TYPE_AMOUNT;
		switch(tmpNode.type)
		{
			case GEP_TYPE_TERMINAL:
				tmpNode.t.value = gep_rand_real(
						randSet.numMax,
						randSet.numMin,
						randSet.expMax,
						randSet.expMin,
						randSet.precision
						);
				break;

			case GEP_TYPE_OPERATOR:
				if(rand() % 2 == 0)
				{
					tmpNode.op.prefixOp = rand() % GEP_OP1T_AMOUNT;
				}
				else
				{
					tmpNode.op.op = rand() % GEP_OP2T_AMOUNT;
				}

				break;

			case GEP_TYPE_VARIABLE:
				tmpNode.var.index = rand() % inputs;
				break;
		}

		return tmpNode;
	}

	double gep_rand_real(int numMax, int numMin, int expMax, int expMin, int precision)
	{
		int randRange;
		int randExp;
		double randNum;

		randRange = numMax - numMin + 1;
		randRange *= precision;
		
		randExp = rand() % (expMax - expMin + 1) + expMin;
		randNum =  (double)(rand() % randRange) / (double)precision + numMin;

		return pow(randNum, randExp);
	}
}

