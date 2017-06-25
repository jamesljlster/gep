#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

#include <gep.hpp>

using namespace std;

namespace gep
{
	void gep_print_terminal(union GEP_NODE node, ostream& os)
	{
		os << node.t.value;
	}

	void gep_print_operator(union GEP_NODE node, ostream& os)
	{
		os << gep_op1t_func_box[node.op.prefixOp].name;
		os << gep_op2t_func_box[node.op.op].name;
	}

	void gep_print_variable(union GEP_NODE node, ostream& os)
	{
		os << "var" << node.var.index;
	}

	void gep_print_chro(std::vector<union GEP_NODE> chro, ostream& os)
	{
		for(int i = 0; i < chro.size(); i++)
		{
			switch(chro[i].type)
			{
				case GEP_TYPE_TERMINAL:
					gep_print_terminal(chro[i], os);
					break;

				case GEP_TYPE_OPERATOR:
					gep_print_operator(chro[i], os);
					break;

				case GEP_TYPE_VARIABLE:
					gep_print_variable(chro[i], os);
					break;

				default:
					os << "Invalid";
			}

			if(i == chro.size() - 1)
			{
				os << endl;
			}
			else
			{
				os << ", ";
			}
		}
	}
}

