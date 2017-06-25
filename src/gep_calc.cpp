
#include <gep.hpp>

using namespace std;

namespace gep
{
	double gep_tree_calc_recursive(vector<union GEP_NODE> chro, vector<double> inputs, int layerIndex, int index)
	{
		double ret;
		int nodes = 1 << layerIndex;
		int base = nodes - 1;
		
		// Checking
		int nodeIndex = base + index;
		if(nodeIndex >= chro.size())
		{
			string tmp = __FUNCTION__;
			tmp += "(): operation out of range!";
			throw std::invalid_argument(tmp);
		}

		// Calculate
		int tmpIndex;
		switch(chro[nodeIndex].type)
		{
			case GEP_TYPE_TERMINAL:
				ret = chro[nodeIndex].t.value;
				break;

			case GEP_TYPE_VARIABLE:
				// Checking
				tmpIndex = chro[nodeIndex].var.index;
				if(tmpIndex >= inputs.size())
				{
					string tmp = __FUNCTION__;
					tmp += "(): input index out of range!";
					throw std::invalid_argument(tmp);
				}
				else
				{
					ret = inputs[tmpIndex];
				}

				break;

			case GEP_TYPE_OPERATOR:
				// Calculate leaf values
				double ter1 = gep_tree_calc_recursive(chro, inputs, layerIndex + 1, index * 2);
				double ter2 = gep_tree_calc_recursive(chro, inputs, layerIndex + 1, index * 2 + 1);
				ret = gep_op1t_func_box[chro[nodeIndex].op.prefixOp].func(
						gep_op2t_func_box[chro[nodeIndex].op.op].func(ter1, ter2)
						);
				break;

		}

		return ret;
	}

	double gep_tree_calc(vector<union GEP_NODE> chro, vector<double> inputs)
	{
		return gep_tree_calc_recursive(chro, inputs, 0, 0);
	}
}

